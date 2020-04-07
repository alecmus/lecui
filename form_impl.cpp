/*
** form_impl.cpp - form_impl implementation
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

#include "form_impl.h"

#include "containers/page.h"
#include "containers/tab_control.h"
#include "containers/pane.h"

#include "widgets/rectangle.h"
#include "widgets/label.h"
#include "widgets/timer.h"
#include "widgets/group.h"
#include "widgets/button.h"
#include "widgets/h_scrollbar/h_scrollbar.h"
#include "widgets/v_scrollbar/v_scrollbar.h"

// Windows headers
#include <ShlObj.h>		// for SHGetFolderPath
#include <dwmapi.h>		// for DwmExtendFrameIntoClientArea
#pragma comment(lib, "Dwmapi.lib")

// Direct2D, DirectWrite and WIC (Windows Imaging Component) headers
#include <d2d1helper.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment (lib, "windowscodecs.lib")

// COM
#include <comdef.h>

// C++ STL headers
#include <vector>
#include <map>
#include <optional>

#define DESIGNLINES	0	// set to 1 to show design lines, 0 otherwise

// initialize static variables
std::atomic<unsigned long> liblec::lecui::form::form_impl::instances_ = 0;
std::atomic<bool> liblec::lecui::form::form_impl::initialized_ = false;
ID2D1Factory* liblec::lecui::form::form_impl::p_direct2d_factory_ = nullptr;
IDWriteFactory* liblec::lecui::form::form_impl::p_directwrite_factory_ = nullptr;
IWICImagingFactory* liblec::lecui::form::form_impl::p_iwic_factory_ = nullptr;

liblec::lecui::form::form_impl::form_impl(const std::string& caption_formatted) :
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
	clr_background_(color{ 244, 244, 244 }),
	clr_titlebar_background_(clr_background_),
	clr_theme_(color{ 0, 120, 170, 255 }),
	clr_theme_hot_(color{ 255, 180, 0, 255 }),
	clr_theme_disabled_(color{ 225, 225, 225, 255 }),
	top_most_(false),
	hWnd_(nullptr),
	hWnd_parent_(nullptr),
	caption_formatted_(caption_formatted),
	activate_(true),
	point_({ 0L, 0L }),
	size_({ 780.f, 480.f }),
	min_size_({ 500.f, 300.f }),
	allow_resizing_(true),
	allow_minimize_(true),
	user_pos_(false),
	preset_pos_(false),
	form_position_(form_position::center_to_working_area),
	dpi_scale_(1.f),
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
	lbutton_pressed_(false),
	on_drop_files_(nullptr),
	h_widget_cursor_(nullptr) {
	log("entering form_impl constructor");

	++instances_;	// increment instances count

	/// Use HeapSetInformation to specify that the process should terminate if the heap manager
	/// detects an error in any heap used by the process. The return value is ignored, because
	/// we want to continue running in the unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// parse the caption
	std::vector<widgets_implementation::text_range_properties> formatting;
	widgets_implementation::parse_formatted_text(caption_formatted_, caption_plain_,
		formatting);

	if (instances_ == 1) {
		// initialize COM
		HRESULT hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

		if (SUCCEEDED(hres)) {
			// Create a Direct2D factory
			hres = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &p_direct2d_factory_);
		}
		if (SUCCEEDED(hres)) {
			// Create a DirectWrite factory.
			hres = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(p_directwrite_factory_),
				reinterpret_cast<IUnknown**>(&p_directwrite_factory_));
		}
		if (SUCCEEDED(hres)) {
			// Create IWIC Imaging factory.
			hres = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&p_iwic_factory_));
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

liblec::lecui::form::form_impl::~form_impl() {
	// ....

	// release Direct2D resources specific to this form
	// 1. it is critical to do this before releasing the factories!!!!!
	// 2. it is critical to set the flag to true so prevent access violation errors as a result of
	//    attempting to call discard resources on custom widgets, which will probably have been
	//    destroyed already by now. Even if they aren't, let the custom widgets destroy their own
	//    resources in the client app when exiting; don't try doing it from here.
	discard_device_resources(true);

	if (initialized_ && instances_ == 1) {
		// release IWIC Imaging resources used by all instances
		safe_release(&p_iwic_factory_);

		// release DirectWrite resources used by all instances
		safe_release(&p_directwrite_factory_);

		// release Direct2D resources used by all instances
		safe_release(&p_direct2d_factory_);

		// Uninitialize COM
		CoUninitialize();

		// set initialized flag to false (only here)
		initialized_ = false;
	}

	--instances_;	// decremement instances count

	log("exiting form_impl destructor");
}

/// Creates resources that are bound to a particular Direct3D device. These resources need to
/// be recreated if the Direct3D device disappears, such as when the display changes, etc
/// 
/// This function will only create widget resources for those pages that have been
/// added in layout(). The resources for pages added later will be created
/// on-the-fly through the use of the
/// liblec::lecui::widgets_implementation::widget::resources_created_ flag (this flag is
/// used internally by each widget's implementation to check if resources have been created
/// and to create them if not).
HRESULT liblec::lecui::form::form_impl::create_device_resources() {
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
				static void create_resources(const containers::page& page,
					ID2D1HwndRenderTarget* p_render_target_) {
					// create widget resources
					for (auto& widget : page.d_page_.widgets()) {
						HRESULT hr = widget.second.create_resources(p_render_target_);

						if (widget.second.type() ==
							widgets_implementation::widget_type::tab_control) {
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
								widgets_implementation::widget_type::pane) {
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
void liblec::lecui::form::form_impl::discard_device_resources(bool in_destructor) {
	safe_release(&p_render_target_);
	safe_release(&p_brush_theme_);
	safe_release(&p_brush_theme_hot_);
	safe_release(&p_brush_theme_disabled_);
	safe_release(&p_brush_titlebar_);

	class helper {
	public:
		static void discard(const containers::page& page, bool in_destructor) {
			// discard widget resources
			for (const auto& widget : page.d_page_.widgets()) {
				if (in_destructor &&
					widget.second.type() ==
					widgets_implementation::widget_type::custom) {
					log("Skipping custom widget discard resources because by now custom widget will have long been destroyed so don't discard resources");
					continue;
				}

				widget.second.discard_resources();

				if (widget.second.type() ==
					widgets_implementation::widget_type::tab_control) {
					try {
						// get this tab control
						const auto& tab_control = page.d_page_.get_tab_control(widget.first);

						for (const auto& tab : tab_control.p_tabs_)
							discard(tab.second, in_destructor);			// recursion
					}
					catch (const std::exception&) {}
				}
				else
					if (widget.second.type() ==
						widgets_implementation::widget_type::pane) {
						try {
							// get this pane
							const auto& pane = page.d_page_.get_pane(widget.first);

							for (const auto& page : pane.p_panes_)
								discard(page.second, in_destructor);	// recursion
						}
						catch (const std::exception&) {}
					}
			}
		}
	};

	// discard page resources
	for (const auto& p_page : p_pages_)
		helper::discard(p_page.second, in_destructor);

	// discard form widget resources
	for (const auto& widget : widgets_)
		widget.second.discard_resources();
}

void liblec::lecui::form::form_impl::create_close_button(std::function<void()> on_click) {
	p_close_button_ =
		std::unique_ptr<widgets_implementation::close_button>(new
			widgets_implementation::close_button());
	widgets_.emplace(p_close_button_->name(), *p_close_button_);
	widgets_order_.emplace_back(p_close_button_->name());

	p_close_button_->specs().color_fill = clr_theme_;
	p_close_button_->specs().color_hot = clr_theme_hot_;
	p_close_button_->specs().color_disabled = clr_theme_disabled_;
	p_close_button_->specs().resize.perc_x = 100;

	p_close_button_->specs().rect.right = size_.width - control_button_margin_;
	p_close_button_->specs().rect.top = control_button_margin_;
	p_close_button_->specs().rect.left = p_close_button_->specs().rect.right -
		(caption_bar_height_ - 2.f * control_button_margin_);
	p_close_button_->specs().rect.bottom = p_close_button_->specs().rect.top +
		(caption_bar_height_ - 2.f * control_button_margin_);

	p_close_button_->specs().on_click = on_click;
}

// should be called after create_close_button()
void liblec::lecui::form::form_impl::create_maximize_button() {
	p_maximize_button_ =
		std::unique_ptr<widgets_implementation::maximize_button>(new
			widgets_implementation::maximize_button());
	widgets_.emplace(p_maximize_button_->name(), *p_maximize_button_);
	widgets_order_.emplace_back(p_maximize_button_->name());

	p_maximize_button_->specs().color_fill = clr_theme_;
	p_maximize_button_->specs().color_hot = clr_theme_hot_;
	p_maximize_button_->specs().color_disabled = clr_theme_disabled_;
	p_maximize_button_->specs().resize.perc_x = 100;

	const auto right_edge = p_close_button_->specs().rect.left;

	p_maximize_button_->specs().rect.right = right_edge - control_button_margin_;
	p_maximize_button_->specs().rect.top = control_button_margin_;
	p_maximize_button_->specs().rect.left = p_maximize_button_->specs().rect.right -
		(caption_bar_height_ - 2.f * control_button_margin_);
	p_maximize_button_->specs().rect.bottom = p_maximize_button_->specs().rect.top +
		(caption_bar_height_ - 2.f * control_button_margin_);
}

// should be called after create_close_button() and create_maximize_button()
void liblec::lecui::form::form_impl::create_minimize_button() {
	p_minimize_button_ =
		std::unique_ptr<widgets_implementation::minimize_button>(new
			widgets_implementation::minimize_button());
	widgets_.emplace(p_minimize_button_->name(), *p_minimize_button_);
	widgets_order_.emplace_back(p_minimize_button_->name());

	p_minimize_button_->specs().color_fill = clr_theme_;
	p_minimize_button_->specs().color_hot = clr_theme_hot_;
	p_minimize_button_->specs().color_disabled = clr_theme_disabled_;
	p_minimize_button_->specs().resize.perc_x = 100;

	const auto right_edge = allow_resizing_ ?
		p_maximize_button_->specs().rect.left :
		p_close_button_->specs().rect.left;

	p_minimize_button_->specs().rect.right = right_edge - control_button_margin_;
	p_minimize_button_->specs().rect.top = control_button_margin_;
	p_minimize_button_->specs().rect.left = p_minimize_button_->specs().rect.right -
		(caption_bar_height_ - 2.f * control_button_margin_);
	p_minimize_button_->specs().rect.bottom = p_minimize_button_->specs().rect.top +
		(caption_bar_height_ - 2.f * control_button_margin_);
}

void liblec::lecui::form::form_impl::create_form_caption(std::function<void()> on_click) {
	p_caption_ =
		std::unique_ptr<widgets_implementation::label>(new
			widgets_implementation::label("", "form_caption",
				p_directwrite_factory_));
	widgets_.emplace(p_caption_->name(), *p_caption_);
	widgets_order_.emplace_back(p_caption_->name());

	p_caption_->specs().text = caption_formatted_;
	p_caption_->specs().center_v = true;
	p_caption_->specs().multiline = false;

	// determine right-most edge based on available control buttons
	const auto right_edge = allow_minimize_ ?
		p_minimize_button_->specs().rect.left :
		(allow_resizing_ ?
			p_maximize_button_->specs().rect.left :
			p_close_button_->specs().rect.left);

	// determine the largest rect that the caption can occupy
	const D2D1_RECT_F max_rect = D2D1::RectF(10.f, control_button_margin_,
		right_edge - control_button_margin_, caption_bar_height_ - control_button_margin_);

	// determine the optimal rect for the caption
	const auto rect = widgets_implementation::measure_label(p_directwrite_factory_,
		p_caption_->specs().text, p_caption_->specs().font, p_caption_->specs().font_size,
		p_caption_->specs().center_h, p_caption_->specs().center_v, max_rect);

	// to-do: address this ... when form is downsized enough, caption creeps behind control
	// buttons since we've eliminated resizing caption rect; perhaps use clip???
	p_caption_->specs().rect = convert_rect(rect);

	p_caption_->specs().on_click = on_click;
}

void liblec::lecui::form::form_impl::update() { InvalidateRect(hWnd_, nullptr, FALSE); }

/// This method discards device-specific resources if the Direct3D device dissapears during
/// execution and recreates the resources the next time it's invoked
HRESULT liblec::lecui::form::form_impl::on_render() {
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

		const float line_width = .05f;

		for (int x = 0; x < width; x += 10)
			p_render_target_->DrawLine(D2D1::Point2F(static_cast<FLOAT>(x), .0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				p_brush_theme_, line_width);

		for (int y = 0; y < height; y += 10)
			p_render_target_->DrawLine(D2D1::Point2F(.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				p_brush_theme_, line_width);
#endif

		class helper {
		public:
			static void render_page(bool allow_render,
				const std::string& page_name,
				const std::string& current_page,
				containers::page& page,
				ID2D1HwndRenderTarget* p_render_target_,
				const D2D1_RECT_F& rectB,
				const D2D1_RECT_F& client_area,
				const float& change_in_width,
				const float& change_in_height,
				const float& dpi_scale_,
				ID2D1SolidColorBrush* p_brush_theme_,
				ID2D1SolidColorBrush* p_brush_theme_hot_,
				bool lbutton_pressed) {
				bool render = page_name == current_page;

				if (!allow_render)
					render = false;

				// define horizontal scroll parameters. rectA defines the area that contains
				// the widgets, while rectB defines the area in the main form outside of which
				// scrolling should kick in
				D2D1_RECT_F rectA = { 0.f, 0.f, 0.f, 0.f };

				{
					// clip
					auto_clip clip(render, p_render_target_, client_area, 1.f);

					do {
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
								page.d_page_.h_scrollbar().x_displacement_,
								x_displacement_translated_,
								page.d_page_.h_scrollbar().force_translate_)) {
								page.d_page_.h_scrollbar().force_translate_ = false;
								page.d_page_.h_scrollbar().x_off_set_ =
									x_displacement_translated_;
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
								page.d_page_.v_scrollbar().y_displacement_,
								y_displacement_translated_,
								page.d_page_.v_scrollbar().force_translate_)) {
								page.d_page_.v_scrollbar().force_translate_ = false;
								page.d_page_.v_scrollbar().y_off_set_ =
									y_displacement_translated_;
							}
						}

						// figure out furthest left and right

						// measure widgets
						bool initialized = false;
						D2D1_RECT_F rect_widgets_ = { 0.f, 0.f, 0.f, 0.f };
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								widgets_implementation::widget_type::h_scrollbar ||
								widget.second.type() ==
								widgets_implementation::widget_type::v_scrollbar ||
								widget.second.type() ==
								widgets_implementation::widget_type::group)
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

						// only attempt to correct needlessly hidden widgets if the left
						// mouse button is not pressed
						if (!lbutton_pressed) {
							bool correct = false;

							// don't let widgets be needlessly hidden to the left while
							// there's room on the right!
							if ((rectA.left < rectB.left) && (rectA.right < rectB.right)) {
								const auto left = rectA.left - rectB.left;
								const auto right = rectA.right - rectB.right;
								const auto x_overflow = abs(left) < abs(right) ? left : right;
								log(current_page + " correcting needless x_overflow of " +
									std::to_string(x_overflow));

								// translate the environment
								page.d_page_.h_scrollbar().x_displacement_ += x_overflow;
								page.d_page_.h_scrollbar().force_translate_ = true;
								correct = true;
							}

							// don't let widgets be needlessly hidden to the top while
							// there's room at the bottom!
							if ((rectA.top < rectB.top) && (rectA.bottom < rectB.bottom)) {
								const auto top = rectA.top - rectB.top;
								const auto bottom = rectA.bottom - rectB.bottom;
								const auto y_overflow = abs(top) < abs(bottom) ? top : bottom;
								log(current_page + " correcting needless y_overflow of " +
									std::to_string(y_overflow));

								// translate the environment
								page.d_page_.v_scrollbar().y_displacement_ += y_overflow;
								page.d_page_.v_scrollbar().force_translate_ = true;
								correct = true;
							}

							if (correct)
								continue;
						}

						break;
					} while (true);

					// resize groupboxes
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() !=
							widgets_implementation::widget_type::group)
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
										specs.rect.left = smallest(specs.rect.left,
											rect_.left);
										specs.rect.top = smallest(specs.rect.top,
											rect_.top);
										specs.rect.right = largest(specs.rect.right,
											rect_.right);
										specs.rect.bottom = largest(specs.rect.bottom,
											rect_.bottom);
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
							widgets_implementation::widget_type::group)
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
							widgets_implementation::widget_type::h_scrollbar ||
							widget.second.type() ==
							widgets_implementation::widget_type::v_scrollbar ||
							widget.second.type() ==
							widgets_implementation::widget_type::group)
							continue;

						widget.second.render(p_render_target_,
							change_in_width, change_in_height,
							(page.d_page_.h_scrollbar().x_off_set_ / dpi_scale_) - client_area.left,
							(page.d_page_.v_scrollbar().y_off_set_ / dpi_scale_) - client_area.top,
							render);

						if (widget.second.type() ==
							widgets_implementation::widget_type::tab_control) {
							try {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								// get client area for this tab control
								const auto& client_area = tab_control.client_area();

								const float change_in_width =
									(tab_control.tab_control_area().right - tab_control.tab_control_area().left) -
									(tab_control.specs().rect.right - tab_control.specs().rect.left);
								const float change_in_height =
									(tab_control.tab_control_area().bottom - tab_control.tab_control_area().top) -
									(tab_control.specs().rect.bottom - tab_control.specs().rect.top);

								for (auto& tab : tab_control.p_tabs_) {
									const float page_tolerance_ = 10.f;
									D2D1_RECT_F rect_page = client_area;
									rect_page.left += page_tolerance_;
									rect_page.top += page_tolerance_;
									rect_page.right -= page_tolerance_;
									rect_page.bottom -= page_tolerance_;

									render_page(render, tab.first, tab_control.current_tab_, tab.second,
										p_render_target_, rect_page, rect_page, change_in_width, change_in_height,
										dpi_scale_, p_brush_theme_, p_brush_theme_hot_, lbutton_pressed);	// recursion
								}
							}
							catch (const std::exception&) {}
						}
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::pane) {
								try {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// get client area for this pane
									const auto& client_area = pane.client_area();

									const float change_in_width =
										(pane.pane_area().right - pane.pane_area().left) -
										(pane.specs().rect.right - pane.specs().rect.left);
									const float change_in_height =
										(pane.pane_area().bottom - pane.pane_area().top) -
										(pane.specs().rect.bottom - pane.specs().rect.top);

									for (auto& page : pane.p_panes_) {
										const float page_tolerance_ = 10.f;
										D2D1_RECT_F rect_page = client_area;
										rect_page.left += page_tolerance_;
										rect_page.top += page_tolerance_;
										rect_page.right -= page_tolerance_;
										rect_page.bottom -= page_tolerance_;

										render_page(render, page.first, pane.current_pane_, page.second,
											p_render_target_, rect_page, rect_page, change_in_width, change_in_height,
											dpi_scale_, p_brush_theme_, p_brush_theme_hot_, lbutton_pressed);	// recursion
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
				containers::page& page,
				const D2D1_RECT_F& client_area) {
				bool render = page_name == current_page;

				if (!render)
					return;

				// clip
				auto_clip clip(render, p_render_target_, client_area, 1.f);

				// render widgets
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.type() ==
						widgets_implementation::widget_type::h_scrollbar ||
						widget.second.type() ==
						widgets_implementation::widget_type::v_scrollbar ||
						widget.second.type() ==
						widgets_implementation::widget_type::group)
						continue;

					widget.second.on_menu(p_render_target_, client_area);

					if (widget.second.type() ==
						widgets_implementation::widget_type::tab_control) {
						try {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							// get client area for this tab control
							const auto& client_area = tab_control.client_area();

							const float change_in_width =
								(tab_control.tab_control_area().right - tab_control.tab_control_area().left) -
								(tab_control.specs().rect.right - tab_control.specs().rect.left);
							const float change_in_height =
								(tab_control.tab_control_area().bottom - tab_control.tab_control_area().top) -
								(tab_control.specs().rect.bottom - tab_control.specs().rect.top);

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
							widgets_implementation::widget_type::pane) {
							try {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								// get client area for this pane
								const auto& client_area = pane.client_area();

								const float change_in_width =
									(pane.pane_area().right - pane.pane_area().left) -
									(pane.specs().rect.right - pane.specs().rect.left);
								const float change_in_height =
									(pane.pane_area().bottom - pane.pane_area().top) -
									(pane.specs().rect.bottom - pane.specs().rect.top);

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
				p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
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
				D2D1::RectF(.0f, .0f, rtSize.width, rtSize.height);
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
void liblec::lecui::form::form_impl::on_resize(UINT width, UINT height) {
	if (p_render_target_) {
		/// Note: This method can fail, but it's okay to ignore the error here, because the
		/// error will be returned again the next time EndDraw is called
		p_render_target_->Resize(D2D1::SizeU(width, height));
	}
}

RECT liblec::lecui::form::form_impl::get_working_area(HWND hWnd) {
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorinfo = { 0 };
	monitorinfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &monitorinfo);

	unscale_RECT(monitorinfo.rcWork, dpi_scale_);
	return monitorinfo.rcWork;
}

D2D1_POINT_2F liblec::lecui::form::form_impl::get_cursor_position() {
	POINT pt = { 0 };
	GetCursorPos(&pt);
	ScreenToClient(hWnd_, &pt);
	return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
}

void liblec::lecui::form::form_impl::set_position(const float& ix, const float& iy,
	const float& icx, const float& icy) {
	size_.width = icx;
	size_.height = icy;

	// ensure visibility of top left
	point_.x = largest(ix, 0.f);
	point_.y = largest(iy, 0.f);
}

void liblec::lecui::form::form_impl::set_position(const liblec::lecui::form_position& wndPos,
	const float& icx_in, const float& icy_in) {
	auto icx = icx_in;
	auto icy = icy_in;

	const float offset = 10.f;

	// get coordinates of working area
	RECT rcWork = get_working_area(GetDesktopWindow());

	form_position m_wndPos = wndPos;

	if (wndPos == form_position::center_to_parent &&
		hWnd_parent_ &&
		IsWindow(hWnd_parent_) &&
		(!IsWindowVisible(hWnd_parent_) || IsIconic(hWnd_parent_)))
		m_wndPos = form_position::center_to_working_area;

	switch (m_wndPos) {
	case form_position::center_to_parent: {
		if (hWnd_parent_ && IsWindow(hWnd_parent_)) {
			// get coordinates of parent window
			RECT rcParent;
			GetWindowRect(hWnd_parent_, &rcParent);
			unscale_RECT(rcParent, dpi_scale_);

			const float user_width = static_cast<float>(rcParent.right - rcParent.left);
			const float user_height = static_cast<float>(rcParent.bottom - rcParent.top);

			// center to working area
			const float ix = rcParent.left + ((user_width - icx) / 2.f);
			const float iy = rcParent.top + ((user_height - icy) / 2.f);

			point_.x = ix;
			point_.y = iy;
			size_.width = icx;
			size_.height = icy;

			break;
		}

		// allow to fall through to form_position::center_to_working_area
	}

	case form_position::center_to_working_area: {
		const float user_width = rcWork.right - rcWork.left + 0.f;
		const float user_height = rcWork.bottom - rcWork.top + 0.f;

		// center to working area
		const float ix = rcWork.left + ((user_width - icx) / 2.f);
		const float iy = rcWork.top + ((user_height - icy) / 2.f);

		point_.x = ix;
		point_.y = iy;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::top_left: {
		point_.x = rcWork.left + 0.f;
		point_.y = rcWork.top + 0.f;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::top_left_offset: {
		point_.x = rcWork.left + offset;
		point_.y = rcWork.top + offset;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::top_right: {
		point_.x = rcWork.right - icx;
		point_.y = rcWork.top + 0.f;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::top_right_offset: {
		point_.x = rcWork.right - icx - offset;
		point_.y = rcWork.top + offset;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::bottom_right: {
		point_.x = rcWork.right - icx;
		point_.y = rcWork.bottom - icy;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::bottom_right_offset: {
		point_.x = rcWork.right - icx - offset;
		point_.y = rcWork.bottom - icy - offset;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::bottom_left: {
		point_.x = rcWork.left + 0.f;
		point_.y = rcWork.bottom - icy;
		size_.width = icx;
		size_.height = icy;
	} break;

	case form_position::bottom_left_offset: {
		point_.x = rcWork.left + offset;
		point_.y = rcWork.bottom - icy - offset;
		size_.width = icx;
		size_.height = icy;
	} break;

	default: {
		// default to top left
		point_.x = rcWork.left + 0.f;
		point_.y = rcWork.top + 0.f;
		size_.width = icx;
		size_.height = icy;
	} break;
	}

	// ensure visibility of top left
	point_.x = largest(point_.x, 0.f);
	point_.y = largest(point_.y, 0.f);
}

bool liblec::lecui::form::form_impl::composition_enabled() {
	BOOL composition_enabled = FALSE;
	bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
	return composition_enabled && success;
}

liblec::lecui::form::form_impl::style liblec::lecui::form::form_impl::select_borderless_style() {
	return composition_enabled() ? style::aero_borderless : style::basic_borderless;
}

void liblec::lecui::form::form_impl::set_shadow(HWND hWnd, bool enabled) {
	if (composition_enabled()) {
		static const MARGINS shadow_state[2]{ { 0, 0, 0, 0 },{ 1, 1, 1, 1 } };
		::DwmExtendFrameIntoClientArea(hWnd, &shadow_state[enabled]);
	}
}

void liblec::lecui::form::form_impl::set_borderless(HWND hWnd, bool enabled) {
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

void liblec::lecui::form::form_impl::set_borderless_shadow(HWND hWnd, bool enabled, bool save_value) {
	if (borderless_) {
		if (save_value)
			borderless_shadow_ = enabled;

		set_shadow(hWnd, enabled);
	}
}

bool liblec::lecui::form::form_impl::maximized(HWND hwnd) {
	WINDOWPLACEMENT placement = { 0 };
	if (GetWindowPlacement(hwnd, &placement))
		return placement.showCmd == SW_MAXIMIZE;
	else
		return false;
}

/// Adjust client rect to not spill over monitor edges when maximized.
/// rect(in/out): in: proposed window rect, out: calculated client rect
/// Does nothing if the window is not maximized.
void liblec::lecui::form::form_impl::adjust_maximized_client_rect(HWND window, RECT& rect) {
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

LRESULT liblec::lecui::form::form_impl::hit_test(const POINT& cursor) {
	// identify borders and corners to allow resizing the window.
	// Note: On Windows 10, windows behave differently and
	// allow resizing outside the visible window frame.
	// This implementation does not replicate that behavior.
	// to-do: check if this behavior can be replicated for a borderless window.
	POINT border{
		::GetSystemMetrics(SM_CXFRAME),
		::GetSystemMetrics(SM_CYFRAME)
	};

	border.x = static_cast<long>(.5f + dpi_scale_ * border.x);
	border.y = static_cast<long>(.5f + dpi_scale_ * border.y);

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
		cursor.y < (window.top + static_cast<long>(.5f + dpi_scale_ * caption_bar_height_))) {
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

void liblec::lecui::form::form_impl::client_hittest(const D2D1_POINT_2F& point) {
	bool contains = false;
	bool change = false;
	HCURSOR h_cursor = nullptr;

	class helper {
	public:
		static void hittest_hscrollbar(const std::string& page_name,
			const std::string& current_page, containers::page& page,
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
						widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						for (auto& tab : tab_control.p_tabs_)
							hittest_hscrollbar(tab.first, tab_control.current_tab_, tab.second,
								point, point_before, contains, change);	// recursion
					}
					else
						if (widget.second.type() ==
							widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							for (auto& page : pane.p_panes_)
								hittest_hscrollbar(page.first, pane.current_pane_, page.second,
									point, point_before, contains, change);	// recursion
						}
				}
			}
		}

		static void hittest_vscrollbar(const std::string& page_name,
			const std::string& current_page, containers::page& page,
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
						widgets_implementation::widget_type::tab_control) {
						// get this tab control
						auto& tab_control = page.d_page_.get_tab_control(widget.first);

						for (auto& tab : tab_control.p_tabs_)
							hittest_vscrollbar(tab.first, tab_control.current_tab_, tab.second,
								point, point_before, contains, change);	// recursion
					}
					else
						if (widget.second.type() ==
							widgets_implementation::widget_type::pane) {
							// get this pane
							auto& pane = page.d_page_.get_pane(widget.first);

							for (auto& page : pane.p_panes_)
								hittest_vscrollbar(page.first, pane.current_pane_, page.second,
									point, point_before, contains, change);	// recursion
						}
				}
			}
		}

		static void hittest_widgets(containers::page& page,
			const D2D1_POINT_2F& point, bool& contains, bool& change, bool lbutton_pressed, HCURSOR& h_cursor) {
			bool in_page = page.d_page_.contains(point);

			// hit test widgets
			for (auto& widget : page.d_page_.widgets()) {
				bool is_scroll_bar = (widget.second.type() ==
					widgets_implementation::widget_type::h_scrollbar) ||
					(widget.second.type() ==
						widgets_implementation::widget_type::v_scrollbar);

				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (change)
					break;

				contains = (in_page || is_scroll_bar || lbutton_pressed) ? widget.second.contains(point) : false;
				if (change = widget.second.hit(contains)) {
					if (widget.second.hit()) h_cursor = widget.second.cursor();
					break;
				}

				if (widget.second.type() ==
					widgets_implementation::widget_type::tab_control) {
					// get this tab control
					auto& tab_control = page.d_page_.get_tab_control(widget.first);

					auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

					if (page_iterator != tab_control.p_tabs_.end())
						helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor);	// recursion
				}
				else
					if (widget.second.type() ==
						widgets_implementation::widget_type::pane) {
						// get this pane
						auto& pane = page.d_page_.get_pane(widget.first);

						auto page_iterator = pane.p_panes_.find(pane.current_pane_);

						if (page_iterator != pane.p_panes_.end())
							helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor);	// recursion
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
			helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed_, h_cursor);
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

	if (change) {
		h_widget_cursor_ = h_cursor;
		update();
	}
}

void liblec::lecui::form::form_impl::on_lbuttondown(const D2D1_POINT_2F& point) {
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
		static void check_widgets(containers::page& page,
			const D2D1_POINT_2F& point, const float& dpi_scale, bool& pressed,
			bool& update_anyway) {
			bool in_page = page.d_page_.contains(point);

			// check widgets
			for (auto& widget : page.d_page_.widgets()) {
				bool is_scroll_bar = (widget.second.type() ==
					widgets_implementation::widget_type::h_scrollbar) ||
					(widget.second.type() ==
						widgets_implementation::widget_type::v_scrollbar);

				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.selected())
					update_anyway = true;

				if (!pressed) {
					// pressed widget not yet found
					pressed = (in_page || is_scroll_bar) ? widget.second.contains(point) : false;
					widget.second.press(pressed);

					if (widget.second.type() !=
						widgets_implementation::widget_type::tab_control)
						widget.second.select(pressed);
					else
						if (widget.second.type() !=
							widgets_implementation::widget_type::pane)
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
					widgets_implementation::widget_type::tab_control) {
					// get this tab control
					auto& tab_control = page.d_page_.get_tab_control(widget.first);

					auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

					if (page_iterator != tab_control.p_tabs_.end())
						helper::check_widgets(page_iterator->second, point, dpi_scale, pressed,
							update_anyway);
				}
				else
					if (widget.second.type() ==
						widgets_implementation::widget_type::pane) {
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

void liblec::lecui::form::form_impl::on_lbuttonup(const D2D1_POINT_2F& point) {
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
		static void check_widgets(containers::page& page,
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
					widgets_implementation::widget_type::tab_control) {
					// get this tab control
					auto& tab_control = page.d_page_.get_tab_control(widget.first);

					auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

					if (page_iterator != tab_control.p_tabs_.end())
						check_widgets(page_iterator->second, point, clicked, update_anyway,
							on_click_handler);
				}
				else
					if (widget.second.type() ==
						widgets_implementation::widget_type::pane) {
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

bool liblec::lecui::form::form_impl::destroy_menus() {
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
		static void check_widgets(containers::page& page,
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
					widgets_implementation::widget_type::tab_control) {
					// get this tab control
					auto& tab_control = page.d_page_.get_tab_control(widget.first);

					auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

					if (page_iterator != tab_control.p_tabs_.end())
						helper::check_widgets(page_iterator->second, update);
				}
				else
					if (widget.second.type() ==
						widgets_implementation::widget_type::pane) {
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

int liblec::lecui::form::form_impl::make_unique_id() {
	++unique_id_;
	return unique_id_;
}

void liblec::lecui::form::form_impl::start_timer(const std::string& name) {
	if (timers_.find(name) != timers_.end()) {
		log("starting timer: " + name);
		SetTimer(hWnd_, (UINT_PTR)timers_.at(name).unique_id,
			(UINT)timers_.at(name).milliseconds, NULL);
		timers_.at(name).running = true;
	}
}

void liblec::lecui::form::form_impl::parse_widget_path(const std::string& name,
	std::vector<std::string>& path, std::string& widget_name) {
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

void liblec::lecui::form::form_impl::parse_container_path(const std::string& name,
	std::vector<std::string>& path, std::string& container_name) {
	return parse_widget_path(name, path, container_name);
}

// throws on failure!
liblec::lecui::widgets_implementation::widget& liblec::lecui::form::form_impl::find_widget(
	const containers::page& page,
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

			return find_widget(page, path_, widget_name);	// recursion
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

			return find_widget(tab, path_, widget_name);	// recursion
		}
}

// throws on failure!
liblec::lecui::widgets_implementation::widget& liblec::lecui::form::form_impl::find_widget(
	const std::map<std::string, containers::page>& pages,
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

// throws on failure!
liblec::lecui::containers::page& liblec::lecui::form::form_impl::find_page(
	containers::page& page, const std::vector<std::string>& path) {
	if (path.size() == 0)
		return page;
	else
		if (path.size() == 1) {
			// assume this is a pane
			const auto& pane_name = path[0];
			auto& pane = page.d_page_.get_pane(pane_name);

			auto& page = pane.p_panes_.at("pane");

			std::vector<std::string> path_(path.size() - 1);
			for (size_t i = 1; i < path.size(); i++)
				path_[i - 1] = path[i];

			return find_page(page, path_);	// recursion
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

			return find_page(tab, path_);	// recursion
		}
}

// throws on failure!
liblec::lecui::containers::page& liblec::lecui::form::form_impl::find_page(
	std::map<std::string, containers::page>& pages, const std::vector<std::string>& path) {
	if (path.size() == 0)
		throw std::exception("Invalid path");
	else
		if (path.size() == 1) {
			const auto page_name = path[0];
			return pages.at(page_name);
		}
		else {
			// assume first level is a page
			const auto page_name = path[0];

			auto& page = pages.at(page_name);

			std::vector<std::string> path_(path.size() - 1);
			for (size_t i = 1; i < path.size(); i++)
				path_[i - 1] = path[i];

			return find_page(page, path_);
		}
}

void liblec::lecui::form::form_impl::enable(const std::string& name, bool enable) {
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

void liblec::lecui::form::form_impl::show(const std::string& name, bool show) {
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

void liblec::lecui::form::form_impl::close(const std::string& name) {
	try {
		std::vector<std::string> path;
		std::string widget_name;
		parse_widget_path(name, path, widget_name);

		auto& widget = find_widget(p_pages_, path, widget_name);
		auto& page = find_page(p_pages_, path);

		const auto type = widget.type();

		///
		/// close widget by
		/// 1. removing it from widgets_
		/// 2. removing it from the page it's in, e.g. tab_controls_ for tab controls
		/// 3. removing it from widgets_order_

		// step 1
		page.d_page_.widgets_.erase(widget_name);

		// step 2
		switch (type) {
		case widgets_implementation::widget_type::rectangle:
			page.d_page_.rectangles_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::label:
			page.d_page_.labels_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::group:
			page.d_page_.groups_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::tab_control:
			page.d_page_.tab_controls_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::button:
			page.d_page_.buttons_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::toggle:
			page.d_page_.toggles_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::combo:
			page.d_page_.combos_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::list:
			page.d_page_.lists_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::custom:
			page.d_page_.customs_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::pane:
			page.d_page_.panes_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::image:
			page.d_page_.images_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::progress_indicator:
			page.d_page_.progress_indicators_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::progress_bar:
			page.d_page_.progress_bars_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::checkbox:
			page.d_page_.checkboxes_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::textbox:
			page.d_page_.textboxes_.erase(widget_name);
			break;
		case widgets_implementation::widget_type::close_button:
		case widgets_implementation::widget_type::maximize_button:
		case widgets_implementation::widget_type::minimize_button:
		case widgets_implementation::widget_type::h_scrollbar:
		case widgets_implementation::widget_type::v_scrollbar:
		default:
			break;
		}

		// step 3
		std::vector<std::string> widgets_order;
		for (auto name : page.d_page_.widgets_order_)
			if (name != widget_name)
				widgets_order.push_back(name);
		page.d_page_.widgets_order_ = widgets_order;

		update();
	}
	catch (const std::exception&) {}
}

LRESULT CALLBACK liblec::lecui::form::form_impl::window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto get_form = [&]() {
		form* p_form = nullptr;
		if (msg == WM_CREATE) {
			CREATESTRUCT* p_create = reinterpret_cast<CREATESTRUCT*>(lParam);
			p_form = reinterpret_cast<form*>(p_create->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)p_form);
		}
		else {
			LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
			p_form = reinterpret_cast<form*>(ptr);
		}

		return (p_form == nullptr) ? std::nullopt :
			std::optional<std::reference_wrapper<form>>{ *p_form };
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

		form_.value().get().on_start();

		for (auto& it : form_.value().get().d_.timers_)
			if (!it.second.running)
				form_.value().get().d_.start_timer(it.first);

		return TRUE;

	case WM_LBUTTONDOWN:
		form_.value().get().d_.lbutton_pressed_ = true;
		// capture the mouse
		SetCapture(hWnd);
		form_.value().get().d_.on_lbuttondown(form_.value().get().d_.get_cursor_position());
		return NULL;

	case WM_LBUTTONUP:
		// release the mouse capture
		ReleaseCapture();
		form_.value().get().d_.on_lbuttonup(form_.value().get().d_.get_cursor_position());
		form_.value().get().d_.lbutton_pressed_ = false;
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
				static void check_page(containers::page& page,
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
						if (widget.second.type() == widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							for (auto& tab : tab_control.p_tabs_)
								helper::check_page(tab.second, change_in_width, change_in_height, system_resizing);	// recursion
						}
						else
							if (widget.second.type() == widgets_implementation::widget_type::pane) {
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

	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) {
			HCURSOR h_cursor = form_.value().get().d_.h_widget_cursor_;
			if (h_cursor) {
				SetCursor(h_cursor);
				return TRUE;
			}
		}
		break;

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
		form_.value().get().on_shutdown();
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
		p_minmaxinfo->ptMinTrackSize.x = static_cast<LONG>(.5f +
			form_.value().get().d_.min_size_.width * form_.value().get().d_.dpi_scale_);
		p_minmaxinfo->ptMinTrackSize.y = static_cast<LONG>(.5f +
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
	} break;

	case WM_CHAR: {
		const char c = (char)wParam;
		bool change = false;

		class helper {
		public:
			static void check_widgets(containers::page& page, const char& c, bool& change) {
				for (auto& widget : page.d_page_.widgets()) {
					if (widget.second.type() ==
						widgets_implementation::widget_type::textbox && widget.second.selected()) {
						change = true;
						try {
							// ignore backspace, tab and return
							if (c == '\b' ||
								c == '\t' ||
								c == '\r')
								break;

							// insert character
							auto& textbox = page.d_page_.get_textbox(widget.first);
							textbox.insert_character(c);
						}
						catch (const std::exception& e) { log(e.what()); }
						break;
					}
					else
						if (widget.second.type() ==
							widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

							if (page_iterator != tab_control.p_tabs_.end())
								helper::check_widgets(page_iterator->second, c, change);	// recursion
						}
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									helper::check_widgets(page_iterator->second, c, change);	// recursion
							}
				}
			}
		};
		
		// get current page
		auto page_iterator = form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

		if (page_iterator != form_.value().get().d_.p_pages_.end())
			helper::check_widgets(page_iterator->second, c, change);

		if (change)
			form_.value().get().d_.update();
	} break;

	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;	// for VK_UP, VK_DOWN to be received in WM_KEYDOWN

	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE: {
			bool change = false;

			class helper {
			public:
				static void check_widgets(containers::page& page, WPARAM wParam, bool& change) {
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							widgets_implementation::widget_type::textbox && widget.second.selected()) {
							change = true;
							try {
								auto& textbox = page.d_page_.get_textbox(widget.first);

								switch (wParam) {
								case VK_LEFT: textbox.key_left(); break;
								case VK_RIGHT: textbox.key_right(); break;
								case VK_BACK: textbox.key_backspace(); break;
								case VK_DELETE: textbox.key_delete(); break;
								default:
									break;
								}
							}
							catch (const std::exception& e) { log(e.what()); }
							break;
						}
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, wParam, change);	// recursion
							}
							else
								if (widget.second.type() ==
									widgets_implementation::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second, wParam, change);	// recursion
								}
					}
				}
			};

			// get current page
			auto page_iterator = form_.value().get().d_.p_pages_.find(form_.value().get().d_.current_page_);

			if (page_iterator != form_.value().get().d_.p_pages_.end())
				helper::check_widgets(page_iterator->second, wParam, change);

			if (change)
				form_.value().get().d_.update();
		} break;

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
					widgets_implementation::widget_type::close_button &&
					widget.second.type() !=
					widgets_implementation::widget_type::maximize_button &&
					widget.second.type() !=
					widgets_implementation::widget_type::minimize_button)
					widget.second.press(true);
				else
					widget.second.press(false);
			}

			class helper {
			public:
				static void check_widgets(containers::page& page) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.selected() &&
							widget.second.type() != widgets_implementation::widget_type::textbox)	// exclude text box from space bar presses
							widget.second.press(true);
						else
							widget.second.press(false);

						if (widget.second.type() ==
							widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

							if (page_iterator != tab_control.p_tabs_.end())
								helper::check_widgets(page_iterator->second);
						}
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::pane) {
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
				widgets_implementation::widget_type::close_button &&
				widget.second.type() !=
				widgets_implementation::widget_type::maximize_button &&
				widget.second.type() !=
				widgets_implementation::widget_type::minimize_button)
				if (widget.second.on_keydown(wParam)) {
					update = true;
					on_click_handler = [&]() { widget.second.on_click(); };
				}
		}

		class helper {
		public:
			static void check_widgets(containers::page& page,
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
								widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
							}
							else
								if (widget.second.type() ==
									widgets_implementation::widget_type::pane) {
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
					widgets_implementation::widget_type::close_button &&
					widget.second.type() !=
					widgets_implementation::widget_type::maximize_button &&
					widget.second.type() !=
					widgets_implementation::widget_type::minimize_button)
					on_space = [&]() { widget.second.on_click(); };

				// reset pressed status
				widget.second.press(false);
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					std::function<void()>& on_space) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.selected())
							on_space = [&]() { widget.second.on_click(); };
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(widget.first);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, on_space);
							}
							else
								if (widget.second.type() ==
									widgets_implementation::widget_type::pane) {
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
				static void check_widgets(containers::page& page,
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
								widgets_implementation::widget_type::tab_control) {
								// get this tab control
								auto& tab_control = page.d_page_.get_tab_control(name);

								auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

								if (page_iterator != tab_control.p_tabs_.end())
									helper::check_widgets(page_iterator->second, reverse_tab_navigation,
										select_next, selected);
							}
							else
								if (widget.type() ==
									widgets_implementation::widget_type::pane) {
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
							widgets_implementation::widget_type::close_button &&
							widget.type() !=
							widgets_implementation::widget_type::maximize_button &&
							widget.type() !=
							widgets_implementation::widget_type::minimize_button) {
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
				widgets_implementation::widget_type::close_button &&
				widget.second.type() !=
				widgets_implementation::widget_type::maximize_button &&
				widget.second.type() !=
				widgets_implementation::widget_type::minimize_button)
				if (widget.second.on_mousewheel(units))
					update = true;
		}

		class helper {
		public:
			static void check_widgets(containers::page& page,
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
							widgets_implementation::widget_type::tab_control) {
							// get this tab control
							auto& tab_control = page.d_page_.get_tab_control(widget.first);

							auto page_iterator = tab_control.p_tabs_.find(tab_control.current_tab_);

							if (page_iterator != tab_control.p_tabs_.end())
								helper::check_widgets(page_iterator->second, units, update);
						}
						else
							if (widget.second.type() ==
								widgets_implementation::widget_type::pane) {
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

	case WM_DROPFILES: {
		if (form_.value().get().d_.on_drop_files_ != nullptr) {
			// a file has been dropped onto the form
			CHAR file[MAX_PATH];
			ZeroMemory(file, sizeof(file));

			HDROP hDrp = (HDROP)wParam;
			DragQueryFileA(hDrp, 0, file, sizeof(file));
			DragFinish(hDrp);

			const std::string fullpath = file;
			form_.value().get().d_.on_drop_files_(fullpath);
		}
	} break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
