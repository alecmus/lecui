/*
** form.cpp - form implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "../form.h"
#include "../page.h"
#include "../controls.h"
#include "../appearance.h"
#include "../rectangle.h"
#include "../label.h"
#include "../timer.h"
#include "../group.h"
#include "../tab_control.h"
#include "../button.h"
#include "../pane.h"

#include "form_common.h"
#include "widgets/page/page_impl.h"
#include "widgets/control_buttons/close_button/close_button.h"
#include "widgets/control_buttons/maximize_button/maximize_button.h"
#include "widgets/control_buttons/minimize_button/minimize_button.h"
#include "widgets/h_scrollbar/h_scrollbar.h"
#include "widgets/v_scrollbar/v_scrollbar.h"

#include "../error/win_error.h"

// Windows headers
#include <Windows.h>
#include <ShlObj.h>		// for SHGetFolderPath
#include <dwmapi.h>		// for DwmExtendFrameIntoClientArea
#pragma comment(lib, "Dwmapi.lib")

// Direct2D and DirectWrite headers
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// COM
#include <comdef.h>

// C++ STL headers
#include <atomic>
#include <vector>
#include <map>
#include <optional>
#include <memory>

#if defined(min)
	#undef min	// to circumvent conflict with liblec::lecui::dimensions::min
#endif

#define DESIGNLINES	0	// set to 1 to show design lines, 0 otherwise

class mouse_track {
	bool mouse_tracking_;

public:
	mouse_track() : mouse_tracking_(false) {}

	void on_mouse_move(HWND hWnd) {
		if (!mouse_tracking_) {
			// Enable mouse tracking
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.dwHoverTime = HOVER_DEFAULT;
			TrackMouseEvent(&tme);
			mouse_tracking_ = true;
		}
	}

	void reset(HWND hWnd) { mouse_tracking_ = false; }
};

class liblec::lecui::form::form_impl {
public:
	form_impl(const std::string& caption) :
		p_parent_(nullptr),
		parent_closing_(false),
		show_called_(false),
		caption_bar_height_(30.f),
		form_border_thickness_(1.f),
		page_tolerance_(form_border_thickness_ / 2.f),
		control_button_margin_(2.f),
		resource_dll_filename_(std::string()),
		resource_module_handle_(nullptr),
		idi_icon_(0),
		idi_icon_small_(0),
		clr_background_(liblec::lecui::color{ 244, 244, 244 }),
		clr_titlebar_background_(clr_background_),
		clr_theme_(liblec::lecui::color{ 20, 80, 140, 255 }),
		clr_theme_hot_(liblec::lecui::color{ 255, 180, 0, 255 }),
		clr_theme_disabled_(liblec::lecui::color{ 225, 225, 225, 255 }),
		top_most_(false),
		hWnd_(nullptr),
		hWnd_parent_(nullptr),
		caption_(caption),
		activate_(true),
		point_({ 0, 0 }),
		size_({ 780, 480 }),
		min_size_({ 500, 300 }),
		allow_resizing_(true),
		allow_minimize_(true),
		user_pos_(false),
		preset_pos_(false),
		form_position_(liblec::lecui::form_position::center_to_working_area),
		dpi_scale_(1.0f),
		borderless_(true),
		borderless_shadow_(true),
		shadow_setting_before_maximize_(borderless_shadow_),
		p_render_target_(nullptr),
		p_brush_theme_(nullptr),
		p_brush_theme_hot_(nullptr),
		p_brush_theme_disabled_(nullptr),
		p_brush_titlebar_(nullptr),
		current_page_(std::string()),
		p_caption_(nullptr),
		p_close_button_(nullptr),
		p_maximize_button_(nullptr),
		p_minimize_button_(nullptr),
		point_before_({ 0.f, 0.f }),
		user_sizing_(false),
		unique_id_(1000),
		reverse_tab_navigation_(false),
		shift_pressed_(false),
		space_pressed_(false),
		new_page_added_(false) {
		log("entering form_impl constructor");

		++instances_;	// increment instances count

		/// Use HeapSetInformation to specify that the process should terminate if the heap manager
		/// detects an error in any heap used by the process. The return value is ignored, because
		/// we want to continue running in the unlikely event that HeapSetInformation fails.
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

		if (instances_ == 1) {
			// initialize COM
			HRESULT hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

			if (SUCCEEDED(hres)) {
				// Create a Direct2D factory
				hres = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &p_direct2d_factory_);
			}
			if (SUCCEEDED(hres)) {
				// Create a DirectWrite factory.
				hres = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(p_directwrite_factory_),
					reinterpret_cast<IUnknown * *>(&p_directwrite_factory_));
			}

			if (FAILED(hres)) {
				// CoInitializeEx failed, get detailed error information
				_com_error com_error(hres);
				std::wstring error = com_error.ErrorMessage();
				error = L"An error occured while initializing the library: " + error;

				MessageBox(nullptr, error.c_str(), L"liblec::lecui::form", MB_ICONERROR);

				// initialization has failed. Go no further.
				return;
			}

			// set initialized flag to true (only here)
			initialized_ = true;
		}

		if (!IsProcessDPIAware()) {
			if (!SetProcessDPIAware())
				MessageBox(nullptr,
					L"This program is not DPI aware. As a result, UI elements may not be clear.",
					L"liblec::lecui::form", MB_ICONWARNING);
		}

		// capture current DPI scale
		HDC hdc_screen = GetDC(NULL);
		dpi_scale_ = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
		ReleaseDC(NULL, hdc_screen);

		// ...
	}

	~form_impl() {
		// ....

		if (initialized_ && instances_ == 1) {
			// release DirectWrite resources used by all instances
			safe_release(&p_directwrite_factory_);

			// release Direct2D resources used by all instances
			safe_release(&p_direct2d_factory_);

			// Uninitialize COM
			CoUninitialize();

			// set initialized flag to false (only here)
			initialized_ = false;
		}

		// release Direct2D resources specific to this form
		safe_release(&p_render_target_);
		safe_release(&p_brush_theme_);
		safe_release(&p_brush_theme_hot_);
		safe_release(&p_brush_theme_disabled_);
		safe_release(&p_brush_titlebar_);

		--instances_;	// decremement instances count

		log("exiting form_impl destructor");
	}

	/// Creates resources that are bound to a particular Direct3D device. These resources need to
	/// be recreated if the Direct3D device disappears, such as when the display changes, etc
	HRESULT create_device_resources() {
		HRESULT hr = S_OK;

		if (!p_render_target_) {
			RECT rc;
			GetClientRect(hWnd_, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

			// Create a Direct2D render target.
			hr = p_direct2d_factory_->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hWnd_, size),
				&p_render_target_);

			if (SUCCEEDED(hr)) {
				// create the theme brushes
				hr = p_render_target_->CreateSolidColorBrush(
					convert_color(clr_theme_), &p_brush_theme_);
				hr = p_render_target_->CreateSolidColorBrush(
					convert_color(clr_theme_hot_), &p_brush_theme_hot_);
				hr = p_render_target_->CreateSolidColorBrush(
					convert_color(clr_theme_disabled_), &p_brush_theme_disabled_);
				hr = p_render_target_->CreateSolidColorBrush(
					convert_color(clr_titlebar_background_), &p_brush_titlebar_);
			}
			if (SUCCEEDED(hr)) {
				try {
					// create form widget resources
					for (auto& widget : widgets_)
						hr = widget.second.create_resources(p_render_target_);
				}
				catch (const std::exception& e) { log(e.what()); }
			}
			if (SUCCEEDED(hr)) {
				class helper {
				public:
					static void create_resources(const liblec::lecui::containers::page& page,
						ID2D1HwndRenderTarget* p_render_target_) {
						// create widget resources
						for (auto& widget : page.d_page_.widgets()) {
							HRESULT hr = widget.second.create_resources(p_render_target_);

							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::tab_control) {
								try {
									// get this tab control
									auto& tab_control = page.d_page_.get_tab_control(widget.first);

									for (auto& tab : tab_control.p_tabs_)
										create_resources(tab.second, p_render_target_);
								}
								catch (const std::exception&) {}
							}
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::pane) {
									try {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										for (auto& page : pane.p_panes_)
											create_resources(page.second, p_render_target_);
									}
									catch (const std::exception&) {}
								}
						}
					}
				};

				try {
					// create page resources
					for (auto& p_page : p_pages_)
						helper::create_resources(p_page.second, p_render_target_);
				}
				catch (const std::exception& e) { log(e.what()); }
			}
		}

		return hr;
	}

	/// Discards device-dependent resources. These resources must be recreated when the Direct3D
	/// device is lost
	void discard_device_resources() {
		safe_release(&p_render_target_);
		safe_release(&p_brush_theme_);
		safe_release(&p_brush_theme_hot_);
		safe_release(&p_brush_theme_disabled_);
		safe_release(&p_brush_titlebar_);

		class helper {
		public:
			static void discard(const liblec::lecui::containers::page& page) {
				// discard widget resources
				for (const auto& widget : page.d_page_.widgets()) {
					widget.second.discard_resources();

					if (widget.second.type() ==
						liblec::lecui::widgets_implementation::widget_type::tab_control) {
						try {
							// get this tab control
							const auto& tab_control = page.d_page_.get_tab_control(widget.first);

							for (const auto& tab : tab_control.p_tabs_)
								discard(tab.second);
						}
						catch (const std::exception&) {}
					}
					else
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::pane) {
							try {
								// get this pane
								const auto& pane = page.d_page_.get_pane(widget.first);

								for (const auto& page : pane.p_panes_)
									discard(page.second);
							}
							catch (const std::exception&) {}
						}
				}
			}
		};

		// discard page resources
		for (const auto& p_page : p_pages_)
			helper::discard(p_page.second);

		// discard form widget resources
		for (const auto& widget : widgets_)
			widget.second.discard_resources();
	}

	void create_close_button(std::function<void()> on_click) {
		p_close_button_ =
			std::unique_ptr<liblec::lecui::widgets_implementation::close_button>(new
			liblec::lecui::widgets_implementation::close_button());
		widgets_.emplace(p_close_button_->name(), *p_close_button_);
		widgets_order_.emplace_back(p_close_button_->name());

		p_close_button_->specs().color_fill = clr_theme_;
		p_close_button_->specs().color_hot = clr_theme_hot_;
		p_close_button_->specs().color_disabled = clr_theme_disabled_;
		p_close_button_->specs().resize.perc_x = 100;

		p_close_button_->specs().rect.right = size_.width -
			static_cast<long>(control_button_margin_);
		p_close_button_->specs().rect.top = static_cast<long>(control_button_margin_);
		p_close_button_->specs().rect.left = p_close_button_->specs().rect.right -
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);
		p_close_button_->specs().rect.bottom = p_close_button_->specs().rect.top +
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);

		p_close_button_->specs().on_click = on_click;
	}

	// should be called after create_close_button()
	void create_maximize_button() {
		p_maximize_button_ =
			std::unique_ptr<liblec::lecui::widgets_implementation::maximize_button>(new
				liblec::lecui::widgets_implementation::maximize_button());
		widgets_.emplace(p_maximize_button_->name(), *p_maximize_button_);
		widgets_order_.emplace_back(p_maximize_button_->name());

		p_maximize_button_->specs().color_fill = clr_theme_;
		p_maximize_button_->specs().color_hot = clr_theme_hot_;
		p_maximize_button_->specs().color_disabled = clr_theme_disabled_;
		p_maximize_button_->specs().resize.perc_x = 100;

		const long right_edge = p_close_button_->specs().rect.left;

		p_maximize_button_->specs().rect.right = static_cast<long>(right_edge) -
			static_cast<long>(control_button_margin_);
		p_maximize_button_->specs().rect.top = static_cast<long>(control_button_margin_);
		p_maximize_button_->specs().rect.left = p_maximize_button_->specs().rect.right -
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);
		p_maximize_button_->specs().rect.bottom = p_maximize_button_->specs().rect.top +
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);
	}

	// should be called after create_close_button() and create_maximize_button()
	void create_minimize_button() {
		p_minimize_button_ =
			std::unique_ptr<liblec::lecui::widgets_implementation::minimize_button>(new
				liblec::lecui::widgets_implementation::minimize_button());
		widgets_.emplace(p_minimize_button_->name(), *p_minimize_button_);
		widgets_order_.emplace_back(p_minimize_button_->name());

		p_minimize_button_->specs().color_fill = clr_theme_;
		p_minimize_button_->specs().color_hot = clr_theme_hot_;
		p_minimize_button_->specs().color_disabled = clr_theme_disabled_;
		p_minimize_button_->specs().resize.perc_x = 100;

		const long right_edge = allow_resizing_ ?
			p_maximize_button_->specs().rect.left :
			p_close_button_->specs().rect.left;

		p_minimize_button_->specs().rect.right = static_cast<long>(right_edge) -
			static_cast<long>(control_button_margin_);
		p_minimize_button_->specs().rect.top = static_cast<long>(control_button_margin_);
		p_minimize_button_->specs().rect.left = p_minimize_button_->specs().rect.right -
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);
		p_minimize_button_->specs().rect.bottom = p_minimize_button_->specs().rect.top +
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);
	}

	void create_form_caption(std::function<void()> on_click) {
		p_caption_ =
			std::unique_ptr<liblec::lecui::widgets_implementation::label>(new
				liblec::lecui::widgets_implementation::label("", "form_caption", p_directwrite_factory_));
		widgets_.emplace(p_caption_->name(), *p_caption_);
		widgets_order_.emplace_back(p_caption_->name());

		p_caption_->specs().text = caption_;
		p_caption_->specs().center_v = true;

		p_caption_->specs().resize.perc_width = 100;

		const long right_edge = allow_minimize_ ?
			p_minimize_button_->specs().rect.left :
			(allow_resizing_ ?
			p_maximize_button_->specs().rect.left :
			p_close_button_->specs().rect.left);

		p_caption_->specs().rect.right = static_cast<long>(right_edge) -
			static_cast<long>(control_button_margin_);
		p_caption_->specs().rect.top = static_cast<long>(control_button_margin_);
		p_caption_->specs().rect.left = 10;
		p_caption_->specs().rect.bottom = p_caption_->specs().rect.top +
			static_cast<long>(caption_bar_height_ - 2.0f * control_button_margin_);

		p_caption_->specs().on_click = on_click;
	}

	void update() { InvalidateRect(hWnd_, nullptr, FALSE); }

	/// This method discards device-specific resources if the Direct3D device dissapears during
	/// execution and recreates the resources the next time it's invoked
	HRESULT on_render() {
		if (new_page_added_) {
			new_page_added_ = false;	// reset flag
			discard_device_resources();
		}

		HRESULT hr = S_OK;

		hr = create_device_resources();

		if (SUCCEEDED(hr)) {
			p_render_target_->BeginDraw();

			p_render_target_->SetTransform(D2D1::Matrix3x2F::Identity());

			// fill form background
			p_render_target_->Clear(convert_color(clr_background_));

			// get render target size
			const D2D1_SIZE_F rtSize = p_render_target_->GetSize();

			// fill titlebar background
			const D2D1_RECT_F rect_titlebar_ = { 0.f, 0.f, rtSize.width, caption_bar_height_ };
			p_render_target_->FillRectangle(&rect_titlebar_, p_brush_titlebar_);

#if defined(_DEBUG) and DESIGNLINES
			// Draw a grid background
			int width = static_cast<int>(rtSize.width);
			int height = static_cast<int>(rtSize.height);

			const float line_width = 0.05f;

			for (int x = 0; x < width; x += 10)
				p_render_target_->DrawLine(D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
					D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
					p_brush_theme_, line_width);

			for (int y = 0; y < height; y += 10)
				p_render_target_->DrawLine(D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
					D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
					p_brush_theme_, line_width);
#endif

			class helper {
			public:
				static void render_page(bool allow_render,
					const std::string& page_name,
					const std::string& current_page,
					liblec::lecui::containers::page& page,
					ID2D1HwndRenderTarget* p_render_target_,
					const D2D1_RECT_F& rectB,
					const D2D1_RECT_F& client_area,
					const float& change_in_width,
					const float& change_in_height,
					const float& dpi_scale_,
					ID2D1SolidColorBrush* p_brush_theme_,
					ID2D1SolidColorBrush* p_brush_theme_hot_) {
					bool render = page_name == current_page;

					if (!allow_render)
						render = false;

					// define horizontal scroll parameters. rectA defines the area that contains the
					// widgets, while rectB defines the area in the main form outside of which
					// scrolling should kick in
					D2D1_RECT_F rectA = { 0.f, 0.f, 0.f, 0.f };

					{
						// clip
						auto_clip clip(render, p_render_target_, client_area, 1.f);

						// h_scrollbar
						{
							// impose limits
							if (page.d_page_.h_scrollbar().x_displacement_ < 0.f)
								page.d_page_.h_scrollbar().x_displacement_ =
								largest(page.d_page_.h_scrollbar().x_displacement_,
									page.d_page_.h_scrollbar().max_displacement_left_);
							else
								page.d_page_.h_scrollbar().x_displacement_ =
								smallest(page.d_page_.h_scrollbar().x_displacement_,
									page.d_page_.h_scrollbar().max_displacement_right_);

							// translate the displacement
							float x_displacement_translated_ = 0.f;
							if (page.d_page_.h_scrollbar().translate_x_displacement(
								page.d_page_.h_scrollbar().x_displacement_, x_displacement_translated_,
								page.d_page_.h_scrollbar().force_translate_)) {
								page.d_page_.h_scrollbar().force_translate_ = false;
								page.d_page_.h_scrollbar().x_off_set_ = x_displacement_translated_;
							}
						}

						// v_scrollbar
						{
							// impose limits
							if (page.d_page_.v_scrollbar().y_displacement_ < 0.f)
								page.d_page_.v_scrollbar().y_displacement_ =
								largest(page.d_page_.v_scrollbar().y_displacement_,
									page.d_page_.v_scrollbar().max_displacement_top_);
							else
								page.d_page_.v_scrollbar().y_displacement_ =
								smallest(page.d_page_.v_scrollbar().y_displacement_,
									page.d_page_.v_scrollbar().max_displacement_bottom_);

							// translate the displacement
							float y_displacement_translated_ = 0.f;
							if (page.d_page_.v_scrollbar().translate_y_displacement(
								page.d_page_.v_scrollbar().y_displacement_, y_displacement_translated_,
								page.d_page_.v_scrollbar().force_translate_)) {
								page.d_page_.v_scrollbar().force_translate_ = false;
								page.d_page_.v_scrollbar().y_off_set_ = y_displacement_translated_;
							}
						}

						// figure out furthest left and right

						// measure widgets
						bool initialized = false;
						D2D1_RECT_F rect_widgets_ = { 0.f, 0.f, 0.f, 0.f };
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::h_scrollbar ||
								widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::v_scrollbar ||
								widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::group)
								continue;

							rect_widgets_ = widget.second.render(p_render_target_,
								change_in_width, change_in_height,
								(page.d_page_.h_scrollbar().x_off_set_ / dpi_scale_) - client_area.left,
								(page.d_page_.v_scrollbar().y_off_set_ / dpi_scale_) - client_area.top,
								false);

							if (!initialized) {
								initialized = true;
								rectA = rect_widgets_;
							}
							else {
								rectA.left = smallest(rectA.left, rect_widgets_.left);
								rectA.right = largest(rectA.right, rect_widgets_.right);
								rectA.top = smallest(rectA.top, rect_widgets_.top);
								rectA.bottom = largest(rectA.bottom, rect_widgets_.bottom);
							}
						}

						// resize groupboxes
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() !=
								liblec::lecui::widgets_implementation::widget_type::group)
								continue;

							try {
								// get the groupbox specs
								auto& specs = page.d_page_.get_group(widget.first).specs();

								bool groupbox_initialized = false;
								for (auto& widget_name :
									page.d_page_.get_group(widget.first).specs().widgets) {
									try {
										// get the rect for this widget
										const auto& rect_ =
											page.d_page_.widgets().at(widget_name).get_rect();

										// adjust the groupbox rect
										if (!groupbox_initialized) {
											specs.rect = convert_rect(rect_);
											groupbox_initialized = true;
										}
										else {
											specs.rect.left = static_cast<long>(smallest(
												static_cast<float>(specs.rect.left), rect_.left));
											specs.rect.top = static_cast<long>(smallest(
												static_cast<float>(specs.rect.top), rect_.top));
											specs.rect.right = static_cast<long>(largest(
												static_cast<float>(specs.rect.right), rect_.right));
											specs.rect.bottom = static_cast<long>(largest(
												static_cast<float>(specs.rect.bottom), rect_.bottom));
										}
									}
									catch (const std::exception&) {}
								}

								// apply the margin
								specs.rect.left -= specs.margin;
								specs.rect.top -= specs.margin;
								specs.rect.right += specs.margin;
								specs.rect.bottom += specs.margin;

								// get bounding rect for all controls within this groupbox
							}
							catch (const std::exception&) {
								// to-do: make sure rect is within bounding rects for page
							}

						}

						// render groupboxes
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() !=
								liblec::lecui::widgets_implementation::widget_type::group)
								continue;

							try {
								// check if groupbox has widgets
								if (page.d_page_.get_group(widget.first).specs().widgets.empty())
									continue;

								// to-do: check if widgets actually exist and discontinue if they dont
							}
							catch (const std::exception&) {
								continue;
							}

							// render with no resizing or offset parameters because the rect for the
							// group is already properly set
							widget.second.render(p_render_target_,
								0.f, 0.f, 0.f, 0.f, render);
						}

						// render widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::h_scrollbar ||
								widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::v_scrollbar ||
								widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::group)
								continue;

							widget.second.render(p_render_target_,
								change_in_width, change_in_height,
								(page.d_page_.h_scrollbar().x_off_set_ / dpi_scale_) - client_area.left,
								(page.d_page_.v_scrollbar().y_off_set_ / dpi_scale_) - client_area.top,
								render);

							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::tab_control) {
								try {
									// get this tab control
									auto& tab_control = page.d_page_.get_tab_control(widget.first);

									// get client area for this tab control
									const auto& client_area = tab_control.client_area();

									const float change_in_width =
										(tab_control.tab_control_area().right - tab_control.tab_control_area().left) -
										static_cast<float>(tab_control.specs().rect.right -
										tab_control.specs().rect.left);
									const float change_in_height =
										(tab_control.tab_control_area().bottom - tab_control.tab_control_area().top) -
										static_cast<float>(tab_control.specs().rect.bottom -
										tab_control.specs().rect.top);

									for (auto& tab : tab_control.p_tabs_) {
										const float page_tolerance_ = 10.f;
										D2D1_RECT_F rect_page = client_area;
										rect_page.left += page_tolerance_;
										rect_page.top += page_tolerance_;
										rect_page.right -= page_tolerance_;
										rect_page.bottom -= page_tolerance_;

										render_page(render, tab.first, tab_control.current_tab_, tab.second,
											p_render_target_, rect_page, rect_page, change_in_width, change_in_height,
											dpi_scale_, p_brush_theme_, p_brush_theme_hot_);
									}
								}
								catch (const std::exception&) {}
							}
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::pane) {
									try {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										// get client area for this pane
										const auto& client_area = pane.client_area();

										const float change_in_width =
											(pane.pane_area().right - pane.pane_area().left) -
											static_cast<float>(pane.specs().rect.right -
												pane.specs().rect.left);
										const float change_in_height =
											(pane.pane_area().bottom - pane.pane_area().top) -
											static_cast<float>(pane.specs().rect.bottom -
												pane.specs().rect.top);

										for (auto& page : pane.p_panes_) {
											const float page_tolerance_ = 10.f;
											D2D1_RECT_F rect_page = client_area;
											rect_page.left += page_tolerance_;
											rect_page.top += page_tolerance_;
											rect_page.right -= page_tolerance_;
											rect_page.bottom -= page_tolerance_;

											render_page(render, page.first, pane.current_pane_, page.second,
												p_render_target_, rect_page, rect_page, change_in_width, change_in_height,
												dpi_scale_, p_brush_theme_, p_brush_theme_hot_);
										}
									}
									catch (const std::exception&) {}
								}
						}

#if defined(_DEBUG) and DESIGNLINES
						if (render) {
							// draw rectA and rectB
							p_render_target_->DrawRectangle(&rectA, p_brush_theme_, .5f);
							p_render_target_->DrawRectangle(&rectB, p_brush_theme_hot_, .5f);
						}
#endif
					}

					// setup horizontal scroll bar and render it
					page.d_page_.h_scrollbar().setup(rectA, rectB);
					page.d_page_.h_scrollbar().render(p_render_target_,
						change_in_width, change_in_height, 0.f - client_area.left,
						0.f - client_area.top, render);

					// setup vertical scroll bar and render it
					page.d_page_.v_scrollbar().setup(rectA, rectB);
					page.d_page_.v_scrollbar().render(p_render_target_,
						change_in_width, change_in_height, 0.f - client_area.left,
						0.f - client_area.top, render);
				}

				static void render_menu(ID2D1HwndRenderTarget* p_render_target_,
					const std::string& page_name,
					const std::string& current_page,
					liblec::lecui::containers::page& page,
					const D2D1_RECT_F& client_area) {
					bool render = page_name == current_page;

					if (!render)
						return;

					// clip
					auto_clip clip(render, p_render_target_, client_area, 1.f);

					// render widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::h_scrollbar ||
							widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::v_scrollbar ||
							widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::group)
							continue;

						widget.second.on_menu(p_render_target_, client_area);

						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::tab_control) {
							try {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								// get client area for this tab control
								const auto& client_area = tab_control.client_area();

								const float change_in_width =
									(tab_control.tab_control_area().right - tab_control.tab_control_area().left) -
									static_cast<float>(tab_control.specs().rect.right -
										tab_control.specs().rect.left);
								const float change_in_height =
									(tab_control.tab_control_area().bottom - tab_control.tab_control_area().top) -
									static_cast<float>(tab_control.specs().rect.bottom -
									tab_control.specs().rect.top);

								for (auto& tab : tab_control.p_tabs_) {
									const float page_tolerance_ = 10.f;
									D2D1_RECT_F rect_page = client_area;
									rect_page.left += page_tolerance_;
									rect_page.top += page_tolerance_;
									rect_page.right -= page_tolerance_;
									rect_page.bottom -= page_tolerance_;

									render_menu(p_render_target_, tab.first,
										tab_control.current_tab_, tab.second, rect_page);
								}
							}
							catch (const std::exception&) {}
						}
						else
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::pane) {
								try {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// get client area for this pane
									const auto& client_area = pane.client_area();

									const float change_in_width =
										(pane.pane_area().right - pane.pane_area().left) -
										static_cast<float>(pane.specs().rect.right -
											pane.specs().rect.left);
									const float change_in_height =
										(pane.pane_area().bottom - pane.pane_area().top) -
										static_cast<float>(pane.specs().rect.bottom -
											pane.specs().rect.top);

									for (auto& page : pane.p_panes_) {
										const float page_tolerance_ = 10.f;
										D2D1_RECT_F rect_page = client_area;
										rect_page.left += page_tolerance_;
										rect_page.top += page_tolerance_;
										rect_page.right -= page_tolerance_;
										rect_page.bottom -= page_tolerance_;

										render_menu(p_render_target_, page.first,
											pane.current_pane_, page.second, rect_page);
									}
								}
								catch (const std::exception&) {}
							}
					}
				}
			};

			const float change_in_width = rtSize.width - size_.width;
			const float change_in_height = rtSize.height - size_.height;

			for (auto& page : p_pages_) {
				const D2D1_RECT_F rect_page = { page_tolerance_,
					caption_bar_height_ + page_tolerance_,
					rtSize.width - page_tolerance_, rtSize.height - page_tolerance_ };

				const D2D1_RECT_F client_area = rect_page;

				helper::render_page(true, page.first, current_page_, page.second, p_render_target_,
					rect_page, client_area, change_in_width, change_in_height, dpi_scale_,
					p_brush_theme_, p_brush_theme_hot_);
			}

			// render form widgets
			for (auto& widget : widgets_)
				widget.second.render(p_render_target_,
					change_in_width, change_in_height, 0.f, 0.f, true);

			// render menu
			for (auto& page : p_pages_) {
				const D2D1_RECT_F rect_page = { page_tolerance_,
					caption_bar_height_ + page_tolerance_,
					rtSize.width - page_tolerance_, rtSize.height - page_tolerance_ };

				const D2D1_RECT_F client_area = rect_page;

				helper::render_menu(p_render_target_, page.first, current_page_,
					page.second, client_area);
			}

			// render form border
			if (!maximized(hWnd_)) {
				const D2D1_RECT_F form_rectangle =
					D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);
				p_render_target_->DrawRectangle(&form_rectangle,
					p_brush_theme_, form_border_thickness_);
			}

			hr = p_render_target_->EndDraw();
		}

		if (hr == D2DERR_RECREATE_TARGET) {
			hr = S_OK;
			discard_device_resources();
		}

		return hr;
	}

	/// If the application receives a WM_SIZE message, this method resizes the render target
	/// appropriately
	void on_resize(UINT width, UINT height) {
		if (p_render_target_) {
			/// Note: This method can fail, but it's okay to ignore the error here, because the
			/// error will be returned again the next time EndDraw is called
			p_render_target_->Resize(D2D1::SizeU(width, height));
		}
	}

	RECT get_working_area(HWND hWnd) {
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monitorinfo = { 0 };
		monitorinfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMonitor, &monitorinfo);

		unscale_RECT(monitorinfo.rcWork, dpi_scale_);
		return monitorinfo.rcWork;
	}

	D2D1_POINT_2F get_cursor_position() {
		POINT pt = { 0 };
		GetCursorPos(&pt);
		ScreenToClient(hWnd_, &pt);
		return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
	}

	void set_position(const int& ix, const int& iy,
		const long& icx, const long& icy) {
		size_.width = icx;
		size_.height = icy;

		// ensure visibility of top left
		point_.x = largest(ix, 0);
		point_.y = largest(iy, 0);
	}

	void set_position(const liblec::lecui::form_position& wndPos,
		const long& icx_in, const long& icy_in) {
		long icx = icx_in;
		long icy = icy_in;

		const int offset = 10;

		// get coordinates of working area
		RECT rcWork = get_working_area(GetDesktopWindow());

		liblec::lecui::form_position m_wndPos = wndPos;

		if (wndPos == liblec::lecui::form_position::center_to_parent &&
			hWnd_parent_ &&
			IsWindow(hWnd_parent_) &&
			(!IsWindowVisible(hWnd_parent_) || IsIconic(hWnd_parent_)))
			m_wndPos = liblec::lecui::form_position::center_to_working_area;

		switch (m_wndPos) {
		case liblec::lecui::form_position::center_to_parent: {
			if (hWnd_parent_ && IsWindow(hWnd_parent_)) {
				// get coordinates of parent window
				RECT rcParent;
				GetWindowRect(hWnd_parent_, &rcParent);
				unscale_RECT(rcParent, dpi_scale_);

				long user_width = rcParent.right - rcParent.left;
				long user_height = rcParent.bottom - rcParent.top;

				long ix = 0;
				long iy = 0;

				// center to working area
				ix = rcParent.left + ((user_width - icx) / 2);
				iy = rcParent.top + ((user_height - icy) / 2);

				point_.x = ix;
				point_.y = iy;
				size_.width = icx;
				size_.height = icy;

				break;
			}

			// allow to fall through to liblec::lecui::form_position::center_to_working_area
		}

		case liblec::lecui::form_position::center_to_working_area: {
			long user_width = rcWork.right - rcWork.left;
			long user_height = rcWork.bottom - rcWork.top;

			long ix = 0;
			long iy = 0;

			// center to working area
			ix = rcWork.left + ((user_width - icx) / 2);
			iy = rcWork.top + ((user_height - icy) / 2);

			point_.x = ix;
			point_.y = iy;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::top_left: {
			point_.x = rcWork.left;
			point_.y = rcWork.top;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::top_left_offset: {
			point_.x = rcWork.left + offset;
			point_.y = rcWork.top + offset;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::top_right: {
			point_.x = rcWork.right - icx;
			point_.y = rcWork.top;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::top_right_offset: {
			point_.x = rcWork.right - icx - offset;
			point_.y = rcWork.top + offset;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::bottom_right: {
			point_.x = rcWork.right - icx;
			point_.y = rcWork.bottom - icy;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::bottom_right_offset: {
			point_.x = rcWork.right - icx - offset;
			point_.y = rcWork.bottom - icy - offset;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::bottom_left: {
			point_.x = rcWork.left;
			point_.y = rcWork.bottom - icy;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		case liblec::lecui::form_position::bottom_left_offset: {
			point_.x = rcWork.left + offset;
			point_.y = rcWork.bottom - icy - offset;
			size_.width = icx;
			size_.height = icy;
		}
		break;

		default: {
			// default to top left
			point_.x = rcWork.left;
			point_.y = rcWork.top;
			size_.width = icx;
			size_.height = icy;
		}
		break;
		}

		// ensure visibility of top left
		point_.x = largest(point_.x, 0L);
		point_.y = largest(point_.y, 0L);
	}

	/// we cannot just use WS_POPUP style
	/// WS_THICKFRAME: without this the window cannot be resized and so aero snap, de-maximizing
	///  and minimizing won't work
	/// WS_SYSMENU: enables the context menu with the move, close, maximize, minize... commands
	///  (shift + right-click on the task bar item)
	/// WS_CAPTION: enables aero minimize animation/transition
	/// WS_MAXIMIZEBOX, WS_MINIMIZEBOX: enable minimize/maximize
	enum class style : DWORD {
		windowed = WS_OVERLAPPEDWINDOW |
		WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		aero_borderless = WS_POPUP |
		WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		basic_borderless = WS_POPUP |
		WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX
	};

	static bool composition_enabled() {
		BOOL composition_enabled = FALSE;
		bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
		return composition_enabled && success;
	}

	static style select_borderless_style() {
		return composition_enabled() ? style::aero_borderless : style::basic_borderless;
	}

	static void set_shadow(HWND hWnd, bool enabled) {
		if (composition_enabled()) {
			static const MARGINS shadow_state[2]{ { 0, 0, 0, 0 },{ 1, 1, 1, 1 } };
			::DwmExtendFrameIntoClientArea(hWnd, &shadow_state[enabled]);
		}
	}

	void set_borderless(HWND hWnd, bool enabled) {
		style new_style = (enabled) ? select_borderless_style() : style::windowed;
		style old_style = static_cast<style>(::GetWindowLongPtrW(hWnd, GWL_STYLE));

		if (new_style != old_style) {
			borderless_ = enabled;

			::SetWindowLongPtrW(hWnd, GWL_STYLE, static_cast<LONG>(new_style));

			if (!allow_resizing_) {
				SetWindowLong(hWnd_, GWL_STYLE,
					GetWindowLong(hWnd_,
						GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
			}

			// when switching between borderless and windowed, restore appropriate shadow state
			set_shadow(hWnd, borderless_shadow_ && (new_style != style::windowed));

			// redraw frame
			::SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		}
	}

	void set_borderless_shadow(HWND hWnd, bool enabled, bool save_value = true) {
		if (borderless_) {
			if (save_value)
				borderless_shadow_ = enabled;

			set_shadow(hWnd, enabled);
		}
	}

	static bool maximized(HWND hwnd) {
		WINDOWPLACEMENT placement = { 0 };
		if (GetWindowPlacement(hwnd, &placement))
			return placement.showCmd == SW_MAXIMIZE;
		else
			return false;
	}

	/// Adjust client rect to not spill over monitor edges when maximized.
	/// rect(in/out): in: proposed window rect, out: calculated client rect
	/// Does nothing if the window is not maximized.
	static void adjust_maximized_client_rect(HWND window, RECT& rect) {
		if (!maximized(window))
			return;

		auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);

		if (!monitor)
			return;

		MONITORINFO monitor_info{ };
		monitor_info.cbSize = sizeof(monitor_info);

		if (!::GetMonitorInfoW(monitor, &monitor_info))
			return;

		// when maximized, make the client area fill just the monitor (without task bar) rect,
		// not the whole window rect which extends beyond the monitor.
		rect = monitor_info.rcWork;
	}

	LRESULT hit_test(const POINT& cursor) {
		// identify borders and corners to allow resizing the window.
		// Note: On Windows 10, windows behave differently and
		// allow resizing outside the visible window frame.
		// This implementation does not replicate that behavior.
		// to-do: check if this behavior can be replicated for a borderless window.
		POINT border {
			::GetSystemMetrics(SM_CXFRAME),
			::GetSystemMetrics(SM_CYFRAME)
		};

		border.x = static_cast<long>(0.5f + dpi_scale_ * border.x);
		border.y = static_cast<long>(0.5f + dpi_scale_ * border.y);

		RECT window;
		if (!::GetWindowRect(hWnd_, &window))
			return HTNOWHERE;

		enum region_mask {
			client = 0b00000,
			left = 0b00001,
			right = 0b00010,
			top = 0b00100,
			bottom = 0b01000,
			caption = 0b10000,
		};

		auto result =
			left * (cursor.x < (window.left + border.x)) |
			right * (cursor.x >= (window.right - border.x)) |
			top * (cursor.y < (window.top + border.y)) |
			bottom * (cursor.y >= (window.bottom - border.y));

		if (cursor.x >= (window.left + border.x) &&
			cursor.x < (window.right - border.x) &&
			cursor.y >= (window.top + border.y) &&
			cursor.y < (window.top + static_cast<long>(0.5f + dpi_scale_ * caption_bar_height_))) {
			POINT m_pt = { 0, 0 };
			ScreenToClient(hWnd_, &m_pt);

			D2D1_POINT_2F point{ static_cast<float>(cursor.x + m_pt.x),
				static_cast<float>(cursor.y + m_pt.y) };

			// exclude page widgets
			bool exclude = false;
			for (auto& widget : widgets_) {
				if (widget.second.contains(point)) {
					exclude = true;
					break;
				}
			}

			if (exclude)
				result = client;
			else
				result = caption;
		}

		switch (result) {
		case left: return allow_resizing_ ? HTLEFT : HTCLIENT;
		case right: return allow_resizing_ ? HTRIGHT : HTCLIENT;
		case top: return allow_resizing_ ? HTTOP : HTCLIENT;
		case bottom: return allow_resizing_ ? HTBOTTOM : HTCLIENT;
		case top | left: return allow_resizing_ ? HTTOPLEFT : HTCLIENT;
		case top | right: return allow_resizing_ ? HTTOPRIGHT : HTCLIENT;
		case bottom | left: return allow_resizing_ ? HTBOTTOMLEFT : HTCLIENT;
		case bottom | right: return allow_resizing_ ? HTBOTTOMRIGHT : HTCLIENT;
		case client: return HTCLIENT;
		case caption: return HTCAPTION;
		default: return HTNOWHERE;
		}
	}

	void client_hittest(const D2D1_POINT_2F& point) {
		bool contains = false;
		bool change = false;

		class helper {
		public:
			static void hittest_hscrollbar(const std::string& page_name,
				const std::string& current_page, liblec::lecui::containers::page& page,
				const D2D1_POINT_2F& point, const D2D1_POINT_2F& point_before,
				bool& contains, bool& change) {
				// hit test horizontal scroll bar
				if (page.d_page_.h_scrollbar().pressed()) {
					if (page_name == current_page) {
						change = true;
						contains = page.d_page_.h_scrollbar().contains(point);
						page.d_page_.h_scrollbar().hit(contains);
					}

					// capture the displacement
					page.d_page_.h_scrollbar().x_displacement_ =
						page.d_page_.h_scrollbar().x_displacement_previous_ + (point.x - point_before.x);
				}

				if (!change) {
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							for (auto& tab : tab_control.p_tabs_)
								hittest_hscrollbar(tab.first, tab_control.current_tab_, tab.second,
									point, point_before, contains, change);
						}
						else
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								for (auto& page : pane.p_panes_)
									hittest_hscrollbar(page.first, pane.current_pane_, page.second,
										point, point_before, contains, change);
							}
					}
				}
			}

			static void hittest_vscrollbar(const std::string& page_name,
				const std::string& current_page, liblec::lecui::containers::page& page,
				const D2D1_POINT_2F& point, const D2D1_POINT_2F& point_before,
				bool& contains, bool& change) {
				// hit test vertical scroll bar
				if (page.d_page_.v_scrollbar().pressed()) {
					if (page_name == current_page) {
						change = true;
						contains = page.d_page_.v_scrollbar().contains(point);
						page.d_page_.v_scrollbar().hit(contains);
					}

					// capture the displacement
					page.d_page_.v_scrollbar().y_displacement_ =
						page.d_page_.v_scrollbar().y_displacement_previous_ + (point.y - point_before.y);
				}

				if (!change) {
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							for (auto& tab : tab_control.p_tabs_)
								hittest_vscrollbar(tab.first, tab_control.current_tab_, tab.second,
									point, point_before, contains, change);
						}
						else
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								for (auto& page : pane.p_panes_)
									hittest_vscrollbar(page.first, pane.current_pane_, page.second,
										point, point_before, contains, change);
							}
					}
				}
			}

			static void hittest_widgets(liblec::lecui::containers::page& page,
				const D2D1_POINT_2F& point, bool& contains, bool& change) {
				// hit test widgets
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (change)
						break;

					contains = widget.second.contains(point);
					if (change = widget.second.hit(contains))
						break;

					if (widget.second.type() ==
						liblec::lecui::widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

						if (page_iterator != tab_control.p_tabs_.end())
							helper::hittest_widgets(page_iterator->second, point, contains, change);
					}
					else
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							auto page_iterator = pane.p_panes_.find(pane.current_pane_);

							if (page_iterator != pane.p_panes_.end())
								helper::hittest_widgets(page_iterator->second, point, contains, change);
						}
				}
			}
		};

		if (!change) {
			for (auto& page : p_pages_)
				helper::hittest_hscrollbar(page.first, current_page_, page.second,
					point, point_before_, contains, change);
		}

		if (!change)
			for (auto& page : p_pages_)
				helper::hittest_vscrollbar(page.first, current_page_, page.second,
					point, point_before_, contains, change);

		if (!change) {
			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::hittest_widgets(page_iterator->second, point, contains, change);
		}

		if (!change) {
			// hit test form widgets
			for (auto& widget : widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				contains = widget.second.contains(point);
				if (change = widget.second.hit(contains))
					break;
			}
		}

		if (change)
			update();
	}

	void on_lbuttondown(const D2D1_POINT_2F& point) {
		point_before_ = point;

		bool pressed = false;
		bool selected = false;
		bool update_anyway = false;

		// check form widgets
		for (auto& widget : widgets_) {
			if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
				continue;

			if (widget.second.selected())
				update_anyway = true;

			if (!pressed) {
				// pressed widget not yet found
				pressed = widget.second.contains(point);
				widget.second.press(pressed);
				widget.second.select(pressed);
			}
			else {
				// pressed widget found
				widget.second.press(false);
				widget.second.select(false);
			}
		}

		class helper {
		public:
			static void check_widgets(liblec::lecui::containers::page& page,
				const D2D1_POINT_2F& point, const float& dpi_scale, bool& pressed,
				bool& update_anyway) {
				// check widgets
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (widget.second.selected())
						update_anyway = true;

					if (!pressed) {
						// pressed widget not yet found
						pressed = widget.second.contains(point);
						widget.second.press(pressed);

						if (widget.second.type() !=
							liblec::lecui::widgets_implementation::widget_type::tab_control)
							widget.second.select(pressed);
						else
							if (widget.second.type() !=
								liblec::lecui::widgets_implementation::widget_type::pane)
								widget.second.select(pressed);
					}
					else {
						// pressed widget found
						widget.second.press(false);
						widget.second.select(false);
					}
				}

				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.type() ==
						liblec::lecui::widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

						if (page_iterator != tab_control.p_tabs_.end())
							helper::check_widgets(page_iterator->second, point, dpi_scale, pressed,
								update_anyway);
					}
					else
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							auto page_iterator = pane.p_panes_.find(pane.current_pane_);

							if (page_iterator != pane.p_panes_.end())
								helper::check_widgets(page_iterator->second, point, dpi_scale, pressed,
									update_anyway);
						}
				}

				// check scroll bars
				if (page.d_page_.h_scrollbar().pressed()) {
					page.d_page_.h_scrollbar().max_displacement(
						page.d_page_.h_scrollbar().max_displacement_left_,
						page.d_page_.h_scrollbar().max_displacement_right_);
					page.d_page_.h_scrollbar().max_displacement_left_ *= dpi_scale;
					page.d_page_.h_scrollbar().max_displacement_right_ *= dpi_scale;

					// translate the environment
					page.d_page_.h_scrollbar().x_displacement_previous_ =
						page.d_page_.h_scrollbar().x_displacement_;
					page.d_page_.h_scrollbar().max_displacement_left_ +=
						page.d_page_.h_scrollbar().x_displacement_previous_;
					page.d_page_.h_scrollbar().max_displacement_right_ +=
						page.d_page_.h_scrollbar().x_displacement_previous_;
				}
				else
					if (page.d_page_.v_scrollbar().pressed()) {
						page.d_page_.v_scrollbar().max_displacement(
							page.d_page_.v_scrollbar().max_displacement_top_,
							page.d_page_.v_scrollbar().max_displacement_bottom_);
						page.d_page_.v_scrollbar().max_displacement_top_ *= dpi_scale;
						page.d_page_.v_scrollbar().max_displacement_bottom_ *= dpi_scale;

						// translate the environment
						page.d_page_.v_scrollbar().y_displacement_previous_ =
							page.d_page_.v_scrollbar().y_displacement_;
						page.d_page_.v_scrollbar().max_displacement_top_ +=
							page.d_page_.v_scrollbar().y_displacement_previous_;
						page.d_page_.v_scrollbar().max_displacement_bottom_ +=
							page.d_page_.v_scrollbar().y_displacement_previous_;
					}
			}
		};

		auto page_iterator = p_pages_.find(current_page_);

		if (page_iterator != p_pages_.end())
			helper::check_widgets(page_iterator->second, point, dpi_scale_, pressed, update_anyway);

		if (pressed || update_anyway)
			update();
	}

	void on_lbuttonup(const D2D1_POINT_2F& point) {
		bool clicked = false;
		bool update_anyway = false;
		std::function<void()> on_click_handler = nullptr;

		// check form widgets
		for (auto& widget : widgets_) {
			if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
				continue;

			if (!clicked &&
				widget.second.pressed() &&
				widget.second.contains(point)) {
				clicked = true;
				on_click_handler = [&]() { widget.second.on_click(); };
			}

			if (widget.second.pressed())
				update_anyway = true;

			// reset pressed status
			widget.second.press(false);
		}

		class helper {
		public:
			static void check_widgets(liblec::lecui::containers::page& page,
				const D2D1_POINT_2F& point, bool& clicked, bool& update_anyway,
				std::function<void()>& on_click_handler) {
				// check widgets
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (!clicked &&
						widget.second.pressed() &&
						widget.second.contains(point)) {
						clicked = true;
						on_click_handler = [&]() { widget.second.on_click(); };
					}

					if (widget.second.pressed())
						update_anyway = true;

					// reset pressed status
					widget.second.press(false);

					if (widget.second.type() ==
						liblec::lecui::widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

						if (page_iterator != tab_control.p_tabs_.end())
							check_widgets(page_iterator->second, point, clicked, update_anyway,
								on_click_handler);
					}
					else
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							auto page_iterator = pane.p_panes_.find(pane.current_pane_);

							if (page_iterator != pane.p_panes_.end())
								check_widgets(page_iterator->second, point, clicked, update_anyway,
									on_click_handler);
						}
				}
			}
		};

		auto page_iterator = p_pages_.find(current_page_);

		if (page_iterator != p_pages_.end())
			helper::check_widgets(page_iterator->second, point, clicked, update_anyway,
				on_click_handler);

		if (clicked || update_anyway) {
			update();

			// invoke the on_click handler
			if (clicked && on_click_handler)
				on_click_handler();
		}
	}

	bool destroy_menus() {
		bool update = false;

		// check form widgets
		for (auto& widget : widgets_) {
			if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
				continue;

			if (widget.second.menu_visible()) {
				update = true;
				widget.second.reset_menu();
			}
		}

		class helper {
		public:
			static void check_widgets(liblec::lecui::containers::page& page,
				bool& update) {
				// check widgets
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (widget.second.menu_visible()) {
						update = true;
						widget.second.reset_menu();
					}
				}

				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.type() ==
						liblec::lecui::widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

						if (page_iterator != tab_control.p_tabs_.end())
							helper::check_widgets(page_iterator->second, update);
					}
					else
						if (widget.second.type() ==
							liblec::lecui::widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							auto page_iterator = pane.p_panes_.find(pane.current_pane_);

							if (page_iterator != pane.p_panes_.end())
								helper::check_widgets(page_iterator->second, update);
						}
				}
			}
		};

		auto page_iterator = p_pages_.find(current_page_);

		if (page_iterator != p_pages_.end())
			helper::check_widgets(page_iterator->second, update);

		return update;
	}

	static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto get_form = [&]() {
			liblec::lecui::form* p_form = nullptr;
			if (msg == WM_CREATE) {
				CREATESTRUCT* p_create = reinterpret_cast<CREATESTRUCT*>(lParam);
				p_form = reinterpret_cast<liblec::lecui::form*>(p_create->lpCreateParams);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)p_form);
			}
			else {
				LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
				p_form = reinterpret_cast<liblec::lecui::form*>(ptr);
			}

			return (p_form == nullptr) ? std::nullopt :
				std::optional<std::reference_wrapper<liblec::lecui::form>>{ *p_form };
		};

		auto form_ = get_form();

		if (!form_.has_value())
			return DefWindowProc(hWnd, msg, wParam, lParam);

		switch (msg) {
		case WM_CREATE:
			form_.value().get().d_.hWnd_ = hWnd;

			if (form_.value().get().d_.p_maximize_button_)
				form_.value().get().d_.p_maximize_button_->set_hwnd(hWnd);

			if (form_.value().get().d_.p_minimize_button_)
				form_.value().get().d_.p_minimize_button_->set_hwnd(hWnd);

			form_.value().get().d_.set_borderless(hWnd, form_.value().get().d_.borderless_);
			form_.value().get().d_.set_borderless_shadow(hWnd,
				form_.value().get().d_.borderless_shadow_);

			if (!form_.value().get().d_.allow_resizing_)
				SetWindowLong(hWnd, GWL_STYLE,
					GetWindowLong(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

			for (auto& it : form_.value().get().d_.timers_)
				if (!it.second.running)
					form_.value().get().d_.start_timer(it.first);

			return TRUE;

		case WM_LBUTTONDOWN:
			// capture the mouse
			SetCapture(hWnd);
			form_.value().get().d_.on_lbuttondown(form_.value().get().d_.get_cursor_position());
			return NULL;

		case WM_LBUTTONUP:
			// release the mouse capture
			ReleaseCapture();
			form_.value().get().d_.on_lbuttonup(form_.value().get().d_.get_cursor_position());
			return NULL;

		case WM_SIZING:
			form_.value().get().d_.user_sizing_ = true;
			break;

		case WM_EXITSIZEMOVE:
			form_.value().get().d_.user_sizing_ = false;
			break;

		case WM_WINDOWPOSCHANGING: {
			WINDOWPOS* p_pos = (WINDOWPOS*)lParam;
			RECT rc;
			GetWindowRect(hWnd, &rc);

			const float change_in_width =
				static_cast<float>(p_pos->cx - (rc.right - rc.left));
			const float change_in_height =
				static_cast<float>(p_pos->cy - (rc.bottom - rc.top));

			bool user_resizing = form_.value().get().d_.user_sizing_;
			bool system_resizing = false;

			if (!user_resizing) {
				// check if system is resizing the window
				if (((p_pos->cx != 0) && (change_in_width != 0.f)) ||
					((p_pos->cy != 0) && (change_in_height != 0.f)))
					system_resizing = true;
			}

			if (user_resizing || system_resizing) {
				class helper {
				public:
					static void check_page(liblec::lecui::containers::page& page,
						float change_in_width, float change_in_height, bool system_resizing) {
						// check if a horizontal scroll bar exists
						if (page.d_page_.h_scrollbar().x_displacement_ > 0.f) {
							// check if width is increasing
							if (change_in_width > 0.f) {
								if (system_resizing)
									log("system: width increasing");

								// impose a limit on change_in_width
								float x_displacement =
									page.d_page_.h_scrollbar().x_displacement_ - change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									page.d_page_.h_scrollbar().x_displacement_ - x_displacement;

								// translate the environment
								page.d_page_.h_scrollbar().x_displacement_ -= change;
								page.d_page_.h_scrollbar().max_displacement_left_ -= change;
								page.d_page_.h_scrollbar().max_displacement_right_ -= change;
							}
							else {
								if (system_resizing)
									log("system: width decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_width
								float x_displacement =
									page.d_page_.h_scrollbar().x_displacement_ + change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									x_displacement - page.d_page_.h_scrollbar().x_displacement_;

								// translate the environment
								page.d_page_.h_scrollbar().x_displacement_ += change;
								page.d_page_.h_scrollbar().max_displacement_left_ += change;
								page.d_page_.h_scrollbar().max_displacement_right_ += change;
							}

							// force translate in on_render()
							page.d_page_.h_scrollbar().force_translate_ = true;
						}

						// check if a vertical scroll bar exists
						if (page.d_page_.v_scrollbar().y_displacement_ > 0.f) {
							// check if height is increasing
							if (change_in_height > 0.f) {
								if (system_resizing)
									log("system: height increasing");

								// impose a limit on change_in_height
								float y_displacement =
									page.d_page_.v_scrollbar().y_displacement_ - change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									page.d_page_.v_scrollbar().y_displacement_ - y_displacement;

								// translate the environment
								page.d_page_.v_scrollbar().y_displacement_ -= change;
								page.d_page_.v_scrollbar().max_displacement_top_ -= change;
								page.d_page_.v_scrollbar().max_displacement_bottom_ -= change;
							}
							else {
								if (system_resizing)
									log("system: height decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_height
								float y_displacement =
									page.d_page_.v_scrollbar().y_displacement_ + change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									y_displacement - page.d_page_.v_scrollbar().y_displacement_;

								// translate the environment
								page.d_page_.v_scrollbar().y_displacement_ += change;
								page.d_page_.v_scrollbar().max_displacement_top_ += change;
								page.d_page_.v_scrollbar().max_displacement_bottom_ += change;
							}

							// force translate in on_render()
							page.d_page_.v_scrollbar().force_translate_ = true;
						}

						// to-do: check actual change in width and height of tab/pane instead of inheriting from page

						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() == liblec::lecui::widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								for (auto& tab : tab_control.p_tabs_)
									helper::check_page(tab.second, change_in_width, change_in_height, system_resizing);	// recursion
							}
							else
								if (widget.second.type() == liblec::lecui::widgets_implementation::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									for (auto& page : pane.p_panes_)
										helper::check_page(page.second, change_in_width, change_in_height, system_resizing);	// recursion
								}
						}
					}
				};

				for (auto& page : form_.value().get().d_.p_pages_)
					helper::check_page(page.second, change_in_width, change_in_height, system_resizing);
			}
		}
		break;

		case WM_SIZE:
			form_.value().get().d_.destroy_menus();
			form_.value().get().d_.on_resize(LOWORD(lParam), HIWORD(lParam));
			return NULL;

		case WM_MOUSEMOVE:
			form_.value().get().d_.mouse_track_.on_mouse_move(hWnd);
			form_.value().get().d_.client_hittest(form_.value().get().d_.get_cursor_position());
			return NULL;

		case WM_MOUSELEAVE:
			// form_.value().get().d_.get_cursor_position() doesn't work here
			form_.value().get().d_.client_hittest({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
			form_.value().get().d_.mouse_track_.reset(hWnd);
			return NULL;

		case WM_MOUSEHOVER:
			form_.value().get().d_.mouse_track_.reset(hWnd);
			return NULL;

		case WM_DISPLAYCHANGE:
			form_.value().get().d_.update();
			return NULL;

		case WM_SETFOCUS:
			return NULL;

		case WM_KILLFOCUS:
			return NULL;

		case WM_PAINT:
			form_.value().get().d_.on_render();
			ValidateRect(hWnd, nullptr);
			return NULL;

		case WM_CLOSE:
			try {
				if (!form_.value().get().d_.widgets_.at("close_button").enabled())
					return NULL;
			}
			catch (const std::exception&) {}
			form_.value().get().on_close();
			return NULL;

		case WM_DESTROY:
			PostQuitMessage(0);
			return NULL;

		case WM_NCCALCSIZE:
			if (wParam == TRUE && form_.value().get().d_.borderless_) {
				auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				adjust_maximized_client_rect(hWnd, params.rgrc[0]);

				if (maximized(hWnd)) {
					form_.value().get().d_.shadow_setting_before_maximize_ =
						form_.value().get().d_.borderless_shadow_;
					form_.value().get().d_.set_borderless_shadow(hWnd, false, false);
				}
				else
					form_.value().get().d_.set_borderless_shadow(hWnd,
						form_.value().get().d_.shadow_setting_before_maximize_, false);

				return NULL;
			}
			break;

		case WM_NCHITTEST:
			if (form_.value().get().d_.borderless_)
				return form_.value().get().d_.hit_test(
					POINT{ (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) });
			break;

		case WM_NCACTIVATE:
			if (!composition_enabled()) {
				// Prevents window frame reappearing on window activation
				// in "basic" theme, where no aero shadow is present.
				return TRUE;
			}
			break;

		case WM_NCLBUTTONDOWN:
			if (form_.value().get().d_.destroy_menus())
				form_.value().get().d_.update();
			break;

		case WM_GETMINMAXINFO: {
			// set lower limits to window size
			MINMAXINFO* p_minmaxinfo = (MINMAXINFO*)lParam;
			p_minmaxinfo->ptMinTrackSize.x = static_cast<LONG>(0.5f +
				form_.value().get().d_.min_size_.width * form_.value().get().d_.dpi_scale_);
			p_minmaxinfo->ptMinTrackSize.y = static_cast<LONG>(0.5f +
				form_.value().get().d_.min_size_.height * form_.value().get().d_.dpi_scale_);
		}
		break;

		case WM_TIMER: {
			int unique_id = (int)wParam;

			for (auto& it : form_.value().get().d_.timers_)
				if (it.second.unique_id == unique_id)
				{
					if (it.second.on_timer)
						it.second.on_timer();

					break;
				}
		}
		break;

		case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS;	// for VK_UP, VK_DOWN to be received in WM_KEYDOWN

		case WM_KEYDOWN: {
			switch (wParam) {
			case VK_SHIFT:
				if (form_.value().get().d_.shift_pressed_)
					break;
				form_.value().get().d_.shift_pressed_ = true;
				log("shift pressed");
				form_.value().get().d_.reverse_tab_navigation_ = true;
				break;

			case VK_SPACE: {
				if (form_.value().get().d_.space_pressed_)
					break;
				form_.value().get().d_.space_pressed_ = true;
				log("space pressed");

				// check form widgets
				for (auto& widget : form_.value().get().d_.widgets_) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (widget.second.selected() &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::close_button &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::maximize_button &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::minimize_button)
						widget.second.press(true);
					else
						widget.second.press(false);
				}

				class helper {
				public:
					static void check_widgets(liblec::lecui::containers::page& page) {
						// check widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							if (widget.second.selected())
								widget.second.press(true);
							else
								widget.second.press(false);

							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second);
							}
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second);
								}
						}
					}
				};

				auto page_iterator =
					form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

				if (page_iterator != form_.value().get().d_.p_pages_.end())
					helper::check_widgets(page_iterator->second);

				log("update");
				form_.value().get().d_.update();
			}
			break;

			default:
				break;
			}

			bool update = false;
			std::function<void()> on_click_handler = nullptr;

			// check form widgets
			for (auto& widget : form_.value().get().d_.widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.selected() &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::close_button &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::maximize_button &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::minimize_button)
					if (widget.second.on_keydown(wParam)) {
						update = true;
						on_click_handler = [&]() { widget.second.on_click(); };
					}
			}

			class helper {
			public:
				static void check_widgets(liblec::lecui::containers::page& page,
					WPARAM wParam, bool& update, std::function<void()>& on_click_handler) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.selected())
							if (widget.second.on_keydown(wParam)) {
								update = true;
								on_click_handler = [&]() { widget.second.on_click(); };
							}
						else
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
							}
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
								}
					}
				}
			};

			auto page_iterator =
				form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

			if (page_iterator != form_.value().get().d_.p_pages_.end())
				helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);

			if (update) {
				form_.value().get().d_.update();

				if (on_click_handler)
					on_click_handler();
			}
		}
		break;

		case WM_KEYUP:
		{
			switch (wParam) {
			case VK_SPACE: {
				log("space released");

				std::function<void()> on_space = nullptr;

				// check form widgets
				for (auto& widget : form_.value().get().d_.widgets_) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (widget.second.selected() &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::close_button &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::maximize_button &&
						widget.second.type() !=
						liblec::lecui::widgets_implementation::widget_type::minimize_button)
						on_space = [&]() { widget.second.on_click(); };

					// reset pressed status
					widget.second.press(false);
				}

				class helper {
				public:
					static void check_widgets(liblec::lecui::containers::page& page,
						std::function<void()>& on_space) {
						// check widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							if (widget.second.selected())
								on_space = [&]() { widget.second.on_click(); };
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::tab_control) {
									// get this tab control
									auto& tab_control = page.d_page_.get_tab_control(widget.first);

									auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

									if (page_iterator != tab_control.p_tabs_.end())
										helper::check_widgets(page_iterator->second, on_space);
								}
								else
									if (widget.second.type() ==
										liblec::lecui::widgets_implementation::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										auto page_iterator = pane.p_panes_.find(pane.current_pane_);

										if (page_iterator != pane.p_panes_.end())
											helper::check_widgets(page_iterator->second, on_space);
									}

							// reset pressed status
							widget.second.press(false);
						}
					}
				};

				auto page_iterator =
					form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

				if (page_iterator != form_.value().get().d_.p_pages_.end())
					helper::check_widgets(page_iterator->second, on_space);

				if (on_space) {
					form_.value().get().d_.update();
					on_space();
				}

				form_.value().get().d_.space_pressed_ = false;
			}
			break;

			case VK_SHIFT:
				form_.value().get().d_.reverse_tab_navigation_ = false;
				form_.value().get().d_.shift_pressed_ = false;
				break;

			case VK_TAB: {
				log("tab pressed");

				class helper {
				public:
					static void check_widgets(liblec::lecui::containers::page& page,
						const bool& reverse_tab_navigation,
						bool& select_next, bool& selected) {
						// check widgets
						auto widgets = page.d_page_.widgets_order();
						if (reverse_tab_navigation) std::reverse(widgets.begin(), widgets.end());

						for (const auto& name : widgets) {
							try {
								auto& widget = page.d_page_.widgets().at(name);

								if (widget.is_static() || !widget.visible() || !widget.enabled())
									continue;

								if (widget.type() ==
									liblec::lecui::widgets_implementation::widget_type::tab_control) {
									// get this tab control
									auto& tab_control = page.d_page_.get_tab_control(name);

									auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

									if (page_iterator != tab_control.p_tabs_.end())
										helper::check_widgets(page_iterator->second, reverse_tab_navigation,
											select_next, selected);
								}
								else
									if (widget.type() ==
										liblec::lecui::widgets_implementation::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(name);

										auto page_iterator = pane.p_panes_.find(pane.current_pane_);

										if (page_iterator != pane.p_panes_.end())
											helper::check_widgets(page_iterator->second, reverse_tab_navigation,
												select_next, selected);
									}
									else {
										if (widget.selected()) {
											widget.select(false);
											select_next = true;
										}
										else
											if (select_next && !selected) {
												widget.select(true);
												selected = true;
											}
									}
							}
							catch (const std::exception& e) { log(e.what()); }
						}
					}
				};

				bool select_next = false;
				bool selected = false;

				// loop twice because the currently selected widget may be the last to be checked
				for (size_t i = 0; i < 2; i++) {
					// check form widgets
					auto widgets = form_.value().get().d_.widgets_order_;
					if (form_.value().get().d_.reverse_tab_navigation_)
						std::reverse(widgets.begin(), widgets.end());

					for (auto& name : widgets) {
						try {
							auto& widget = form_.value().get().d_.widgets_.at(name);

							if (widget.is_static() || !widget.visible() || !widget.enabled())
								continue;

							if (widget.type() !=
								liblec::lecui::widgets_implementation::widget_type::close_button &&
								widget.type() !=
								liblec::lecui::widgets_implementation::widget_type::maximize_button &&
								widget.type() !=
								liblec::lecui::widgets_implementation::widget_type::minimize_button) {
								if (widget.selected()) {
									widget.select(false);
									select_next = true;
								}
								else
									if (select_next && !selected) {
										widget.select(true);
										selected = true;
									}
							}
						}
						catch (const std::exception& e) { log(e.what()); }
					}

					auto page_iterator =
						form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

					if (page_iterator != form_.value().get().d_.p_pages_.end())
						helper::check_widgets(page_iterator->second,
							form_.value().get().d_.reverse_tab_navigation_, select_next, selected);

					if (select_next)
						form_.value().get().d_.update();

					if (selected)
						break;

					select_next = true;	// no widget was selected
				}
			}
			break;

			default:
				break;
			}
		}
		break;

		case WM_MOUSEHWHEEL: {
			log("horizontal");
		}
		break;

		case WM_MOUSEWHEEL: {
			const long delta = GET_WHEEL_DELTA_WPARAM(wParam);
			const float units = static_cast<float>(delta) / 120.f;
			bool update = false;

			log("units: " + std::to_string(units));

			// check form widgets
			for (auto& widget : form_.value().get().d_.widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.hit() &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::close_button &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::maximize_button &&
					widget.second.type() !=
					liblec::lecui::widgets_implementation::widget_type::minimize_button)
					if (widget.second.on_mousewheel(units))
						update = true;
			}

			class helper {
			public:
				static void check_widgets(liblec::lecui::containers::page& page,
					float units, bool& update) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.hit()) {
							if (widget.second.on_mousewheel(units))
								update = true;
						}
						else
							if (widget.second.type() ==
								liblec::lecui::widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, units, update);
							}
							else
								if (widget.second.type() ==
									liblec::lecui::widgets_implementation::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second, units, update);
								}
					}
				}
			};

			auto page_iterator =
				form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

			if (page_iterator != form_.value().get().d_.p_pages_.end())
				helper::check_widgets(page_iterator->second, units, update);

			if (update)
				form_.value().get().d_.update();
		}
		break;

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	int make_unique_id() {
		++unique_id_;
		return unique_id_;
	}

	void start_timer(const std::string& name) {
		if (timers_.find(name) != timers_.end()) {
			log("starting timer: " + name);
			SetTimer(hWnd_, (UINT_PTR)timers_.at(name).unique_id,
				(UINT)timers_.at(name).milliseconds, NULL);
			timers_.at(name).running = true;
		}
	}

	void parse_widget_path(const std::string& name,
		std::vector<std::string>& path,
		std::string& widget_name) {
		if (name.empty())
			return;

		// extract page and widget names
		std::string page_name;
		std::string widget_name_;

		auto idx = name.find("/");

		if (idx != std::string::npos) {
			page_name = name.substr(0, idx);
			widget_name_ = name.substr(idx + 1);
		}
		else
			widget_name_ = name;

		if (page_name.empty()) {
			widget_name = widget_name_;
			return;
		}
		else
		{
			path.push_back(page_name);
			if (widget_name_.find("/") != std::string::npos)
				parse_widget_path(widget_name_, path, widget_name);
			else
				widget_name = widget_name_;
		}
	}

	// throws on failure!
	liblec::lecui::widgets_implementation::widget& find_widget(
		const liblec::lecui::containers::page& page,
		const std::vector<std::string>& path,
		const std::string& widget_name) {
		if (path.size() == 0)
			return page.d_page_.widgets().at(widget_name);
		else
			if (path.size() == 1) {
				// assume this is a pane
				const auto& pane_name = path[0];
				auto& pane = page.d_page_.get_pane(pane_name);

				auto& page = pane.p_panes_.at("pane");

				std::vector<std::string> path_(path.size() - 1);
				for (size_t i = 1; i < path.size(); i++)
					path_[i - 1] = path[i];

				return find_widget(page, path_, widget_name);
			}
			else {
				// assume first level is a tab control
				const auto& tab_control_name = path[0];
				auto& tab_control = page.d_page_.get_tab_control(tab_control_name);

				// assume second level is a tab
				const auto& tab_name = path[1];
				auto& tab = tab_control.p_tabs_.at(tab_name);

				std::vector<std::string> path_(path.size() - 2);
				for (size_t i = 2; i < path.size(); i++)
					path_[i - 2] = path[i];

				return find_widget(tab, path_, widget_name);
			}
	}

	// throws on failure!
	liblec::lecui::widgets_implementation::widget& find_widget(
		const std::map<std::string, liblec::lecui::containers::page>& pages,
		const std::vector<std::string>& path,
		const std::string& widget_name) {
		if (path.size() == 0)
			return widgets_.at(widget_name);
		else
			if (path.size() == 1) {
				const auto page_name = path[0];
				return pages.at(page_name).d_page_.widgets().at(widget_name);
			}
			else {
				// assume first level is a page
				const auto page_name = path[0];

				auto& page = pages.at(page_name);

				std::vector<std::string> path_(path.size() - 1);
				for (size_t i = 1; i < path.size(); i++)
					path_[i - 1] = path[i];

				return find_widget(page, path_, widget_name);
			}
	}

	void enable(const std::string& name, bool enable) {
		try {
			std::vector<std::string> path;
			std::string widget_name_;
			parse_widget_path(name, path, widget_name_);

			auto& widget = find_widget(p_pages_, path, widget_name_);
			widget.enable(enable);
			update();
		}
		catch (const std::exception&) {}

		bool allow_resizing_before_ = allow_resizing_;

		try { allow_resizing_ = widgets_.at("maximize_button").enabled(); }
		catch (const std::exception&) { allow_resizing_ = false; }

		if (allow_resizing_before_ != allow_resizing_) {
			if (allow_resizing_)
				SetWindowLong(hWnd_, GWL_STYLE,
					GetWindowLong(hWnd_, GWL_STYLE) | (WS_SIZEBOX | WS_MAXIMIZEBOX));
			else
				SetWindowLong(hWnd_, GWL_STYLE,
					GetWindowLong(hWnd_, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
		}

		try { allow_minimize_ = widgets_.at("minimize_button").enabled(); }
		catch (const std::exception&) { allow_minimize_ = false; }
	}

	void show(const std::string& name, bool show) {
		try {
			std::vector<std::string> path;
			std::string widget_name;
			parse_widget_path(name, path, widget_name);
			auto& widget = find_widget(p_pages_, path, widget_name);
			widget.show(show);
			update();
		}
		catch (const std::exception&) {}
	}

private:
	// static members
	static std::atomic<unsigned long> instances_;
	static std::atomic<bool> initialized_;
	static ID2D1Factory* p_direct2d_factory_;
	static IDWriteFactory* p_directwrite_factory_;

	form* p_parent_;
	bool parent_closing_;
	std::map<form*, form*> m_children_;
	bool show_called_;

	// constant members
	const float caption_bar_height_;
	const float form_border_thickness_;
	const float page_tolerance_;
	const float control_button_margin_;

	// name of dll containing resources like PNGs etc
	std::string resource_dll_filename_;
	HMODULE resource_module_handle_;

	// icons for use by the Windows OS
	int idi_icon_, idi_icon_small_;

	// colors
	liblec::lecui::color clr_background_, clr_titlebar_background_, clr_theme_, clr_theme_hot_,
		clr_theme_disabled_;

	bool top_most_;
	HWND hWnd_, hWnd_parent_;

	// caption (also the name of the home page)
	std::string caption_;

	bool activate_;

	// window coordinates
	liblec::lecui::point point_;
	liblec::lecui::size size_, min_size_;
	bool allow_resizing_, allow_minimize_;

	bool user_pos_;
	bool preset_pos_;
	liblec::lecui::form_position form_position_;

	float dpi_scale_;

	bool borderless_;			// should the window be borderless
	bool borderless_shadow_;	// should the window display a native aero shadow while borderless
	bool shadow_setting_before_maximize_;

	// Direct2D resources
	ID2D1HwndRenderTarget* p_render_target_;
	ID2D1SolidColorBrush* p_brush_theme_;
	ID2D1SolidColorBrush* p_brush_theme_hot_;
	ID2D1SolidColorBrush* p_brush_theme_disabled_;
	ID2D1SolidColorBrush* p_brush_titlebar_;

	// pages <K = page name, T>
	std::map<std::string, liblec::lecui::containers::page> p_pages_;
	std::string current_page_;

	mouse_track mouse_track_;

	// form widgets <K = widget name, T>
	std::map<std::string, liblec::lecui::widgets_implementation::widget&> widgets_;
	std::vector<std::string> widgets_order_;
	std::unique_ptr<liblec::lecui::widgets_implementation::close_button> p_close_button_;
	std::unique_ptr<liblec::lecui::widgets_implementation::maximize_button> p_maximize_button_;
	std::unique_ptr<liblec::lecui::widgets_implementation::minimize_button> p_minimize_button_;
	std::unique_ptr<liblec::lecui::widgets_implementation::label> p_caption_;

	D2D1_POINT_2F point_before_;
	bool user_sizing_;

	struct timer {
		int unique_id = -1;
		bool running = false;
		unsigned long milliseconds = 1000;
		std::function<void()> on_timer = nullptr;
	};

	// timer map <K = name, T>
	std::map<std::string, timer> timers_;
	std::atomic<int> unique_id_;

	bool reverse_tab_navigation_;
	bool shift_pressed_;
	bool space_pressed_;
	bool new_page_added_;

	friend form;
	friend liblec::lecui::dimensions;
	friend liblec::lecui::appearance;
	friend liblec::lecui::controls;
	friend liblec::lecui::widgets::timer;
	friend liblec::lecui::page;
}; // form_impl

// initialize static variables
std::atomic<unsigned long> liblec::lecui::form::form_impl::instances_ = 0;
std::atomic<bool> liblec::lecui::form::form_impl::initialized_ = false;
ID2D1Factory* liblec::lecui::form::form_impl::p_direct2d_factory_ = nullptr;
IDWriteFactory* liblec::lecui::form::form_impl::p_directwrite_factory_ = nullptr;

// this is the constructor that all the others below call
liblec::lecui::form::form(const std::string& caption) :
	d_(*new form_impl(caption)) {
	log("entering form constructor");
}

liblec::lecui::form::form() :
	liblec::lecui::form::form("liblec::lecui::form") {}

liblec::lecui::form::form(const std::string& caption, form& parent) :
	liblec::lecui::form::form(caption) {
	// capture parent
	d_.p_parent_ = &parent;
	d_.hWnd_parent_ = parent.d_.hWnd_;

	// this is a child window. add it to the parent's map of children.
	parent.d_.m_children_.insert(std::make_pair(this, this));
}

liblec::lecui::form::~form() {
	if (d_.hWnd_parent_ && d_.p_parent_) {
		// this is a child window. remove it from the parent's map of children
		try { d_.p_parent_->d_.m_children_.erase(this); }
		catch (const std::exception&) {}
	}

	delete& d_;
	log("exiting form destructor");
}

void liblec::lecui::form::move(const liblec::lecui::point& point) {
	d_.user_pos_ = true;
	d_.preset_pos_ = false;
	d_.point_ = point;
}

void liblec::lecui::form::move(const liblec::lecui::form_position& form_position) {
	d_.user_pos_ = true;
	d_.preset_pos_ = true;
	d_.form_position_ = form_position;
}

bool liblec::lecui::form::show(std::string& error) {
	log("liblec::lecui::form::show");

	if (d_.show_called_) {
		error = "Library usage error: liblec::lecui::form::show";
		return false;
	}
	else {
		d_.show_called_ = true;

		if (!d_.initialized_) {
			error = "Library initialization error: liblec::lecui::form::show";
			return false;
		}
	}

	// get resource module handle
	if (!d_.resource_dll_filename_.empty()) {
		d_.resource_module_handle_ =
			LoadLibrary(convert_string(d_.resource_dll_filename_).c_str());

		if (!d_.resource_module_handle_) {
			// LoadLibrary failed
			error = "Loading " + d_.resource_dll_filename_ + " failed: " + get_last_error();
			return false;
		}
	}
	else
		d_.resource_module_handle_ = GetModuleHandle(nullptr);

	// create form controls

	d_.create_close_button([&]() { on_close(); });

	if (d_.allow_resizing_)
		d_.create_maximize_button();

	if (d_.allow_minimize_)
		d_.create_minimize_button();

	d_.create_form_caption([&]() { on_caption(); });

	// call the layout virtual function
	if (!layout(error))
		return false;

	// register window class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = d_.window_procedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.hInstance = GetModuleHandle(nullptr);

	// load main application icon
	if (d_.idi_icon_)
		wcex.hIcon = LoadIcon(d_.resource_module_handle_, MAKEINTRESOURCE(d_.idi_icon_));

	// load small application icon
	if (d_.idi_icon_small_)
		wcex.hIconSm = (HICON)LoadImage(d_.resource_module_handle_,
			MAKEINTRESOURCE(d_.idi_icon_small_),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR
		);

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszClassName = L"liblec::lecui::form";
	RegisterClassEx(&wcex);

	if (d_.user_pos_) {
		if (d_.preset_pos_)
			d_.set_position(d_.form_position_, d_.size_.width, d_.size_.height);
		else
			d_.set_position(d_.point_.x, d_.point_.y, d_.size_.width, d_.size_.height);
	}
	else {
		if (IsWindow(d_.hWnd_parent_) && IsWindowEnabled(d_.hWnd_parent_))
			d_.set_position(liblec::lecui::form_position::center_to_parent,
				d_.size_.width, d_.size_.height);
		else
			d_.set_position(liblec::lecui::form_position::center_to_working_area,
				d_.size_.width, d_.size_.height);
	}

	// perform initialization (d_.hWnd_ will be captured in WM_CREATE)
	if (!CreateWindowEx(d_.top_most_ == true ? WS_EX_TOPMOST : NULL, wcex.lpszClassName,
		convert_string(d_.caption_).c_str(),
		static_cast<DWORD>(liblec::lecui::form::form_impl::style::aero_borderless),
		static_cast<int>(0.5f + d_.point_.x * d_.dpi_scale_),
		static_cast<int>(0.5f + d_.point_.y * d_.dpi_scale_),
		static_cast<int>(0.5f + d_.size_.width * d_.dpi_scale_),
		static_cast<int>(0.5f + d_.size_.height * d_.dpi_scale_),
		d_.hWnd_parent_, nullptr, wcex.hInstance, this)) {
		error = get_last_error();
		return false;
	}

	ShowWindow(d_.hWnd_, SW_SHOW);
	UpdateWindow(d_.hWnd_);

	if (d_.activate_)
		SetForegroundWindow(d_.hWnd_);

	// disable parent
	if (IsWindow(d_.hWnd_parent_) && IsWindowEnabled(d_.hWnd_parent_))
		EnableWindow(d_.hWnd_parent_, FALSE);

	MSG msg = {};

	// main message loop
	while (GetMessage(&msg, nullptr, 0, 0)) {
		// IsDialogMessage() is critical for WS_TABSTOP to work in a custom window
		if (!IsDialogMessage(d_.hWnd_, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (d_.parent_closing_)
		PostQuitMessage(0);

	return true;
}

void liblec::lecui::form::close() {
	if (IsWindow(d_.hWnd_)) {
		// stop all timers
		for (auto& it : d_.timers_)
		{
			liblec::lecui::widgets::timer timer(*this);
			if (timer.running(it.first))
				timer.stop(it.first);
		}

		if (!d_.m_children_.empty()) {
			// children exist, notify them the parent is closing and close them
			for (auto& child : d_.m_children_) {
				child.second->d_.parent_closing_ = true;
				child.second->close();
			}
		}

		// enable parent
		if (IsWindow(d_.hWnd_parent_) && !IsWindowEnabled(d_.hWnd_parent_))
			EnableWindow(d_.hWnd_parent_, TRUE);

		DestroyWindow(d_.hWnd_);
	}
}

bool liblec::lecui::form::layout(std::string& error) { return true; }
void liblec::lecui::form::on_close() { close(); }
void liblec::lecui::form::on_caption() {}

bool liblec::lecui::form::prompt(const std::string& question) {
	if (MessageBoxA(IsWindow(d_.hWnd_) ? d_.hWnd_ : GetForegroundWindow(),
		question.c_str(), d_.caption_.c_str(), MB_YESNO) == IDYES)
		return true;
	else
		return false;
}

void liblec::lecui::form::message(const std::string& message) {
	if (!message.empty())
		MessageBoxA(IsWindow(d_.hWnd_) ? d_.hWnd_ : GetForegroundWindow(),
			message.c_str(), d_.caption_.c_str(), MB_OK);
}

std::string liblec::lecui::form::select_folder(const std::string& title) {
	char szDir[MAX_PATH];
	BROWSEINFOA bInfo = { 0 };
	bInfo.hwndOwner = d_.hWnd_;
	bInfo.pszDisplayName = szDir;
	bInfo.lpszTitle = title.c_str();
	bInfo.iImage = -1;

	std::string folder;
	LPITEMIDLIST lpItem = SHBrowseForFolderA(&bInfo);
	if (lpItem != NULL) {
		SHGetPathFromIDListA(lpItem, szDir);
		folder = szDir;
	}

	return folder;
}

auto make_filter_string(const std::vector<liblec::lecui::file_type>& file_types,
	const bool& include_all_supported_types,
	std::vector<std::string>& filetypes_indexed) {
	/// If we indicate a NULL termminator with a new line character,
	/// The filter string format is as follows :
	/// Bitmap Files(*.bmp)
	/// *.BMP
	/// JPEG(*.jpg; *.jpeg)
	/// *.JPG; *.JPEG
	/// PNG(*.png)
	/// *.PNG
	/// All supported images
	/// *.JPG; *.JPEG; *.PNG; *.BMP
	std::string filter;
	filetypes_indexed.clear();

	// create NULL terminator
	std::string s_null({ '\0' });

	std::vector<liblec::lecui::file_type> types;

	for (size_t i = 0; i < file_types.size(); i++) {
		// check if type description hasn't already been captured; if it has,
		// append it's extension to the other
		bool b_new = true;

		for (size_t x = 0; x < types.size(); x++) {
			if (types[x].description == file_types[i].description) {
				types[x].extension += ";*." + file_types[i].extension;

				b_new = false;
				break;
			}
		}

		if (b_new) {
			liblec::lecui::file_type type;
			type.description = file_types[i].description;
			type.extension = "*." + file_types[i].extension;
			types.push_back(type);
		}
	}

	std::string all_file_extensions;

	for (size_t i = 0; i < types.size(); i++) {
		std::string description = types[i].description;
		std::string extension = types[i].extension;

		if (all_file_extensions.empty())
			all_file_extensions = extension;
		else
			all_file_extensions += ";" + extension;

		std::string filter_;
		filter_ = description + " (" + extension + ")" + s_null;
		filter_ += extension + s_null;

		filter += filter_;
		filetypes_indexed.push_back(types[i].extension);
	}

	// add all supported files
	if (include_all_supported_types) {
		std::string filter_;
		std::string description = "All supported files";
		std::string extension = all_file_extensions;
		filter_ = description + " (" + extension + ")" + s_null;
		filter_ += extension + s_null;

		filter += filter_;
		filetypes_indexed.push_back("");
	}

	return filter;
}

// remove anything that includes and comes after the first occurence of c in the string fullpath
void rem_tail(std::string& str, char c) {
	// remove extension if present.
	const auto char_idx = str.find(c);
	if (std::string::npos != char_idx)
		str.erase(char_idx);

	return;
}

std::string liblec::lecui::form::open_file(const liblec::lecui::open_file_params& params) {
	std::vector<std::string> filetypes_indexed;
	const auto filter_string =
		make_filter_string(params.file_types, params.include_all_supported_types, filetypes_indexed);

	char _FilePath[MAX_PATH];
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = d_.hWnd_;
	ofn.lpstrFile = _FilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter_string.c_str();
	ofn.nFilterIndex = 1;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrTitle = params.title.c_str();
	ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	return (GetOpenFileNameA(&ofn) == TRUE) ? std::string(_FilePath) : std::string();
}

std::string liblec::lecui::form::save_file(const std::string& file,
	const liblec::lecui::save_file_params& params) {
	std::vector<std::string> filetypes_indexed;
	const auto filter_string =
		make_filter_string(params.file_types, params.include_all_supported_types, filetypes_indexed);

	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	char _FilePath[MAX_PATH];
	if (!file.empty())
		auto temp = lstrcpynA(_FilePath, file.c_str(), _countof(_FilePath));
	ofn.hwndOwner = d_.hWnd_;
	ofn.lpstrFile = _FilePath;
	if (file.empty())
		ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter_string.c_str();
	ofn.nFilterIndex = 1;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrTitle = params.title.c_str();
	ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
		OFN_NOTESTFILECREATE | OFN_ENABLESIZING;

	std::string fullpath;
	if (GetSaveFileNameA(&ofn) == TRUE) {
		// capture full path
		fullpath = _FilePath;

		// remove multiple file extensions
		for (size_t i = 0; i < filetypes_indexed.size(); i++)
			rem_tail(filetypes_indexed[i], ';');

		// determine selected file type
		std::string sExtension("");

		///
		/// nFilterIndex
		/// Type: DWORD
		/// The index of the currently selected filter in the File Types control. The buffer pointed to by lpstrFilter
		/// contains pairs of strings that define the filters. The first pair of strings has an index value of 1, the
		/// second pair 2, and so on. An index of zero indicates the custom filter specified by	lpstrCustomFilter.
		/// You can specify an index on input to indicate the initial filter description and filter pattern for the
		/// dialog box. When the user selects a file, nFilterIndex returns the index of the currently displayed filter.
		/// If nFilterIndex is zero and lpstrCustomFilter is NULL, the system uses the first filter in the lpstrFilter buffer.
		/// If all three members are zero or NULL, the system does not use any filters and does not show any files in
		/// the file list control of the dialog box.
		/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646839(v=vs.85).aspx
		///
		if (filetypes_indexed.size() > 0 && filetypes_indexed.size() >= ofn.nFilterIndex - 1)
			sExtension = filetypes_indexed[ofn.nFilterIndex - 1];

		// add the selected extension to the full path (if it exists)
		if (!sExtension.empty()) {
			// format extension properly
			const size_t period_idx = sExtension.rfind('.');

			if (std::basic_string<TCHAR>::npos != period_idx)
				sExtension.erase(0, period_idx);

			// add the extension if it's not there
			if (fullpath.find(sExtension) == std::basic_string<TCHAR>::npos)
				fullpath += sExtension;
			else { /* user probably selected an existing file or typed it in with an extension */ }
		}
	}

	return fullpath;
}

