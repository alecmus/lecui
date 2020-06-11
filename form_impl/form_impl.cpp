/*
** form_impl.cpp - form::impl implementation
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

#include "../containers/page.h"
#include "../containers/tab_pane.h"
#include "../containers/pane.h"
#include "../containers/group.h"

#include "../widgets/rectangle.h"
#include "../widgets/label.h"
#include "../widgets/button.h"
#include "../widgets/h_scrollbar/h_scrollbar.h"
#include "../widgets/v_scrollbar/v_scrollbar.h"

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

namespace liblec {
	namespace lecui {
		// initialize static variables
		std::atomic<unsigned long> form::impl::instances_ = 0;
		std::atomic<bool> form::impl::initialized_ = false;
		ID2D1Factory* form::impl::p_direct2d_factory_ = nullptr;
		IDWriteFactory* form::impl::p_directwrite_factory_ = nullptr;
		IWICImagingFactory* form::impl::p_iwic_factory_ = nullptr;

		form::impl::impl(const std::string& caption_formatted) :
			p_parent_(nullptr),
			menu_form_(caption_formatted == form::menu_form_caption()),
			parent_closing_(false),
			show_called_(false),
			caption_bar_height_(menu_form_ ? 0.f : 30.f),
			form_border_thickness_(1.f),
			page_tolerance_(form_border_thickness_ / 2.f),
			control_button_margin_(2.f),
			resource_dll_filename_(std::string()),
			resource_module_handle_(nullptr),
			idi_icon_(0),
			idi_icon_small_(0),
			theme_(themes::light),
			clr_background_(defaults::color(theme_, item::form)),
			clr_titlebar_background_(defaults::color(theme_, item::titlebar)),
			clr_theme_(defaults::color(theme_, item::accent)),
			clr_theme_hot_(defaults::color(theme_, item::accent_hover)),
			clr_theme_disabled_(defaults::color(theme_, item::disabled)),
			clr_theme_text_(defaults::color(theme_, item::label)),
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
			on_caption_(nullptr),
			on_drop_files_(nullptr),
			h_widget_cursor_(nullptr) {
			++instances_;	// increment instances count

			/// Use HeapSetInformation to specify that the process should terminate if the heap manager
			/// detects an error in any heap used by the process. The return value is ignored, because
			/// we want to continue running in the unlikely event that HeapSetInformation fails.
			HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

			// parse the caption
			// the default color doesn't matter here we're just getting the plain text
			std::vector<formatted_text_parser::text_range_properties> formatting;
			widgets_impl::parse_formatted_text(caption_formatted_, caption_plain_,
				D2D1::ColorF(D2D1::ColorF::Black), formatting);

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

					MessageBox(nullptr, error.c_str(), L"form", MB_ICONERROR);

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
						L"form", MB_ICONWARNING);
			}

			// capture current DPI scale
			HDC hdc_screen = GetDC(NULL);
			dpi_scale_ = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
			ReleaseDC(NULL, hdc_screen);

			// ...
		}

		form::impl::~impl() {
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

			log("exiting form::impl destructor");
		}

		/// Creates resources that are bound to a particular Direct3D device. These resources need to
		/// be recreated if the Direct3D device disappears, such as when the display changes, etc
		/// 
		/// This function will only create widget resources for those pages that have been
		/// added in layout(). The resources for pages added later will be created
		/// on-the-fly through the use of the
		/// widgets_impl::widget::resources_created_ flag (this flag is
		/// used internally by each widget's implementation to check if resources have been created
		/// and to create them if not).
		HRESULT form::impl::create_device_resources() {
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
									widgets_impl::widget_type::tab_pane) {
									try {
										// get this tab pane
										auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

										for (auto& tab : tab_pane.p_tabs_)
											create_resources(tab.second, p_render_target_);
									}
									catch (const std::exception&) {}
								}
								else
									if (widget.second.type() ==
										widgets_impl::widget_type::pane) {
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
		void form::impl::discard_device_resources(bool in_destructor) {
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
							widgets_impl::widget_type::custom) {
							log("Skipping custom widget discard resources because by now custom widget will have long been destroyed so don't discard resources");
							continue;
						}

						widget.second.discard_resources();

						if (widget.second.type() ==
							widgets_impl::widget_type::tab_pane) {
							try {
								// get this tab pane
								const auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								for (const auto& tab : tab_pane.p_tabs_)
									discard(tab.second, in_destructor);			// recursion
							}
							catch (const std::exception&) {}
						}
						else
							if (widget.second.type() ==
								widgets_impl::widget_type::pane) {
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

		void form::impl::create_close_button(std::function<void()> on_click) {
			p_close_button_ =
				std::unique_ptr<widgets_impl::close_button>(new
					widgets_impl::close_button());
			widgets_.emplace(p_close_button_->alias(), *p_close_button_);
			widgets_order_.emplace_back(p_close_button_->alias());
			
			p_close_button_->specs().color_fill = clr_theme_;
			p_close_button_->specs().color_hot = clr_theme_hot_;
			p_close_button_->specs().color_disabled = clr_theme_disabled_;
			p_close_button_->specs().on_resize.perc_x = 100;

			p_close_button_->specs().rect.right = size_.width - control_button_margin_;
			p_close_button_->specs().rect.top = control_button_margin_;
			p_close_button_->specs().rect.left = p_close_button_->specs().rect.right -
				(caption_bar_height_ - 2.f * control_button_margin_);
			p_close_button_->specs().rect.bottom = p_close_button_->specs().rect.top +
				(caption_bar_height_ - 2.f * control_button_margin_);

			p_close_button_->specs().events().click = on_click;
		}

		// should be called after create_close_button()
		void form::impl::create_maximize_button() {
			p_maximize_button_ =
				std::unique_ptr<widgets_impl::maximize_button>(new
					widgets_impl::maximize_button());
			widgets_.emplace(p_maximize_button_->alias(), *p_maximize_button_);
			widgets_order_.emplace_back(p_maximize_button_->alias());

			p_maximize_button_->specs().color_fill = clr_theme_;
			p_maximize_button_->specs().color_hot = clr_theme_hot_;
			p_maximize_button_->specs().color_disabled = clr_theme_disabled_;
			p_maximize_button_->specs().on_resize.perc_x = 100;

			const auto right_edge = p_close_button_->specs().rect.left;

			p_maximize_button_->specs().rect.right = right_edge - control_button_margin_;
			p_maximize_button_->specs().rect.top = control_button_margin_;
			p_maximize_button_->specs().rect.left = p_maximize_button_->specs().rect.right -
				(caption_bar_height_ - 2.f * control_button_margin_);
			p_maximize_button_->specs().rect.bottom = p_maximize_button_->specs().rect.top +
				(caption_bar_height_ - 2.f * control_button_margin_);
		}

		// should be called after create_close_button() and create_maximize_button()
		void form::impl::create_minimize_button() {
			p_minimize_button_ =
				std::unique_ptr<widgets_impl::minimize_button>(new
					widgets_impl::minimize_button());
			widgets_.emplace(p_minimize_button_->alias(), *p_minimize_button_);
			widgets_order_.emplace_back(p_minimize_button_->alias());

			p_minimize_button_->specs().color_fill = clr_theme_;
			p_minimize_button_->specs().color_hot = clr_theme_hot_;
			p_minimize_button_->specs().color_disabled = clr_theme_disabled_;
			p_minimize_button_->specs().on_resize.perc_x = 100;

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

		void form::impl::create_form_caption() {
			p_caption_ =
				std::unique_ptr<widgets_impl::label>(new
					widgets_impl::label("", "form_caption",
						p_directwrite_factory_));
			widgets_.emplace(p_caption_->alias(), *p_caption_);
			widgets_order_.emplace_back(p_caption_->alias());

			p_caption_->specs().text = caption_formatted_;
			p_caption_->specs().center_v = true;
			p_caption_->specs().multiline = false;

			// load label settings
			p_caption_->specs().color_text = defaults::color(theme_, item::label);
			p_caption_->specs().color_selected = defaults::color(theme_, item::label_selected);
			p_caption_->specs().color_hot = defaults::color(theme_, item::label_hover);
			p_caption_->specs().color_hot_pressed = defaults::color(theme_, item::label_pressed);
			p_caption_->specs().color_disabled = defaults::color(theme_, item::label_disabled);

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
			const auto rect = widgets_impl::measure_label(p_directwrite_factory_,
				p_caption_->specs().text, p_caption_->specs().font, p_caption_->specs().font_size,
				p_caption_->specs().center_h, p_caption_->specs().center_v, max_rect);

			// to-do: address this ... when form is downsized enough, caption creeps behind control
			// buttons since we've eliminated resizing caption rect; perhaps use clip???
			p_caption_->specs().rect = convert_rect(rect);

			p_caption_->specs().events().click = on_caption_;
		}

		void form::impl::update() { InvalidateRect(hWnd_, nullptr, FALSE); }

		/// <summary>
		/// Move all the trees in a page into a special tree pane. If a tree has
		/// already been moved it will be left where it is.
		/// </summary>
		void form::impl::move_trees() {
			// check if this page has a tree pane
			auto page_iterator = p_pages_.find(current_page_);

			struct tree_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::tree::tree_specs tree;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<tree_info> trees;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_trees_to_move(lecui::containers::page& page,
						std::vector<tree_info>& trees) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is a tree pane
							if (widget.first.find(widgets_impl::pane::tree_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a tree pane (it has a tree inside. move was already done), continue to next widget

							// check if this is a tree
							if (widget.second.type() == widgets_impl::widget_type::tree) {
								// this is a tree, we need to "move" it into a special pane

								// get the tree specs
								auto& tree_specs = page.d_page_.get_tree(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets_impl::pane::tree_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = tree_specs.rect;
								pane().on_resize = tree_specs.on_resize;
								pane().color_fill = tree_specs.color_fill;
								pane().color_border = tree_specs.color_border;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								trees.push_back({ widget.first, tree_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets_impl::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_trees_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.second.type() == widgets_impl::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_trees_to_move(page.second, trees);	// recursion
								}
						}
					}
				};

				helper::find_trees_to_move(page, trees);

				// move the trees
				for (auto& it : trees) {
					log("moving tree: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::tree tree(it.destination, it.alias);
						// copy specs
						tree() = it.tree;

						// adjust specs
						tree().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						tree().on_resize = { 0, 0, 0, 0 };	// critical because tree will change size as tree is browsed or changed. the pane scroll bars will do the job.
						tree().color_fill.alpha = 0;
						tree().color_border.alpha = 0;

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets_impl::widget_type::tree, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			}
		}

		void form::impl::move_html_editors() {
			// check if this page has an html widget
			auto page_iterator = p_pages_.find(current_page_);

			struct html_editor_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::html_editor::html_editor_specs html_editor;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<html_editor_info> html_editors;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_html_editors_to_move(lecui::containers::page& page,
						std::vector<html_editor_info>& trees) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is an html pane
							if (widget.first.find(widgets_impl::pane::html_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a tree pane (it has a tree inside. move was already done), continue to next widget

							// check if this is an html editor
							if (widget.second.type() == widgets_impl::widget_type::html_editor) {
								// this is an html editor, we need to "move" it into a special pane

								// get the html editor specs
								auto& html_editor_specs = page.d_page_.get_html_editor(widget.first).specs();

								// make controls pane in source (predefined, fixed height)
								containers::pane controls_pane(page, widgets_impl::pane::html_controls_pane_alias_prefix() + widget.first);
								controls_pane().rect = html_editor_specs.rect;
								controls_pane().rect.height(40.f);
								controls_pane().on_resize = html_editor_specs.on_resize;
								controls_pane().on_resize.perc_height = 0.f;
								controls_pane().on_resize.min_height = 0.f;

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets_impl::pane::html_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = html_editor_specs.rect;
								pane().rect.top = controls_pane().rect.bottom;
								pane().on_resize = html_editor_specs.on_resize;
								if (pane().on_resize.min_height)
									pane().on_resize.min_height = largest(pane().on_resize.min_height - controls_pane().rect.height(), 0.f);
								pane().color_fill = html_editor_specs.color_fill;
								pane().color_border = html_editor_specs.color_border;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								trees.push_back({ widget.first, html_editor_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets_impl::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_html_editors_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.second.type() == widgets_impl::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_html_editors_to_move(page.second, trees);	// recursion
								}
						}
					}
				};

				helper::find_html_editors_to_move(page, html_editors);

				// move the html editors
				for (auto& it : html_editors) {
					log("moving html_editor: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::html_editor html_editor(it.destination, it.alias);
						// copy specs
						html_editor() = it.html_editor;

						// adjust specs
						html_editor().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						html_editor().on_resize = { 0.f, 0.f, 100.f, 0.f };	// critical because html_editor will change height as user types or contents are changed. the pane scroll bars will do the job.
						html_editor().color_fill.alpha = 0;
						html_editor().color_border.alpha = 0;

						// close the widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets_impl::widget_type::html_editor, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			}
		}

		/// If the application receives a WM_SIZE message, this method resizes the render target
		/// appropriately
		void form::impl::on_resize(UINT width, UINT height) {
			if (p_render_target_) {
				/// Note: This method can fail, but it's okay to ignore the error here, because the
				/// error will be returned again the next time EndDraw is called
				p_render_target_->Resize(D2D1::SizeU(width, height));
			}
		}

		RECT form::impl::get_working_area(HWND hWnd) {
			HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitorinfo = { 0 };
			monitorinfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &monitorinfo);

			unscale_RECT(monitorinfo.rcWork, dpi_scale_);
			return monitorinfo.rcWork;
		}

		void form::impl::set_position(const float& ix, const float& iy,
			const float& icx, const float& icy) {
			size_.width = icx;
			size_.height = icy;

			// ensure visibility of top left
			point_.x = largest(ix, 0.f);
			point_.y = largest(iy, 0.f);
		}

		void form::impl::set_position(const form_position& wndPos,
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

		bool form::impl::composition_enabled() {
			BOOL composition_enabled = FALSE;
			bool success = ::DwmIsCompositionEnabled(&composition_enabled) == S_OK;
			return composition_enabled && success;
		}

		form::impl::style form::impl::select_borderless_style() {
			return composition_enabled() ? style::aero_borderless : style::basic_borderless;
		}

		void form::impl::set_shadow(HWND hWnd, bool enabled) {
			if (composition_enabled()) {
				static const MARGINS shadow_state[2]{ { 0, 0, 0, 0 },{ 1, 1, 1, 1 } };
				::DwmExtendFrameIntoClientArea(hWnd, &shadow_state[enabled]);
			}
		}

		void form::impl::set_borderless(HWND hWnd, bool enabled) {
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

		void form::impl::set_borderless_shadow(HWND hWnd, bool enabled, bool save_value) {
			if (borderless_) {
				if (save_value)
					borderless_shadow_ = enabled;

				set_shadow(hWnd, enabled);
			}
		}

		bool form::impl::maximized(HWND hwnd) {
			WINDOWPLACEMENT placement = { 0 };
			if (GetWindowPlacement(hwnd, &placement))
				return placement.showCmd == SW_MAXIMIZE;
			else
				return false;
		}

		/// Adjust client rect to not spill over monitor edges when maximized.
		/// rect(in/out): in: proposed window rect, out: calculated client rect
		/// Does nothing if the window is not maximized.
		void form::impl::adjust_maximized_client_rect(HWND window, RECT& rect) {
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

		bool form::impl::destroy_menus() {
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
							widgets_impl::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

							auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

							if (page_iterator != tab_pane.p_tabs_.end())
								helper::check_widgets(page_iterator->second, update);
						}
						else
							if (widget.second.type() ==
								widgets_impl::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									helper::check_widgets(page_iterator->second, update);
							}
					}
				}
			};

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, update);

			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::check_widgets(page_iterator->second, update);

			return update;
		}

		int form::impl::make_unique_id() {
			++unique_id_;
			return unique_id_;
		}

		void form::impl::start_timer(const std::string& alias) {
			if (timers_.find(alias) != timers_.end()) {
				SetTimer(hWnd_, (UINT_PTR)timers_.at(alias).unique_id,
					(UINT)timers_.at(alias).milliseconds, NULL);
				timers_.at(alias).running = true;
			}
		}

		form::impl::widget_search_results
			form::impl::find_widget(containers::page& container,
				const std::string& path) {
			// check if current path has a container
			auto idx = path.find("/");

			if (idx == std::string::npos) {
				try {
					// check if widget is directly in container
					return form::impl::widget_search_results{ container.d_page_.widgets_.at(path), container };
				}
				catch (const std::exception&) {}

				// check if widget is in special pane

				// check special tree pane
				auto& tree_pane_control = container.d_page_.get_pane(widgets_impl::pane::tree_pane_alias_prefix() + path);

				// tree pane control confirmed ... get the pane page
				auto& tree_pane = tree_pane_control.p_panes_.at("pane");

				// pane confirmed ... recurse
				return find_widget(tree_pane, path);
			}
			else {
				// get the container's alias
				const auto container_alias = path.substr(0, idx);

				// get the rest of the path
				const auto path_left = path.substr(idx + 1);

				// check if the container is a tab pane
				try {
					auto& tab_pane = container.d_page_.get_tab_pane(container_alias);

					// tab pane confirmed ... get the tab
					try {
						idx = path_left.find("/");

						if (idx != std::string::npos) {
							// get the tab name
							const auto tab_name = path_left.substr(0, idx);

							// get the rest of the path
							const auto tab_path_left = path_left.substr(idx + 1);

							// get the tab
							auto& tab = tab_pane.p_tabs_.at(tab_name);

							// tab confirmed ... recurse
							return find_widget(tab, tab_path_left);	// recursion
						}
					}
					catch (const std::exception&) {}
				}
				catch (const std::exception&) {}

				// check if the container is a pane
				auto& pane_control = container.d_page_.get_pane(container_alias);

				// pane control confirmed ... get the pane page
				auto& pane = pane_control.p_panes_.at("pane");

				// pane confirmed ... recurse
				return find_widget(pane, path_left);
			}
		}

		containers::page&
			form::impl::find_page(containers::page& container, const std::string& path) {
			auto idx = path.find("/");

			if (path.empty()) {
				return container;
			}
			else {
				if (idx == std::string::npos) {
					// check if the container is a pane
					auto& pane_control = container.d_page_.get_pane(path);

					// pane control confirmed ... get the pane page
					return pane_control.p_panes_.at("pane");
				}
				else {
					// get the container's alias
					const auto container_alias = path.substr(0, idx);

					// get the rest of the path
					const auto path_left = path.substr(idx + 1);

					// check if the container is a tab pane
					try {
						auto& tab_pane = container.d_page_.get_tab_pane(container_alias);

						// tab pane confirmed ... get the tab
						try {
							idx = path_left.find("/");

							if (idx == std::string::npos) {
								// check if this is a tab
								const auto tab_name = path_left;
								return tab_pane.p_tabs_.at(tab_name);
							}
							else {
								// get the tab name
								const auto tab_name = path_left.substr(0, idx);

								// get the rest of the path
								const auto tab_path_left = path_left.substr(idx + 1);

								// get the tab
								auto& tab = tab_pane.p_tabs_.at(tab_name);

								// tab confirmed ... recurse
								return find_page(tab, tab_path_left);	// recursion
							}
						}
						catch (const std::exception&) {}
					}
					catch (const std::exception&) {}

					// check if the container is a pane
					auto& pane_control = container.d_page_.get_pane(container_alias);

					// pane control confirmed ... get the pane page
					auto& pane = pane_control.p_panes_.at("pane");

					// pane confirmed ... recurse
					return find_page(pane, path_left);
				}
			}
		}

		void form::impl::enable(const std::string& path, bool enable) {
			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(p_pages_.at(page_alias), path_remaining);
						result.widget.enable(enable);
						update();
					}
					catch (const std::exception&) {}

					try {
						// check status panes
						auto result = find_widget(p_status_panes_.at(page_alias), path_remaining);
						result.widget.enable(enable);
						update();
					}
					catch (const std::exception&) {}
				}
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

		void form::impl::show(const std::string& path, bool show) {
			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(p_pages_.at(page_alias), path_remaining);
						result.widget.show(show);
						update();
					}
					catch (const std::exception&) {}

					try {
						// check status pages
						auto result = find_widget(p_status_panes_.at(page_alias), path_remaining);
						result.widget.show(show);
						update();
					}
					catch (const std::exception&) {}
				}
			}
			catch (const std::exception&) {}
		}

		void form::impl::close(const std::string& path) {
			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(p_pages_.at(page_alias), path_remaining);

						// close widget
						std::string error;
						result.page.d_page_.close_widget(result.widget.alias(), result.widget.type(), error);

						update();
					}
					catch (const std::exception&) {}

					try {
						// check status pages
						auto result = find_widget(p_status_panes_.at(page_alias), path_remaining);

						// close widget
						std::string error;
						result.page.d_page_.close_widget(result.widget.alias(), result.widget.type(), error);

						update();
					}
					catch (const std::exception&) {}
				}
			}
			catch (const std::exception&) {}
		}

		lecui::size form::impl::get_status_size(containers::status_pane::location type) {
			std::string alias;
			switch (type) {
			case containers::status_pane::location::top: alias = "status::top"; break;
			case containers::status_pane::location::left: alias = "status::left"; break;
			case containers::status_pane::location::right: alias = "status::right"; break;
			case containers::status_pane::location::bottom:
			default: alias = "status::bottom"; break;
			}

			lecui::size size;
			auto it = p_status_panes_.find(alias);
			if (it != p_status_panes_.end()) {
				try {
					if (!p_status_pane_specs_.at(it->first).floating)
						size = it->second.size();
				}
				catch (const std::exception&) {}
			}

			return size;
		}

		LRESULT CALLBACK form::impl::window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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

			case WM_WINDOWPOSCHANGING:
				form_.value().get().d_.on_form_pos_changing(lParam);
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
				// check if it's the parent that now has focus
				if (form_.value().get().d_.menu_form_) {
					if (IsWindow(form_.value().get().d_.hWnd_parent_)) {
						if (GetForegroundWindow() != form_.value().get().d_.hWnd_parent_) {
							// focus lost, but not to parent
							form_.value().get().close();
						}
						else {
							// let parent decide whether to close child in WM_LBUTTONDOWN
						}
					}
					else {
						// focus lost, and there is no parent
						form_.value().get().close();
					}
				}
				return NULL;

			case WM_PAINT:
				form_.value().get().d_.move_trees();
				form_.value().get().d_.move_html_editors();
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
					return form_.value().get().d_.non_client_hittest(
						POINT{ (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) });
				break;

			case WM_NCACTIVATE:
				if (!composition_enabled()) {
					// Prevents window frame reappearing on window activation
					// in "basic" theme, where no aero shadow is present.
					return TRUE;
				}
				break;

			case WM_NCRBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
				for (auto& [key, child] : form_.value().get().d_.m_children_) {
					if (child && IsWindow(child->d_.hWnd_) && child->d_.menu_form_) {
						// close child menu forms
						child->close();
					}
				}

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
					if (it.second.unique_id == unique_id) {
						if (it.second.on_timer)
							it.second.on_timer();

						break;
					}
			} break;

			case WM_CHAR:
				form_.value().get().d_.on_char(wParam);
				break;

			case WM_GETDLGCODE:
				return DLGC_WANTALLKEYS;	// for VK_UP, VK_DOWN to be received in WM_KEYDOWN

			case WM_KEYDOWN:
				form_.value().get().d_.on_keydown(wParam);
				break;

			case WM_KEYUP:
				form_.value().get().d_.on_keyup(wParam);
				break;

			case WM_MOUSEWHEEL:
				form_.value().get().d_.on_wheel(wParam);
				break;

			case WM_MOUSEHWHEEL:
				form_.value().get().d_.on_hwheel(wParam);
				break;

			case WM_DROPFILES:
				form_.value().get().d_.on_dropfiles(wParam);
				break;

			default:
				break;
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}