void liblec::lecui::form::enable(const std::string& name, bool enable) { d_.enable(name, enable); }
void liblec::lecui::form::show(const std::string& name, bool show) { d_.show(name, show); }

class liblec::lecui::widgets::timer::timer_impl {
public:
	timer_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::widgets::timer::timer(form& fm) :
	d_(*new timer_impl(fm)) {}

liblec::lecui::widgets::timer::~timer() { delete& d_; }

void liblec::lecui::widgets::timer::add(const std::string& name,
	const unsigned long& milliseconds, std::function<void()> on_timer) {
	if (running(name)) return;

	int unique_id = (d_.fm_.d_.timers_.find(name) == d_.fm_.d_.timers_.end()) ?
		d_.fm_.d_.make_unique_id() : d_.fm_.d_.timers_.at(name).unique_id;

	liblec::lecui::form::form_impl::timer timer_;
	timer_.milliseconds = milliseconds;
	timer_.on_timer = on_timer;
	timer_.unique_id = unique_id;

	// insert to timer map
	d_.fm_.d_.timers_[name] = timer_;

	if (IsWindow(d_.fm_.d_.hWnd_))
		d_.fm_.d_.start_timer(name);
	else
		d_.fm_.d_.timers_.at(name).running = false;	// timer will be started on window creation
}

bool liblec::lecui::widgets::timer::running(const std::string& name) {
	return (d_.fm_.d_.timers_.find(name) == d_.fm_.d_.timers_.end()) ?
		false : d_.fm_.d_.timers_.at(name).running;
}

void liblec::lecui::widgets::timer::stop(const std::string& name) {
	if (d_.fm_.d_.timers_.find(name) != d_.fm_.d_.timers_.end()) {
		log("stopping timer: " + name);
		KillTimer(d_.fm_.d_.hWnd_, (UINT_PTR)d_.fm_.d_.timers_.at(name).unique_id);
		d_.fm_.d_.timers_.at(name).running = false;
	}
}

class liblec::lecui::dimensions::dimensions_impl {
public:
	dimensions_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::dimensions::dimensions(form& fm) :
	d_(*(new dimensions_impl(fm))) {}

liblec::lecui::dimensions::~dimensions() { delete& d_; }

void liblec::lecui::dimensions::size(const liblec::lecui::size& size) {
	if (size.width) {
		d_.fm_.d_.size_.width = size.width;

		// force minimum width to conform (it cannot be greater)
		d_.fm_.d_.min_size_.width = smallest(d_.fm_.d_.size_.width, d_.fm_.d_.min_size_.width);
	}

	if (size.height) {
		d_.fm_.d_.size_.height = size.height;

		// force minimum height to conform (it cannot be greater)
		d_.fm_.d_.min_size_.height = smallest(d_.fm_.d_.size_.height, d_.fm_.d_.min_size_.height);
	}
}

const liblec::lecui::size& liblec::lecui::dimensions::size() {
	return d_.fm_.d_.size_;
}

void liblec::lecui::dimensions::min(const liblec::lecui::size& size) {
	if (size.width)		// do not allow minimum width to be greater than current window width
		d_.fm_.d_.min_size_.width = smallest(size.width, d_.fm_.d_.size_.width);

	if (size.height)	// do not allow minimum height to be greater than current window height
		d_.fm_.d_.min_size_.height = smallest(size.height, d_.fm_.d_.size_.height);
}

const liblec::lecui::size& liblec::lecui::dimensions::min() {
	return d_.fm_.d_.min_size_;
}

class liblec::lecui::appearance::appearance_impl {
public:
	appearance_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::appearance::appearance(form& fm) :
	d_(*(new appearance_impl(fm))) {}

liblec::lecui::appearance::~appearance() { delete& d_; }

void liblec::lecui::appearance::theme(const liblec::lecui::color& color,
	const liblec::lecui::color& color_hot,
	const liblec::lecui::color& color_disabled) {
	d_.fm_.d_.clr_theme_ = color;
	d_.fm_.d_.clr_theme_hot_ = color_hot;
	d_.fm_.d_.clr_theme_disabled_ = color_disabled;
}

void liblec::lecui::appearance::background(const liblec::lecui::color& color,
	const liblec::lecui::color& color_titlebar) {
	d_.fm_.d_.clr_background_ = color;
	d_.fm_.d_.clr_titlebar_background_ = color_titlebar;
}

void liblec::lecui::appearance::shadow(const bool& enable) {
	if (IsWindow(d_.fm_.d_.hWnd_))
		d_.fm_.d_.set_borderless_shadow(d_.fm_.d_.hWnd_, enable);
	else
		d_.fm_.d_.borderless_shadow_ = enable;
}

class liblec::lecui::controls::controls_impl {
public:
	controls_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::controls::controls(form& fm) : d_(*(new controls_impl(fm))) {}
liblec::lecui::controls::~controls() { delete& d_; }

void liblec::lecui::controls::resize(bool enable) { d_.fm_.d_.allow_resizing_ = enable; }
void liblec::lecui::controls::minimize(bool enable) { d_.fm_.d_.allow_minimize_ = enable; }

class liblec::lecui::page::page_impl {
public:
	page_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::page::page(form& fm) : d_(*(new page_impl(fm))) {}
liblec::lecui::page::~page() { delete& d_; }

bool liblec::lecui::page::exists(const std::string& name) {
	return d_.fm_.d_.p_pages_.count(name) != 0;
}

liblec::lecui::containers::page& liblec::lecui::page::add(const std::string& name) {
	log("liblec::lecui::page::add");

	if (d_.fm_.d_.p_pages_.count(name)) {
		log("library usage error liblec::lecui::page::add");

		/// to-do: find a mechanism that makes sense ...
		return d_.fm_.d_.p_pages_.at(name);
	}

	d_.fm_.d_.p_pages_.emplace(name, name);
	d_.fm_.d_.new_page_added_ = true;		// essential for this page's resources to be created

	// specify directwrite factory (used internally for text rendering)
	d_.fm_.d_.p_pages_.at(name).d_page_.directwrite_factory(d_.fm_.d_.p_directwrite_factory_);

	const long thickness = 10;
	const long margin = static_cast<long>(d_.fm_.d_.page_tolerance_ + 0.5);

	// initialize the page's horizontal scroll bar
	{
		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().resize.perc_width = 100;
		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().resize.perc_y = 100;

		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().rect.left =
			margin + thickness - static_cast<long>(d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().rect.right =
			d_.fm_.d_.size_.width - (margin + thickness) -
			static_cast<long>(d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().rect.bottom =
			d_.fm_.d_.size_.height - margin -
			static_cast<long>(d_.fm_.d_.caption_bar_height_ + d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().rect.top =
			d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

		d_.fm_.d_.p_pages_.at(name).d_page_.h_scrollbar().specs().on_click = nullptr;
	}

	// initialize the page's vertical scroll bar
	{
		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().resize.perc_height = 100;
		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().resize.perc_x = 100;

		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().rect.top =
			margin + thickness - static_cast<long>(d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().rect.bottom =
			d_.fm_.d_.size_.height - (margin + thickness) -
			static_cast<long>(d_.fm_.d_.caption_bar_height_ + d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().rect.right =
			d_.fm_.d_.size_.width - margin -
			static_cast<long>(d_.fm_.d_.page_tolerance_);
		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().rect.left =
			d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().rect.right - thickness;

		d_.fm_.d_.p_pages_.at(name).d_page_.v_scrollbar().specs().on_click = nullptr;
	}

	// add an invisible rect to bound the page. This is essential for scroll bars to work
	// appropriately when contents don't reach the page borders
	auto& rectangle = d_.fm_.d_.p_pages_.at(name).d_page_.add_rectangle("minimal_page_border_rect");
	rectangle.is_filled = false;

	// make it transparent
	rectangle.color_border = { 255, 0, 0, 0 };
	rectangle.color_hot = { 255, 0, 0, 0 };

	d_.fm_.d_.p_pages_.at(name).d_page_.size(d_.fm_.d_.size_);

	d_.fm_.d_.p_pages_.at(name).d_page_.width(d_.fm_.d_.p_pages_.at(name).d_page_.width() -
		static_cast<long>(2.f * d_.fm_.d_.page_tolerance_));
	d_.fm_.d_.p_pages_.at(name).d_page_.height(d_.fm_.d_.p_pages_.at(name).d_page_.height() -
		static_cast<long>(2.f * d_.fm_.d_.page_tolerance_ + d_.fm_.d_.caption_bar_height_));

	rectangle.rect.set(0, 0, d_.fm_.d_.p_pages_.at(name).d_page_.width(),
		d_.fm_.d_.p_pages_.at(name).d_page_.height());

	rectangle.corner_radius_x = 15.f;
	rectangle.corner_radius_y = 15.f;

	rectangle.resize.perc_width = 100;
	rectangle.resize.perc_height = 100;

	// return reference to page so caller can add widgets to it
	return d_.fm_.d_.p_pages_.at(name);
}

void liblec::lecui::page::show(const std::string& name) {
	log("liblec::lecui::page::show");

	d_.fm_.d_.current_page_ = name;

	if (IsWindow(d_.fm_.d_.hWnd_))
		d_.fm_.d_.update();
}
