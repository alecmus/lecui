//
// form_impl.cpp - form::impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "form_impl.h"

#include "../font/font.h"

#include "../timer.h"

#include "../containers/page.h"
#include "../containers/tab_pane.h"
#include "../containers/pane.h"
#include "../containers/group.h"

#include "../widgets/rectangle.h"
#include "../widgets/label.h"
#include "../widgets/button.h"
#include "../widgets/line.h"
#include "../widgets/h_scrollbar/h_scrollbar_impl.h"
#include "../widgets/v_scrollbar/v_scrollbar_impl.h"

#include "../utilities/color_picker.h"
#include "../utilities/date_time.h"
#include "../menus/context_menu.h"

#include "../command_line_parser.h"

// Windows headers
#include <windowsx.h>
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
		limit_single_instance* form::impl::p_instance_ = nullptr;

		form::impl::impl(form& fm, const std::string& caption_formatted) :
			fm_(fm),
			p_parent_(nullptr),
			menu_form_(caption_formatted == form::menu_form_caption()),
			tooltip_form_(caption_formatted == form::tooltip_form_caption()),
			parent_closing_(false),
			show_called_(false),
			reg_id_(0),
			receiving_(false),
			data_received_(std::string()),
			caption_bar_height_(menu_form_ || tooltip_form_ ? 0.f : 30.f),
			caption_and_menu_gap_(25.f),
			form_menu_margin_(10.f),
			form_border_thickness_(1.f),
			page_tolerance_(form_border_thickness_ / 2.f),
			control_button_margin_(2.f),
			receive_data_timer_alias_("liblec::lecui::receive_data_timer"),
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
			dpi_scale_(get_process_dpi()),
			borderless_(true),
			borderless_shadow_(true),
			shadow_setting_before_maximize_(borderless_shadow_),
			p_render_target_(nullptr),
			p_brush_theme_(nullptr),
			p_brush_theme_hot_(nullptr),
			p_brush_theme_disabled_(nullptr),
			p_brush_titlebar_(nullptr),
			current_page_(std::string()),
			controls_page_(fm, ""),
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
			h_widget_cursor_(nullptr),
			schedule_refresh_(false),
			close_called_(false),
			force_instance_(false),
			tray_icon_present_(false) {
			++instances_;	// increment instances count

			/// Use HeapSetInformation to specify that the process should terminate if the heap manager
			/// detects an error in any heap used by the process. The return value is ignored, because
			/// we want to continue running in the unlikely event that HeapSetInformation fails.
			HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

			// parse the caption
			// the default color doesn't matter here we're just getting the plain text
			std::vector<formatted_text_parser::text_range_properties> formatting;
			widgets::parse_formatted_text(caption_formatted_, caption_plain_,
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

				if (p_instance_) {
					delete p_instance_;
					p_instance_ = nullptr;
				}

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
		/// widgets::widget::resources_created_ flag (this flag is
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
									widgets::widget_type::tab_pane) {
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
										widgets::widget_type::pane) {
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
							widgets::widget_type::custom) {
							log("Skipping custom widget discard resources because by now custom widget will have long been destroyed so don't discard resources");
							continue;
						}

						widget.second.discard_resources();

						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
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
								widgets::widget_type::pane) {
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
				std::unique_ptr<widgets::close_button_impl>(new
					widgets::close_button_impl(controls_page_));
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
				std::unique_ptr<widgets::maximize_button_impl>(new
					widgets::maximize_button_impl(controls_page_));
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
				std::unique_ptr<widgets::minimize_button_impl>(new
					widgets::minimize_button_impl(controls_page_));
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
				std::unique_ptr<widgets::label_impl>(new
					widgets::label_impl(controls_page_, "form_caption",
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
			const auto rect = widgets::measure_label(p_directwrite_factory_,
				p_caption_->specs().text, p_caption_->specs().font, p_caption_->specs().font_size,
				p_caption_->specs().center_h, p_caption_->specs().center_v, max_rect);

			// to-do: address this ... when form is downsized enough, caption creeps behind control
			// buttons since we've eliminated resizing caption rect; perhaps use clip???
			p_caption_->specs().rect = convert_rect(rect);

			p_caption_->specs().events().action = on_caption_;
		}

		void form::impl::create_form_menu() {
			float left = p_caption_->specs().rect.right + caption_and_menu_gap_;

			for (auto& main_menu_item : form_menu_) {
				p_menu_.push_back(
					std::unique_ptr<widgets::label_impl>(new
						widgets::label_impl(controls_page_, "menu::" + main_menu_item.text,
							p_directwrite_factory_)));
				auto& it = p_menu_.back();

				widgets_.emplace(it->alias(), *it);
				widgets_order_.emplace_back(it->alias());

				it->specs().text = main_menu_item.text;
				it->specs().center_v = true;
				it->specs().multiline = false;

				// load label settings
				it->specs().color_text = defaults::color(theme_, item::label);
				it->specs().color_selected = defaults::color(theme_, item::label_selected);
				it->specs().color_hot = defaults::color(theme_, item::label_hover);
				it->specs().color_hot_pressed = defaults::color(theme_, item::label_pressed);
				it->specs().color_disabled = defaults::color(theme_, item::label_disabled);

				// determine right-most edge based on available control buttons
				const auto right_edge = allow_minimize_ ?
					p_minimize_button_->specs().rect.left :
					(allow_resizing_ ?
						p_maximize_button_->specs().rect.left :
						p_close_button_->specs().rect.left);

				// determine the largest rect that the caption can occupy
				const D2D1_RECT_F max_rect = D2D1::RectF(left, control_button_margin_,
					right_edge - control_button_margin_, caption_bar_height_ - control_button_margin_);

				// determine the optimal rect for the caption
				const auto rect = widgets::measure_label(p_directwrite_factory_,
					it->specs().text, it->specs().font, it->specs().font_size,
					it->specs().center_h, it->specs().center_v, max_rect);

				// to-do: address this ... when form is downsized enough, caption creeps behind control
				// buttons since we've eliminated resizing caption rect; perhaps use clip???
				it->specs().rect = convert_rect(rect);

				main_menu_item.rc_text = it->specs().rect;

				left = it->specs().rect.right + form_menu_margin_;

				it->specs().events().action = [&]() {
					if (main_menu_item.items.empty())
						return;

					context_menu::specs menu_specs;
					menu_specs.quality = image_quality::high;

					for (const auto& item : main_menu_item.items) {
						menu_item mi;
						mi.label = item.label;
						mi.font = item.font;
						mi.font_size = item.font_size;
						menu_specs.items.push_back(mi);
					}

					menu_specs.pin = main_menu_item.rc_text;
					menu_specs.type = context_menu::pin_type::bottom;

					POINT pt = { 0, 0 };
					ClientToScreen(hWnd_, &pt);

					menu_specs.pin.left += (pt.x / get_dpi_scale());
					menu_specs.pin.right += (pt.x / get_dpi_scale());
					menu_specs.pin.top += (pt.y / get_dpi_scale());
					menu_specs.pin.bottom += (pt.y / get_dpi_scale());

					auto result = context_menu()(fm_, menu_specs);
					
					for (auto& m_it : main_menu_item.items) {
						if (m_it.label == result) {
							if (m_it.action != nullptr)
								m_it.action();
						}
					}
				};
			}
		}

		void form::impl::update() { InvalidateRect(hWnd_, nullptr, FALSE); }

		/// The tree view widget is constructed as follows:
		/// 1. A special pane is made for carrying the tree view
		/// 2. The tree view widget is moved into the container
		void form::impl::move_trees() {
			// check if this page has a tree pane
			auto page_iterator = p_pages_.find(current_page_);

			struct tree_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::tree_view::tree_view_specs tree;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				containers::pane::pane_specs& tree_pane_specs;
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
							if (widget.first.find(widgets::pane_impl::tree_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a tree pane (it has a tree inside. move was already done), continue to next widget

							// check if this is a tree
							if (widget.second.type() == widgets::widget_type::tree_view) {
								// this is a tree, we need to "move" it into a special pane

								// get the tree specs
								auto& tree_specs = page.d_page_.get_tree(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::tree_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = tree_specs.rect;
								pane().on_resize = tree_specs.on_resize;
								pane().color_fill = tree_specs.color_fill;
								pane().color_border = tree_specs.color_border;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								trees.push_back({ widget.first, tree_specs, page, pane.get(), pane() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_trees_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
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
						widgets::tree_view tree(it.destination, it.alias);
						// copy specs
						tree() = it.tree;

						// adjust specs
						tree().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						tree().on_resize = { 0, 0, 0, 0 };	// critical because tree will change size as tree is browsed or changed. the pane scroll bars will do the job.

						// capture tree pane specs
						it.destination.d_page_.get_tree(it.alias).set_tree_pane_specs(it.tree_pane_specs);

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::tree_view, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			}
		}

		/// The html editor widget is constructed as follows:
		/// 1. A special pane is made for carrying the html controls
		/// 2. A special pane is made for carrying the html text
		/// 3. The html widget is moved into the html container
		/// 4. Control widgets are added to the control pane
		void form::impl::move_html_editors() {
			// check if this page has an html widget
			auto page_iterator = p_pages_.find(current_page_);

			struct html_editor_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::html_editor::html_editor_specs html_editor;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				containers::pane::pane_specs& html_control_pane_specs;
				containers::pane::pane_specs& html_pane_specs;
			};

			std::vector<html_editor_info> html_editors;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class move_helper {
				public:
					static void find_html_editors_to_move(lecui::containers::page& page,
						std::vector<html_editor_info>& trees) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is an html pane
							if (widget.first.find(widgets::pane_impl::html_pane_alias_prefix()) != std::string::npos)
								continue;	// this is an html pane (it has an html widget inside. move was already done), continue to next widget

							// check if this is an html editor
							if (widget.second.type() == widgets::widget_type::html_editor) {
								// this is an html editor, we need to "move" it into a special pane

								// get the html editor specs
								auto& html_editor_specs = page.d_page_.get_html_editor(widget.first).specs();

								// make controls pane in source (predefined, fixed height)
								containers::pane controls_pane(page, widgets::pane_impl::html_controls_pane_alias_prefix() + widget.first);
								controls_pane().rect = html_editor_specs.rect;
								controls_pane().rect.height(
									(10.f * 2) +	// top and bottom margin
									25.f +			// first row (font name, font size ...)
									5.f + 20.f		// seond row (bold, italic ...)
									);
								controls_pane().on_resize = html_editor_specs.on_resize;
								controls_pane().on_resize.perc_height = 0.f;
								controls_pane().on_resize.min_height = 0.f;
								controls_pane().color_fill = html_editor_specs.color_control_fill;
								controls_pane().color_border = html_editor_specs.color_control_border;

								// cause controls pane to be initialized by calling get()
								auto& controls_pane_page = controls_pane.get();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::html_pane_alias_prefix() + widget.first);

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
								trees.push_back({ widget.first, html_editor_specs, page, pane.get(), controls_pane(), pane() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_html_editors_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_html_editors_to_move(page.second, trees);	// recursion
								}
						}
					}
				};

				move_helper::find_html_editors_to_move(page, html_editors);

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

						// capture html pane specs
						it.destination.d_page_.get_html_editor(it.alias).set_pane_specs(it.html_control_pane_specs, it.html_pane_specs);

						// close the widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::html_editor, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_html_controls(lecui::containers::page& page) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.first.find(widgets::pane_impl::html_controls_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated html editor widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get the pages
										auto& html_controls_page = page.d_page_.get_pane(widgets::pane_impl::html_controls_pane_alias_prefix() + widget_alias).p_panes_.at("pane");
										auto& html_page = page.d_page_.get_pane(widgets::pane_impl::html_pane_alias_prefix() + widget_alias).p_panes_.at("pane");

										// get the html editor and controls
										auto& html_editor = html_page.d_page_.get_html_editor(widget_alias);

										if (!html_editor.controls_initialized()) {
											/// add controls to controls pane
											
											/// add basic font formatting
											
											/// add font selection combobox
											widgets::combobox font(html_controls_page, html_editor.alias_font());
											font().rect = { 0.f, 150.f, 0.f, 25.f };
											font().editable = false;

											// get list of fonts
											const auto& font_list = font::available_fonts();

											for (const auto& font_name : font_list) {
												widgets::combobox::combobox_item item;
												item.label = font_name;
												item.font = font_name;	// essential for preview
												font().items.push_back(item);
											}

											font().selected = "Calibri";
											font().events().selection = [&](const std::string& font_name) {
												html_editor.selection_font(font_name);
											};
											
											/// add bold control
											widgets::rectangle bold(html_controls_page, html_editor.alias_bold());
											bold().rect.size(20.f, 20.f);
											bold().rect.snap_to(font().rect, rect::snap_type::bottom_left, 5.f);
											bold().color_fill.alpha = 0;
											bold().color_border.alpha = 0;
											bold().events().action = [&]() {
												html_editor.selection_bold();
											};

											widgets::label bold_label(html_controls_page);
											bold_label().rect = bold().rect;
											bold_label().text = "<strong>B</strong>";
											bold_label().font_size = 11.f;
											bold_label().center_h = true;
											bold_label().center_v = true;

											/// add italic control
											widgets::rectangle italic(html_controls_page, html_editor.alias_italic());
											italic().rect.size(20.f, 20.f);
											italic().rect.snap_to(bold().rect, rect::snap_type::right, 5.f);
											italic().color_fill.alpha = 0;
											italic().color_border.alpha = 0;
											italic().events().action = [&]() {
												html_editor.selection_italic();
											};

											widgets::label italic_label(html_controls_page);
											italic_label().rect = italic().rect;
											italic_label().text = "<em><strong>I</strong></em>";
											italic_label().font_size = 11.f;
											italic_label().center_h = true;
											italic_label().center_v = true;

											/// add underline control
											widgets::rectangle underline(html_controls_page, html_editor.alias_underline());
											underline().rect.size(20.f, 20.f);
											underline().rect.snap_to(italic_label().rect, rect::snap_type::right, 5.f);
											underline().color_fill.alpha = 0;
											underline().color_border.alpha = 0;
											underline().events().action = [&]() {
												html_editor.selection_underline();
											};

											widgets::label underline_label(html_controls_page);
											underline_label().rect = underline().rect;
											underline_label().text = "<u>U</u>";
											underline_label().font_size = 11.f;
											underline_label().center_h = true;
											underline_label().center_v = true;

											/// add strikethrough control
											widgets::rectangle strikethrough(html_controls_page, html_editor.alias_strikethrough());
											strikethrough().rect.size(30.f, 20.f);
											strikethrough().rect.snap_to(underline_label().rect, rect::snap_type::right, 5.f);
											strikethrough().color_fill.alpha = 0;
											strikethrough().color_border.alpha = 0;
											strikethrough().events().action = [&]() {
												html_editor.selection_strikethrough();
											};

											widgets::label strikethrough_label(html_controls_page);
											strikethrough_label().rect = strikethrough().rect;
											strikethrough_label().text = "<s>abc</s>";
											strikethrough_label().font_size = 11.f;
											strikethrough_label().center_h = true;
											strikethrough_label().center_v = true;

											/// add color control
											widgets::rectangle font_color(html_controls_page, html_editor.alias_font_color());
											font_color().rect.size(20.f, 20.f);
											font_color().rect.snap_to(strikethrough().rect, rect::snap_type::right, 5.f);
											font_color().color_fill.alpha = 0;
											font_color().color_border.alpha = 0;
											font_color().events().action = [&]() {
												html_editor.selection_color();
											};

											widgets::label font_color_label(html_controls_page);
											font_color_label().rect = font_color().rect;
											font_color_label().rect.bottom -= 5.f;
											font_color_label().text = "<strong>A</strong>";
											font_color_label().font_size = 9.5f;
											font_color_label().center_h = true;
											font_color_label().center_v = true;

											widgets::rectangle font_color_bar(html_controls_page, html_editor.alias_font_color_bar());
											font_color_bar().rect = font_color().rect;
											font_color_bar().rect.top = font_color_label().rect.bottom;
											font_color_bar().rect.bottom -= 1.f;
											font_color_bar().rect.left += 2.f;
											font_color_bar().rect.right -= 2.f;
											font_color_bar().color_fill = html_editor.get_last_color();
											font_color_bar().border = .2f;

											widgets::rectangle font_color_menu(html_controls_page);
											font_color_menu().rect.size(10.f, 20.f);
											font_color_menu().rect.snap_to(font_color().rect, rect::snap_type::right, 2.f);
											font_color_menu().color_fill.alpha = 0;
											font_color_menu().color_border.alpha = 0;
											font_color_menu().events().action = [&]() {
												color font_color;
												if (color_picker(page.d_page_.fm_).pick(font_color)) {
													html_editor.selection_color(font_color);

													try {
														// update font color bar
														auto& font_color_bar_impl = html_controls_page.d_page_.get_rectangle(html_editor.alias_font_color_bar());
														font_color_bar_impl.specs().color_fill = font_color;
													}
													catch (const std::exception&) {}

													page.d_page_.fm_.update();
												}
											};

											widgets::line font_color_menu_dropdown(html_controls_page);
											font_color_menu_dropdown().rect.size(font_color_menu().rect.width(), font_color_menu().rect.width());	// important for it to be a square
											font_color_menu_dropdown().rect.place(font_color_menu().rect, 50.f, 50.f);
											font_color_menu_dropdown().points.push_back({ .1f * font_color_menu_dropdown().rect.width(), .3f * font_color_menu_dropdown().rect.height() });
											font_color_menu_dropdown().points.push_back({ .5f * font_color_menu_dropdown().rect.width(), .7f * font_color_menu_dropdown().rect.height() });
											font_color_menu_dropdown().points.push_back({ .9f * font_color_menu_dropdown().rect.width(), .3f * font_color_menu_dropdown().rect.height() });

											/// add font size combobox
											widgets::combobox font_size(html_controls_page, html_editor.alias_font_size());
											font_size().rect.size(70.f, 25.f);
											font_size().rect.snap_to(font().rect, rect::snap_type::right, 10.f);
											font_size().editable = true;
											font_size().force_numerical_sort = true;
											font_size().items = {
												{"8", "Segoe UI", 8 },
												{"9", "Segoe UI", 9},
												{"10", "Segoe UI", 10},
												{"11", "Segoe UI", 11},
												{"12", "Segoe UI", 12},
												{"13", "Segoe UI", 13},
												{"14", "Segoe UI", 14},
												{"18", "Segoe UI", 18},
												{"24", "Segoe UI", 24},
												{"36", "Segoe UI", 36} };
											font_size().selected = "11";
											font_size().events().selection = [&](const std::string& font_size) {
												float font_size_ = 0.f;
												std::stringstream ss;
												ss << font_size;
												ss >> font_size_;
												html_editor.selection_font_size(font_size_);
											};

											/// increase font size control

											html_editor.initialize_controls(true);
										}
									}
								}
								catch (const std::exception& e) { log(e.what()); }
							}
							else
								if (widget.second.type() == widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane.p_tabs_)
										add_html_controls(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										// initialize panes
										for (auto& page : pane.p_panes_)
											add_html_controls(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_html_controls(page);
			}
		}

		/// The time widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. Three rectangles are added to the pane, these are for the backgrounds and borders
		/// of the hour, minute and second
		/// 3. Three labels are added one above each rectangle. These are for displaying the
		/// digits
		/// 4. A label with a colon is placed between each rectangle for use as seperators
		/// 5. The rectangles are used for hit testing, and each has a handler that brings up a
		/// context menu for editing the corresponding time component
		void form::impl::move_times() {
			// check if this page has a time widget
			auto page_iterator = p_pages_.find(current_page_);

			struct time_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::time::time_specs time;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<time_info> times;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_times_to_move(lecui::containers::page& page,
						std::vector<time_info>& times) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is a time pane
							if (widget.first.find(widgets::pane_impl::time_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a time pane (it has a time widget inside. move was already done), continue to next widget

							// check if this is a time widget
							if (widget.second.type() == widgets::widget_type::time) {
								// this is a time widget, we need to "move" it into a special pane

								// get the time specs
								auto& time_specs = page.d_page_.get_time(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::time_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = time_specs.rect;
								pane().on_resize = time_specs.on_resize;
								pane().color_fill.alpha = 0;
								pane().color_border.alpha = 0;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								times.push_back({ widget.first, time_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_times_to_move(tab.second, times);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_times_to_move(page.second, times);	// recursion
								}
						}
					}
				};

				helper::find_times_to_move(page, times);

				// move times
				for (auto& it : times) {
					log("moving time: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::time time(it.destination, it.alias);
						// copy specs
						time() = it.time;

						// adjust specs
						time().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						time().on_resize = { 0, 0, 0, 0 };

						// add hour destination
						widgets::rectangle hour(it.destination, widgets::time_impl::alias_hour());
						hour().rect = { 0, 18, 0, 20 };
						hour().on_resize = { 0, 0, 0, 0 };
						hour().corner_radius_x = time().corner_radius_x;
						hour().corner_radius_y = time().corner_radius_y;
						hour().border = time().border;
						hour().color_fill = time().color_fill;
						hour().color_border = time().color_border;
						hour().color_disabled = time().color_disabled;
						hour().color_selected = time().color_selected;

						widgets::label hour_label(it.destination, widgets::time_impl::alias_hour_label());
						hour_label().rect = hour().rect;
						hour_label().center_h = true;
						hour_label().center_v = true;
						hour_label().on_resize = { 0, 0, 0, 0 };
						hour_label().color_text = time().color_text;
						hour_label().text = time().time_value.hour < 10 ? "0" + std::to_string(time().time_value.hour) :
							std::to_string(time().time_value.hour);

						// add seperator to destination
						widgets::label seperator_1(it.destination);
						seperator_1().rect = { 0, 8, 0, 20 };
						seperator_1().rect.snap_to(hour().rect, rect::snap_type::right, 0.f);
						seperator_1().on_resize = { 0, 0, 0, 0 };
						seperator_1().text = ":";
						seperator_1().color_text = time().color_text;
						seperator_1().center_h = true;
						seperator_1().center_v = true;

						// add minute to destination
						widgets::rectangle minute(it.destination, widgets::time_impl::alias_minute());
						minute().rect = { 0, 18, 0, 20 };
						minute().rect.snap_to(seperator_1().rect, rect::snap_type::right, 0.f);
						minute().corner_radius_x = time().corner_radius_x;
						minute().corner_radius_y = time().corner_radius_y;
						minute().border = time().border;
						minute().color_fill = time().color_fill;
						minute().color_border = time().color_border;
						minute().color_disabled = time().color_disabled;
						minute().color_selected = time().color_selected;

						widgets::label minute_label(it.destination, widgets::time_impl::alias_minute_label());
						minute_label().rect = minute().rect;
						minute_label().center_h = true;
						minute_label().center_v = true;
						minute_label().on_resize = { 0, 0, 0, 0 };
						minute_label().color_text = time().color_text;
						minute_label().text = time().time_value.minute < 10 ? "0" + std::to_string(time().time_value.minute) :
							std::to_string(time().time_value.minute);

						// add seperator to destination
						widgets::label seperator_2(it.destination);
						seperator_2().rect = { 0, 8, 0, 20 };
						seperator_2().rect.snap_to(minute().rect, rect::snap_type::right, 0.f);
						seperator_2().on_resize = { 0, 0, 0, 0 };
						seperator_2().text = ":";
						seperator_2().color_text = time().color_text;
						seperator_2().center_h = true;
						seperator_2().center_v = true;

						// add second to destination
						widgets::rectangle second(it.destination, widgets::time_impl::alias_second());
						second().rect = { 0, 18, 0, 20 };
						second().rect.snap_to(seperator_2().rect, rect::snap_type::right, 0.f);
						second().corner_radius_x = time().corner_radius_x;
						second().corner_radius_y = time().corner_radius_y;
						second().border = time().border;
						second().color_fill = time().color_fill;
						second().color_border = time().color_border;
						second().color_disabled = time().color_disabled;
						second().color_selected = time().color_selected;

						widgets::label second_label(it.destination, widgets::time_impl::alias_second_label());
						second_label().rect = second().rect;
						second_label().center_h = true;
						second_label().center_v = true;
						second_label().on_resize = { 0, 0, 0, 0 };
						second_label().color_text = time().color_text;
						second_label().text = time().time_value.second < 10 ? "0" + std::to_string(time().time_value.second) :
							std::to_string(time().time_value.second);

						// capture time label specs
						it.destination.d_page_.get_time(it.alias).set_time_label_specs(
							hour_label(), seperator_1(), minute_label(), seperator_2(), second_label());

						// capture rectangle specs
						it.destination.d_page_.get_time(it.alias).set_time_specs(
							hour(), minute(), second());

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::time, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_times(lecui::containers::page& page) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.first.find(widgets::pane_impl::time_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated time widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get time pane
										auto& time_page = page.d_page_.get_pane(widgets::pane_impl::time_pane_alias_prefix() + widget_alias).p_panes_.at("pane");

										// get time widget specs
										auto& specs = time_page.d_page_.get_time(widget_alias).specs();

										// get hour
										auto& hour = time_page.d_page_.get_rectangle(widgets::time_impl::alias_hour());
										auto& hour_lbl = time_page.d_page_.get_label(widgets::time_impl::alias_hour_label());

										if (hour().events().action == nullptr) {
											hour().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												for (int i = 0; i < 24; i++) {
													std::string hr = std::to_string(i);
													if (i < 10)
														hr = "0" + hr;
													menu_specs.items.push_back({ hr });
												}

												auto selected = context_menu()(time_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													hour_lbl().text = selected;
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value.hour;

													if (specs.events().change)
														specs.events().change(specs.time_value);
												}
											};
										}

										// get minute label
										auto& minute = time_page.d_page_.get_rectangle(widgets::time_impl::alias_minute());
										auto& minute_lbl = time_page.d_page_.get_label(widgets::time_impl::alias_minute_label());

										if (minute().events().action == nullptr) {
											minute().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												for (int i = 0; i < 60; i++) {
													std::string mn = std::to_string(i);
													if (i < 10)
														mn = "0" + mn;
													menu_specs.items.push_back({ mn });
												}

												auto selected = context_menu()(time_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													minute_lbl().text = selected;
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value.minute;

													if (specs.events().change)
														specs.events().change(specs.time_value);
												}
											};
										}

										// get second label
										auto& second = time_page.d_page_.get_rectangle(widgets::time_impl::alias_second());
										auto& second_lbl = time_page.d_page_.get_label(widgets::time_impl::alias_second_label());

										if (second().events().action == nullptr) {
											second().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												for (int i = 0; i < 60; i++) {
													std::string mn = std::to_string(i);
													if (i < 10)
														mn = "0" + mn;
													menu_specs.items.push_back({ mn });
												}

												auto selected = context_menu()(time_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													second_lbl().text = selected;
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value.second;

													if (specs.events().change)
														specs.events().change(specs.time_value);
												}
											};
										}
									}
								}
								catch (const std::exception& e) { log(e.what()); }
							}
							else
								if (widget.second.type() == widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane.p_tabs_)
										add_times(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										// initialize panes
										for (auto& page : pane.p_panes_)
											add_times(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_times(page);
			}
		}

		/// The date widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. A label is added to the pane, for displaying the week day
		/// 3. Three rectangles are added to the pane, these are for the backgrounds and borders
		/// of the day, month and year
		/// 4. Three labels are added one above each rectangle. These are for displaying the
		/// day, month and year
		/// 5. A label with a dash is placed between each rectangle for use as seperators
		/// 6. The rectangles are used for hit testing, and each has a handler that brings up a
		/// context menu for editing the corresponding date component
		void form::impl::move_dates() {
			// check if this page has a date widget
			auto page_iterator = p_pages_.find(current_page_);

			struct date_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::date::date_specs date;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<date_info> dates;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_dates_to_move(lecui::containers::page& page,
						std::vector<date_info>& dates) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is a date pane
							if (widget.first.find(widgets::pane_impl::date_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a date pane (it has a date widget inside. move was already done), continue to next widget

							// check if this is a date widget
							if (widget.second.type() == widgets::widget_type::date) {
								// this is a date widget, we need to "move" it into a special pane

								// get the date specs
								auto& date_specs = page.d_page_.get_date(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::date_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = date_specs.rect;
								pane().on_resize = date_specs.on_resize;
								pane().color_fill.alpha = 0;
								pane().color_border.alpha = 0;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								dates.push_back({ widget.first, date_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_dates_to_move(tab.second, dates);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_dates_to_move(page.second, dates);	// recursion
								}
						}
					}
				};

				helper::find_dates_to_move(page, dates);

				// move dates
				for (auto& it : dates) {
					log("moving date: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::date date(it.destination, it.alias);
						// copy specs
						date() = it.date;

						// impose limits
						date().date_value.year = largest(date().date_value.year, (unsigned short)1970);

						// adjust specs
						date().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						date().on_resize = { 0, 0, 0, 0 };
						date().color_fill.alpha = 0;

						// add week day to destination
						widgets::label weekday_label(it.destination, widgets::date_impl::alias_weekday_label());
						weekday_label().rect = { 0, 90, 0, 20 };
						weekday_label().on_resize = { 0, 0, 0, 0 };
						weekday_label().color_text = date().color_text;
						weekday_label().text = date_time::weekday_to_string(date_time::day_of_week(date().date_value));
						weekday_label().font_size = 8.5f;	// to-do: eliminate magic number

						// add day to destination
						widgets::rectangle day(it.destination, widgets::date_impl::alias_day());
						day().rect = { 0, 18, 20, 20 + 20 };
						day().on_resize = { 0, 0, 0, 0 };
						day().corner_radius_x = date().corner_radius_x;
						day().corner_radius_y = date().corner_radius_y;
						day().border = date().border;
						day().color_fill = date().color_fill;
						day().color_border = date().color_border;
						day().color_disabled = date().color_disabled;
						day().color_selected = date().color_selected;

						widgets::label day_label(it.destination, widgets::date_impl::alias_day_label());
						day_label().rect = day().rect;
						day_label().center_h = true;
						day_label().center_v = true;
						day_label().on_resize = { 0, 0, 0, 0 };
						day_label().color_text = date().color_text;
						day_label().text = date().date_value.day < 10 ? "0" + std::to_string(date().date_value.day) :
							std::to_string(date().date_value.day);

						// add seperator to destination
						widgets::label seperator_1(it.destination);
						seperator_1().rect = { 0, 8, 0, 20 };
						seperator_1().rect.snap_to(day().rect, rect::snap_type::right, 0.f);
						seperator_1().on_resize = { 0, 0, 0, 0 };
						seperator_1().text = "-";
						seperator_1().color_text = date().color_text;
						seperator_1().center_h = true;
						seperator_1().center_v = true;

						// add month to destination
						widgets::rectangle month(it.destination, widgets::date_impl::alias_month());
						month().rect = { 0, 25, 0, 20 };
						month().rect.snap_to(seperator_1().rect, rect::snap_type::right, 0.f);
						month().corner_radius_x = date().corner_radius_x;
						month().corner_radius_y = date().corner_radius_y;
						month().border = date().border;
						month().color_fill = date().color_fill;
						month().color_border = date().color_border;
						month().color_disabled = date().color_disabled;
						month().color_selected = date().color_selected;

						widgets::label month_label(it.destination, widgets::date_impl::alias_month_label());
						month_label().rect = month().rect;
						month_label().center_h = true;
						month_label().center_v = true;
						month_label().on_resize = { 0, 0, 0, 0 };
						month_label().color_text = date().color_text;
						month_label().text = date_time::month_to_string(date().date_value.month);

						// add seperator to destination
						widgets::label seperator_2(it.destination);
						seperator_2().rect = { 0, 8, 0, 20 };
						seperator_2().rect.snap_to(month().rect, rect::snap_type::right, 0.f);
						seperator_2().on_resize = { 0, 0, 0, 0 };
						seperator_2().text = "-";
						seperator_2().color_text = date().color_text;
						seperator_2().center_h = true;
						seperator_2().center_v = true;

						// add year to destination
						widgets::rectangle year(it.destination, widgets::date_impl::alias_year());
						year().rect = { 0, 31, 0, 20 };
						year().rect.snap_to(seperator_2().rect, rect::snap_type::right, 0.f);
						year().corner_radius_x = date().corner_radius_x;
						year().corner_radius_y = date().corner_radius_y;
						year().border = date().border;
						year().color_fill = date().color_fill;
						year().color_border = date().color_border;
						year().color_disabled = date().color_disabled;
						year().color_selected = date().color_selected;

						widgets::label year_label(it.destination, widgets::date_impl::alias_year_label());
						year_label().rect = year().rect;
						year_label().center_h = true;
						year_label().center_v = true;
						year_label().on_resize = { 0, 0, 0, 0 };
						year_label().color_text = date().color_text;
						year_label().text = std::to_string(date().date_value.year);

						// capture date label specs
						it.destination.d_page_.get_date(it.alias).set_date_label_specs(
							weekday_label(), day_label(), seperator_1(), month_label(), seperator_2(), year_label());

						// capture rectangle specs
						it.destination.d_page_.get_date(it.alias).set_date_specs(
							day(), month(), year());

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::date, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_dates(lecui::containers::page& page) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.first.find(widgets::pane_impl::date_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated date widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get date pane
										auto& date_page = page.d_page_.get_pane(widgets::pane_impl::date_pane_alias_prefix() + widget_alias).p_panes_.at("pane");

										// get date widget specs
										auto& specs = date_page.d_page_.get_date(widget_alias).specs();

										// get weekday label
										auto& weekday_lbl = date_page.d_page_.get_label(widgets::date_impl::alias_weekday_label());

										// get day
										auto& day = date_page.d_page_.get_rectangle(widgets::date_impl::alias_day());
										auto& day_lbl = date_page.d_page_.get_label(widgets::date_impl::alias_day_label());

										if (day().events().action == nullptr) {
											day().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												for (int i = 1; i < date_time::last_day_of_month(specs.date_value).day + 1; i++) {
													std::string dy = std::to_string(i);
													if (i < 10)
														dy = "0" + dy;
													menu_specs.items.push_back({ dy });
												}

												auto selected = context_menu()(date_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													day_lbl().text = selected;
													std::stringstream ss;
													ss << selected;
													ss >> specs.date_value.day;

													// update weekday
													weekday_lbl().text = date_time::weekday_to_string(date_time::day_of_week(specs.date_value));

													if (specs.events().change)
														specs.events().change(specs.date_value);
												}
											};
										}

										// get month label
										auto& month = date_page.d_page_.get_rectangle(widgets::date_impl::alias_month());
										auto& month_lbl = date_page.d_page_.get_label(widgets::date_impl::alias_month_label());

										if (month().events().action == nullptr) {
											month().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												menu_specs.items = {
													{ "Jan" },
													{ "Feb" },
													{ "Mar" },
													{ "Apr" },
													{ "May" },
													{ "Jun" },
													{ "Jul" },
													{ "Aug" },
													{ "Sep" },
													{ "Oct" },
													{ "Nov" },
													{ "Dec" }
												};

												auto selected = context_menu()(date_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													month_lbl().text = selected;
													specs.date_value.month = date_time::month_from_string(selected);

													// ensure day is within range
													specs.date_value.day = smallest(specs.date_value.day,
														date_time::last_day_of_month({ 1, specs.date_value.month, specs.date_value.year }).day);
													day_lbl().text = std::to_string(specs.date_value.day);
													for (size_t i = day_lbl().text.length(); i < 2; i++) day_lbl().text = "0" + day_lbl().text;

													// update weekday
													weekday_lbl().text = date_time::weekday_to_string(date_time::day_of_week(specs.date_value));

													if (specs.events().change)
														specs.events().change(specs.date_value);
												}
											};
										}

										// get year label
										auto& year = date_page.d_page_.get_rectangle(widgets::date_impl::alias_year());
										auto& year_lbl = date_page.d_page_.get_label(widgets::date_impl::alias_year_label());

										if (year().events().action == nullptr) {
											year().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												// to-do: make possible to use year prior to 1970 (mktime issue)
												for (int i = 1970; i < 2100; i++) {
													std::string yr = std::to_string(i);
													menu_specs.items.push_back({ yr });
												}

												auto selected = context_menu()(date_page.d_page_.get_form(), menu_specs);

												if (!selected.empty()) {
													year_lbl().text = selected;
													std::stringstream ss;
													ss << selected;
													ss >> specs.date_value.year;

													// ensure day is within range
													specs.date_value.day = smallest(specs.date_value.day,
														date_time::last_day_of_month({ 1, specs.date_value.month, specs.date_value.year }).day);
													day_lbl().text = std::to_string(specs.date_value.day);
													for (size_t i = day_lbl().text.length(); i < 2; i++) day_lbl().text = "0" + day_lbl().text;

													// update weekday
													weekday_lbl().text = date_time::weekday_to_string(date_time::day_of_week(specs.date_value));

													if (specs.events().change)
														specs.events().change(specs.date_value);
												}
											};
										}
									}
								}
								catch (const std::exception& e) { log(e.what()); }
							}
							else
								if (widget.second.type() == widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane.p_tabs_)
										add_dates(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										// initialize panes
										for (auto& page : pane.p_panes_)
											add_dates(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_dates(page);
			}
		}

		/// The icon widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. A rectangle is placed into the pane, this is for hit testing
		/// 3. An image view is added above the rectangle
		/// 4. Two labels are added beside the image view, one is the icon text and the other is
		/// the descriptive text
		void form::impl::move_icons() {
			// check if this page has a icon widget
			auto page_iterator = p_pages_.find(current_page_);

			struct icon_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::icon::icon_specs icon;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<icon_info> icons;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_icons_to_move(lecui::containers::page& page,
						std::vector<icon_info>& icons) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is a icon pane
							if (widget.first.find(widgets::pane_impl::icon_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a icon pane (it has a icon widget inside. move was already done), continue to next widget

							// check if this is a icon widget
							if (widget.second.type() == widgets::widget_type::icon) {
								// this is a icon widget, we need to "move" it into a special pane

								// get the icon specs
								auto& icon_specs = page.d_page_.get_icon(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::icon_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = icon_specs.rect;
								pane().on_resize = icon_specs.on_resize;
								pane().color_fill.alpha = 0;
								pane().color_border.alpha = 0;

								// save move info so we can move the tree into the pane later
								// we cannot do it here because we're iterating
								icons.push_back({ widget.first, icon_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_icons_to_move(tab.second, icons);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_icons_to_move(page.second, icons);	// recursion
								}
						}
					}
				};

				helper::find_icons_to_move(page, icons);

				// move icons
				for (auto& it : icons) {
					log("moving icon: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::icon icon(it.destination, it.alias);
						// copy specs
						icon() = it.icon;

						// adjust specs
						icon().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						icon().on_resize = { 0, 0, 0, 0 };

						const float gap_ = 5.f;
						const float padding_ = 5.f;

						// add rectangle to destination (for hit-testing)
						widgets::rectangle icn(it.destination, widgets::icon_impl::alias_icon());
						icn().rect.size(it.destination.size().width, it.destination.size().height);
						icn().corner_radius_x = icon().corner_radius_x;
						icn().corner_radius_y = icon().corner_radius_y;
						icn().border = icon().border;
						icn().color_fill = icon().color_fill;
						icn().color_border = icon().color_border;
						icn().color_hot = icon().color_hot;
						icn().cursor = widgets::specs::cursor_type::hand;

						// move the click and action handler from the icon to the rectangle
						icn().events().action = icon().events().action;
						icn().events().click = icon().events().click;
						icon().events().action = nullptr;
						icon().events().click = nullptr;

						// compute size of image side
						auto image_side = smallest(it.destination.size().width, it.destination.size().height) - 2 * padding_;
						if (icon().max_image_size > 0.f)
							image_side = smallest(image_side, icon().max_image_size);

						// add image to destination
						widgets::image_view image(it.destination, widgets::icon_impl::alias_image());
						image().rect = { 0, image_side, 0, image_side };

						switch (icon().text_position) {
						case widgets::icon::icon_specs::icon_text_position::right:
							image().rect.place({ padding_, it.destination.size().width - padding_, padding_, it.destination.size().height - padding_ },
								0.f, 0.f);
							break;
						case widgets::icon::icon_specs::icon_text_position::left:
							image().rect.place({ padding_, it.destination.size().width - padding_, padding_, it.destination.size().height - padding_ },
								100.f, 0.f);
							break;
						case widgets::icon::icon_specs::icon_text_position::top:
							image().rect.place({ padding_, it.destination.size().width - padding_, padding_, it.destination.size().height - padding_ },
								50.f, 100.f);
							break;
						case widgets::icon::icon_specs::icon_text_position::bottom:
							image().rect.place({ padding_, it.destination.size().width - padding_, padding_, it.destination.size().height - padding_ },
								50.f, 0.f);
							break;
						default:
							break;
						}
						
						image().file = icon().file;
						image().png_resource = icon().png_resource;
						image().quality = icon().quality;

						// add text to destination
						widgets::label text(it.destination, widgets::icon_impl::alias_text());
						text().text = icon().text;
						text().color_text = icon().color_text;
						text().font_size = icon().font_size;

						switch (icon().text_position) {
						case widgets::icon::icon_specs::icon_text_position::right: {
							text().rect = { 0, it.destination.size().width - (image_side + gap_) - padding_, 0, it.destination.size().height };
							const auto ideal_rect = widgets::measure_label(p_directwrite_factory_, text().text, text().font, text().font_size,
								false, false, convert_rect(text().rect));
							text().rect.height(ideal_rect.bottom - ideal_rect.top);
							text().rect.snap_to(image().rect, rect::snap_type::right_top, gap_);
						}
							break;
						case widgets::icon::icon_specs::icon_text_position::left: {
							text().rect = { padding_, it.destination.size().width - (image_side + gap_) - padding_, padding_, it.destination.size().height };
							const auto ideal_rect = widgets::measure_label(p_directwrite_factory_, text().text, text().font, text().font_size,
								false, false, convert_rect(text().rect));
							text().rect.height(ideal_rect.bottom - ideal_rect.top);
							text().rect.snap_to(image().rect, rect::snap_type::left_top, gap_);
						}
							break;
						case widgets::icon::icon_specs::icon_text_position::top: {
							text().rect = { padding_, it.destination.size().width - padding_, 0, it.destination.size().height - padding_ };
							const auto ideal_rect = widgets::measure_label(p_directwrite_factory_, text().text, text().font, text().font_size,
								false, false, convert_rect(text().rect));
							text().rect.height(ideal_rect.bottom - ideal_rect.top);
							text().center_h = true;
						}
							break;
						case widgets::icon::icon_specs::icon_text_position::bottom: {
							text().rect = { padding_, it.destination.size().width - padding_, 0, it.destination.size().height - padding_ };
							const auto ideal_rect = widgets::measure_label(p_directwrite_factory_, text().text, text().font, text().font_size,
								false, false, convert_rect(text().rect));
							text().rect.height(ideal_rect.bottom - ideal_rect.top);
							text().center_h = true;
							text().rect.snap_to(image().rect, rect::snap_type::bottom, gap_);
						}
							break;
						default:
							break;
						}

						// add description to destination
						widgets::label description(it.destination, widgets::icon_impl::alias_description());
						description().text = icon().description;
						description().color_text = icon().color_text_description;
						description().font_size = icon().font_size_description;
						description().multiline = true;

						switch (icon().text_position) {
						case widgets::icon::icon_specs::icon_text_position::right: {
							description().rect = { padding_, text().rect.width(), padding_, it.destination.size().height - text().rect.height() };
							description().rect.snap_to(text().rect, rect::snap_type::bottom_left, 0.f);
						}
																				 break;
						case widgets::icon::icon_specs::icon_text_position::left: {
							description().rect = { padding_, text().rect.width(), padding_, it.destination.size().height - text().rect.height() };
							description().rect.snap_to(text().rect, rect::snap_type::bottom_left, 0.f);
						}
							break;
						case widgets::icon::icon_specs::icon_text_position::top: {
							description().rect = { padding_, text().rect.width(), 0, it.destination.size().height - text().rect.height() };
							description().rect.snap_to(text().rect, rect::snap_type::bottom, 0.f);
							description().center_h = true;
						}
							break;
						case widgets::icon::icon_specs::icon_text_position::bottom: {
							description().rect = { padding_, text().rect.width(), 0, it.destination.size().height - text().rect.height() };
							description().rect.snap_to(text().rect, rect::snap_type::bottom, 0.f);
							description().center_h = true;
						}
							break;
						default:
							break;
						}

						// capture image view specs
						it.destination.d_page_.get_icon(it.alias).set_icon_specs(
							icn(), image(), text(), description());

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::icon, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			}
		}

		/// The table view widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. The table view is moved into the pane
		void form::impl::move_tables() {
			// check if this page has a table pane
			auto page_iterator = p_pages_.find(current_page_);

			struct table_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::table_view::table_view_specs table;
				lecui::containers::page& source;
				lecui::containers::page& destination;
			};

			std::vector<table_info> tables;

			if (page_iterator != p_pages_.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_tables_to_move(lecui::containers::page& page,
						std::vector<table_info>& tables) {
						for (auto& widget : page.d_page_.widgets()) {
							// check if this is a table pane
							if (widget.first.find(widgets::pane_impl::table_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a table pane (it has a table inside. move was already done), continue to next widget

							// check if this is a table
							if (widget.second.type() == widgets::widget_type::table_view) {
								// this is a table, we need to "move" it into a special pane

								// get the table specs
								auto& table_specs = page.d_page_.get_table_view(widget.first).specs();

								// make pane whose alias is prefixed by the special string
								containers::pane pane(page, widgets::pane_impl::table_pane_alias_prefix() + widget.first);

								// clone essential properties to pane
								pane().rect = table_specs.rect;
								pane().on_resize = table_specs.on_resize;
								pane().color_fill = table_specs.color_fill;
								pane().color_border = table_specs.color_border;

								// save move info so we can move the table into the pane later
								// we cannot do it here because we're iterating
								tables.push_back({ widget.first, table_specs, page, pane.get() });
								break;
							}

							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								// initialize tabs
								for (auto& tab : tab_pane.p_tabs_)
									find_tables_to_move(tab.second, tables);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									// initialize panes
									for (auto& page : pane.p_panes_)
										find_tables_to_move(page.second, tables);	// recursion
								}
						}
					}
				};

				helper::find_tables_to_move(page, tables);

				// move the tables
				for (auto& it : tables) {
					log("moving table: " + it.alias + " from " + it.source.d_page_.alias() + " to " + it.destination.d_page_.alias());

					try {
						// clone into destination
						widgets::table_view table(it.destination, it.alias);
						// copy specs
						table() = it.table;

						// adjust specs
						table().rect = { 0, it.destination.size().width, 0, it.destination.size().height };
						table().on_resize = { 0, 0, 0, 0 };	// critical because table will change size as table is browsed or changed. the pane scroll bars will do the job.

						// close widget
						std::string error;
						it.source.d_page_.close_widget(it.alias, widgets::widget_type::table_view, error);
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

		float form::impl::get_dpi_scale() {
			return dpi_scale_;
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

				if (!allow_minimize_) {
					SetWindowLong(hWnd_, GWL_STYLE,
						GetWindowLong(hWnd_, GWL_STYLE) & ~WS_MINIMIZEBOX);
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
				try {
					// check special tree pane
					auto& tree_pane_control = container.d_page_.get_pane(widgets::pane_impl::tree_pane_alias_prefix() + path);

					// tree pane control confirmed ... get the pane page
					auto& tree_pane = tree_pane_control.p_panes_.at("pane");

					// pane confirmed ... recurse
					return find_widget(tree_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special html pane
					auto& html_pane_control = container.d_page_.get_pane(widgets::pane_impl::html_pane_alias_prefix() + path);

					// html pane control confirmed ... get the pane page
					auto& html_pane = html_pane_control.p_panes_.at("pane");

					// pane confirmed ... recurse
					return find_widget(html_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special time pane
					auto& time_pane_control = container.d_page_.get_pane(widgets::pane_impl::time_pane_alias_prefix() + path);

					// time pane control confirmed ... get the pane page
					auto& time_pane = time_pane_control.p_panes_.at("pane");

					// pane confirmed ... recurse
					return find_widget(time_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special date pane
					auto& date_pane_control = container.d_page_.get_pane(widgets::pane_impl::date_pane_alias_prefix() + path);

					// date pane control confirmed ... get the pane page
					auto& date_pane = date_pane_control.p_panes_.at("pane");

					// pane confirmed ... recurse
					return find_widget(date_pane, path);
				}
				catch (const std::exception&) {}

				// check special icon pane
				auto& icon_pane_control = container.d_page_.get_pane(widgets::pane_impl::icon_pane_alias_prefix() + path);

				// icon pane control confirmed ... get the pane page
				auto& icon_pane = icon_pane_control.p_panes_.at("pane");

				// pane confirmed ... recurse
				return find_widget(icon_pane, path);
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

			if (allow_minimize_)
				SetWindowLong(hWnd_, GWL_STYLE,
					GetWindowLong(hWnd_, GWL_STYLE) | WS_MINIMIZEBOX);
			else
				SetWindowLong(hWnd_, GWL_STYLE,
					GetWindowLong(hWnd_, GWL_STYLE) & ~WS_MINIMIZEBOX);
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
			auto do_close_special = [&](const std::string& special_pane_alias_prefix,
				const std::string& alias) {
				// get the special pane's path
				const auto idx = path.rfind("/");

				if (idx != std::string::npos) {
					const auto pane_path = path.substr(0, idx + 1);
					// close pane (this will automatically close all the contents, including the widget in question)
					close_container(pane_path + special_pane_alias_prefix + alias);
				}
			};

			auto do_close = [&](widget_search_results result) {
				std::string error;
				switch (result.widget.type()) {
					/// widgets in special panes need special treatment
				case liblec::lecui::widgets::widget_type::tree_view:
					do_close_special(widgets::pane_impl::tree_pane_alias_prefix(), result.widget.alias());
					break;
				case liblec::lecui::widgets::widget_type::html_editor:
					do_close_special(widgets::pane_impl::html_controls_pane_alias_prefix(), result.widget.alias());
					do_close_special(widgets::pane_impl::html_pane_alias_prefix(), result.widget.alias());
					break;
				case liblec::lecui::widgets::widget_type::time:
					do_close_special(widgets::pane_impl::time_pane_alias_prefix(), result.widget.alias());
					break;
				case liblec::lecui::widgets::widget_type::date:
					do_close_special(widgets::pane_impl::date_pane_alias_prefix(), result.widget.alias());
					break;
				case liblec::lecui::widgets::widget_type::icon:
					do_close_special(widgets::pane_impl::icon_pane_alias_prefix(), result.widget.alias());
					break;

					/// plain widgets can be closed directly
				default:
					result.page.d_page_.close_widget(result.widget.alias(), result.widget.type(), error);
					break;
				}
			};

			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(p_pages_.at(page_alias), path_remaining);
						do_close(result);
						update();
					}
					catch (const std::exception&) {
						try {
							// check status pages
							auto result = find_widget(p_status_panes_.at(page_alias), path_remaining);
							do_close(result);
							update();
						}
						catch (const std::exception&) {
							// check tab controls
							const auto tab_name = find_page(p_pages_.at(page_alias), path_remaining).d_page_.alias();

							// tab control confirmed ... get the tab control's container and the tab control's implementation
							auto idx = path_remaining.rfind("/");

							if (idx != std::string::npos) {
								auto tab_control_container_path_remaining = path_remaining.substr(0, idx);
								idx = tab_control_container_path_remaining.rfind("/");

								if (idx != std::string::npos) {
									// this tab control is not directly in a top level container
									const auto alias = tab_control_container_path_remaining.substr(idx + 1);
									tab_control_container_path_remaining = tab_control_container_path_remaining.substr(0, idx);

									// get tab control's container
									auto& page = find_page(p_pages_.at(page_alias), tab_control_container_path_remaining);

									// get the tab control implementation
									auto& tab_pane_impl = page.d_page_.get_tab_pane(alias);

									// close tab
									tab_pane_impl.close_tab(tab_name);
									update();
								}
								else {
									// this tab control is in a top level container
									const auto alias = tab_control_container_path_remaining;
									
									// get tab control's container
									auto& page = p_pages_.at(page_alias);

									// get the tab control implementation
									auto& tab_pane_impl = page.d_page_.get_tab_pane(alias);

									// close tab
									tab_pane_impl.close_tab(tab_name);
									update();
								}
							}
						}
					}
				}
			}
			catch (const std::exception&) {}
		}

		void form::impl::close_container(const std::string& path) {
			close(path);	// to-do: enable closing of top level containers
		}

		void form::impl::clear_selection(containers::page& container) {
			for (auto& widget : container.d_page_.widgets()) {
				widget.second.select(false);

				if (widget.second.type() ==
					widgets::widget_type::tab_pane) {
					// get this tab pane
					auto& tab_pane = container.d_page_.get_tab_pane(widget.first);

					auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

					if (page_iterator != tab_pane.p_tabs_.end())
						clear_selection(page_iterator->second);	// recursion
				}
				else
					if (widget.second.type() ==
						widgets::widget_type::pane) {
						// get this pane
						auto& pane = container.d_page_.get_pane(widget.first);

						auto page_iterator = pane.p_panes_.find(pane.current_pane_);

						if (page_iterator != pane.p_panes_.end())
							clear_selection(page_iterator->second);	// recursion
					}
			}
		}

		void form::impl::select(const std::string& path) {
			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					// clear selection in given page
					clear_selection(p_pages_.at(page_alias));

					try {
						// check form pages
						auto result = find_widget(p_pages_.at(page_alias), path_remaining);
						result.widget.select(true);
						update();
					}
					catch (const std::exception&) {}

					try {
						// check status pages
						auto result = find_widget(p_status_panes_.at(page_alias), path_remaining);
						result.widget.select(true);
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

		HWND form::impl::find_native_handle(const std::string& guid) {
			struct searcher_struct {
				/// <summary>The window's unique registration ID.</summary>
				UINT unique_reg_id = 0;

				/// <summary>The window handle.</summary>
				HWND hWnd = nullptr;
			};

			class helper {
			public:
				/// <summary>Search for an existing instance of such a registered window.</summary>
				/// <param name="hWnd">The handle of the window.</param>
				/// <param name="lParam">Pointer to data struct.</param>
				/// <returns>return TRUE if the window is not found, and FALSE if it is found.</returns>
				static BOOL CALLBACK searcher(HWND hWnd, LPARAM lParam) {
					searcher_struct* p = (searcher_struct*)lParam;

					if (p) {
						UINT UWM_ARE_YOU_ME = p->unique_reg_id;

						DWORD_PTR result = 0;
						LRESULT ok = ::SendMessageTimeout(hWnd,
							UWM_ARE_YOU_ME,	// message
							0,				// WPARAM
							0,
							SMTO_BLOCK |
							SMTO_ABORTIFHUNG,
							200,
							&result);

						if (ok == 0)
							return TRUE; // ignore this and continue ... could be it, but hung

						if (result == UWM_ARE_YOU_ME) {
							// found it
							p->hWnd = hWnd;
							return FALSE; // stop search
						}
					}

					return TRUE; // continue search
				}
			};

			if (!guid.empty()) {
				searcher_struct search_info;
				search_info.unique_reg_id = RegisterWindowMessageA(guid.c_str());
				EnumWindows(helper::searcher, (LPARAM)&search_info);
				return search_info.hWnd;
			}

			return nullptr;
		}

		void form::impl::open_existing_instance() {
			if (!guid_.empty()) {
				auto hWnd = find_native_handle(guid_);

				if (hWnd != nullptr) {
					log("opening existing instance");

					SetForegroundWindow(hWnd);	// pop

					if (IsMinimized(hWnd))
						ShowWindow(hWnd, SW_RESTORE);	// restore
					else
						if (!IsWindowVisible(hWnd))
							ShowWindow(hWnd, SW_SHOW);	// show

					// check if there is data in the commandline
					command_line_parser args;

					std::string arg_string;

					if (args.size() > 1) {
						log("Passing data in command line to existing instance");

						for (size_t i = 1; i < args.size(); i++) {
							std::string arg = args[i];

							if (arg.find(" ") != std::string::npos)
								arg = "\"" + arg + "\"";

							if (arg_string.empty())
								arg_string = arg;
							else
								arg_string += " " + arg;
						}

						// pass the commandline to the existing instance
						LPSTR szCmdLine = (LPSTR)arg_string.c_str();

						COPYDATASTRUCT cds;
						cds.cbData = static_cast<DWORD>(strlen(szCmdLine) + 1);
						cds.lpData = szCmdLine;

						DWORD_PTR result = 0;
						LRESULT ok = ::SendMessageTimeout(hWnd,
							WM_COPYDATA,	// message
							0,				// WPARAM
							(LPARAM)&cds,	// LPARAM
							SMTO_BLOCK |
							SMTO_ABORTIFHUNG,
							500,			// a bit more generaous since we know our window
							&result);
					}
				}
			}
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

			auto form_optional = get_form();

			if (!form_optional.has_value())
				return DefWindowProc(hWnd, msg, wParam, lParam);

			auto& form_ = form_optional.value().get();

			switch (msg) {
			case WM_CREATE:
				form_.d_.hWnd_ = hWnd;

				if (form_.d_.p_maximize_button_)
					form_.d_.p_maximize_button_->set_hwnd(hWnd);

				if (form_.d_.p_minimize_button_)
					form_.d_.p_minimize_button_->set_hwnd(hWnd);

				if (!form_.d_.tooltip_form_) {
					form_.d_.set_borderless(hWnd, form_.d_.borderless_);
					form_.d_.set_borderless_shadow(hWnd, form_.d_.borderless_shadow_);
				}

				if (!form_.d_.allow_resizing_)
					SetWindowLong(hWnd, GWL_STYLE,
						GetWindowLong(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

				if (!form_.d_.allow_minimize_) {
					SetWindowLong(hWnd, GWL_STYLE,
						GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
				}

				form_.on_start();

				for (auto& it : form_.d_.timers_)
					if (!it.second.running)
						form_.d_.start_timer(it.first);

				return TRUE;

			case WM_LBUTTONDOWN:
				form_.d_.lbutton_pressed_ = true;
				// capture the mouse
				SetCapture(hWnd);
				form_.d_.on_lbuttondown(form_.d_.get_cursor_position());
				return NULL;

			case WM_LBUTTONUP:
				// release the mouse capture
				ReleaseCapture();
				form_.d_.on_lbuttonup(form_.d_.get_cursor_position());
				form_.d_.lbutton_pressed_ = false;
				return NULL;

			case WM_SIZING:
				form_.d_.user_sizing_ = true;
				break;

			case WM_EXITSIZEMOVE:
				form_.d_.user_sizing_ = false;
				break;

			case WM_WINDOWPOSCHANGING:
				form_.d_.on_form_pos_changing(lParam);
				break;

			case WM_SIZE:
				form_.d_.on_resize(LOWORD(lParam), HIWORD(lParam));
				return NULL;

			case WM_MOUSEMOVE:
				form_.d_.mouse_track_.on_mouse_move(hWnd);
				form_.d_.client_hittest(form_.d_.get_cursor_position());
				return NULL;

			case WM_MOUSELEAVE:
				// form_.d_.get_cursor_position() doesn't work here
				form_.d_.client_hittest({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
				form_.d_.mouse_track_.reset(hWnd);
				return NULL;

			case WM_MOUSEHOVER:
				form_.d_.mouse_track_.reset(hWnd);
				return NULL;

			case WM_SETCURSOR:
				if (LOWORD(lParam) == HTCLIENT) {
					HCURSOR h_cursor = form_.d_.h_widget_cursor_;
					if (h_cursor) {
						SetCursor(h_cursor);
						return TRUE;
					}
				}
				break;

			case WM_DISPLAYCHANGE:
				form_.d_.update();
				return NULL;

			case WM_SETFOCUS:
				return NULL;

			case WM_KILLFOCUS:
				// check if it's the parent that now has focus
				if (form_.d_.menu_form_ || form_.d_.tooltip_form_) {
					if (IsWindow(form_.d_.hWnd_parent_)) {
						if (GetForegroundWindow() != form_.d_.hWnd_parent_) {
							// focus lost, but not to parent
							form_.close();
						}
						else {
							// let parent decide whether to close child in WM_LBUTTONDOWN
						}
					}
					else {
						// focus lost, and there is no parent
						form_.close();
					}
				}
				return NULL;

			case WM_PAINT:
				form_.d_.move_trees();
				form_.d_.move_html_editors();
				form_.d_.move_times();
				form_.d_.move_dates();
				form_.d_.move_icons();
				form_.d_.move_tables();
				form_.d_.on_render();

				if (form_.d_.schedule_refresh_)
					form_.d_.schedule_refresh_ = false;
				else
					ValidateRect(hWnd, nullptr);

				return NULL;

			case WM_CLOSE:
				try {
					if (!form_.d_.widgets_.at("close_button").enabled())
						return NULL;
				}
				catch (const std::exception&) {}
				form_.on_close();
				return NULL;

			case WM_DESTROY:
				form_.on_shutdown();
				PostQuitMessage(0);
				return NULL;

			case WM_NCCALCSIZE:
				if (wParam == TRUE && form_.d_.borderless_) {
					auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
					adjust_maximized_client_rect(hWnd, params.rgrc[0]);

					if (maximized(hWnd)) {
						form_.d_.shadow_setting_before_maximize_ =
							form_.d_.borderless_shadow_;
						form_.d_.set_borderless_shadow(hWnd, false, false);
					}
					else
						form_.d_.set_borderless_shadow(hWnd,
							form_.d_.shadow_setting_before_maximize_, false);

					return NULL;
				}
				break;

			case WM_NCHITTEST:
				if (form_.d_.borderless_)
					return form_.d_.non_client_hittest(
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
				for (auto& [key, child] : form_.d_.m_children_) {
					if (child && IsWindow(child->d_.hWnd_) && (child->d_.menu_form_ || child->d_.tooltip_form_)) {
						// close child menu forms and child tooltip forms
						child->close();
					}
				}
				break;

			case WM_GETMINMAXINFO: {
				// set lower limits to window size
				MINMAXINFO* p_minmaxinfo = (MINMAXINFO*)lParam;
				p_minmaxinfo->ptMinTrackSize.x = static_cast<LONG>(.5f +
					form_.d_.min_size_.width * form_.d_.dpi_scale_);
				p_minmaxinfo->ptMinTrackSize.y = static_cast<LONG>(.5f +
					form_.d_.min_size_.height * form_.d_.dpi_scale_);
			} break;

			case WM_TIMER: {
				int unique_id = (int)wParam;

				for (auto& it : form_.d_.timers_)
					if (it.second.unique_id == unique_id) {
						if (it.second.on_timer)
							it.second.on_timer();

						break;
					}
			} break;

			case WM_CHAR:
				form_.d_.on_char(wParam);
				break;

			case WM_GETDLGCODE:
				return DLGC_WANTALLKEYS;	// for VK_UP, VK_DOWN to be received in WM_KEYDOWN

			case WM_KEYDOWN:
				form_.d_.on_keydown(wParam);
				break;

			case WM_KEYUP:
				form_.d_.on_keyup(wParam);
				break;

			case WM_MOUSEWHEEL:
				form_.d_.on_wheel(wParam);
				break;

			case WM_MOUSEHWHEEL:
				form_.d_.on_hwheel(wParam);
				break;

			case WM_COPYDATA: {
				/// 1. Busy
				/// 2. No handler
				/// 3. Handled
				LRESULT result = 0;
				if (!form_.d_.receiving_) {
					COPYDATASTRUCT* p_copy_data = (COPYDATASTRUCT*)lParam;

					if (p_copy_data && form_.d_.on_receive_data_) {
						form_.d_.data_received_ = std::string((LPSTR)p_copy_data->lpData, p_copy_data->cbData);

						// forward data to the receive data handler through a timer set to 0
						if (!form_.d_.data_received_.empty()) {
							form_.d_.receiving_ = true;
							timer_management(form_).add(form_.d_.receive_data_timer_alias_, 0, [&]() {
								timer_management(form_).stop(form_.d_.receive_data_timer_alias_);
								form_.d_.on_receive_data_(form_.d_.data_received_);
								form_.d_.receiving_ = false;
								});
						}

						result = instance_messages::handled;
					}
					else {
						if (!form_.d_.on_receive_data_)
							result = instance_messages::no_handler;
					}
				}
				else
					result = instance_messages::busy;

				if (result)
					return result;
			} break;

			case WM_DROPFILES:
				form_.d_.on_dropfiles(wParam);
				break;

			case WM_APP:
				switch (lParam) {
				case WM_RBUTTONUP:
					// tray icon right clicked
					if (!form_.d_.tray_icon_menu_items_.empty()) {
						context_menu::specs menu_specs;
						menu_specs.quality = image_quality::high;

						for (const auto& item : form_.d_.tray_icon_menu_items_) {
							menu_item mi;
							mi.label = item.label;
							mi.font = item.font;
							mi.font_size = item.font_size;
							menu_specs.items.push_back(mi);
						}

						menu_specs.type = context_menu::pin_type::bottom;

						auto result = context_menu()(form_, menu_specs);

						for (auto& m_it : form_.d_.tray_icon_menu_items_) {
							if (!m_it.label.empty() && m_it.label == result) {
								if (m_it.action != nullptr)
									m_it.action();
							}
						}
					}
					break;

				case WM_LBUTTONUP:
					// tray icon left clicked, invoke the action of the default item
					for (auto& m_it : form_.d_.tray_icon_menu_items_) {

						std::string plain_label;
						// parse the label
						// the default color doesn't matter here we're just getting the plain text
						std::vector<formatted_text_parser::text_range_properties> formatting;
						widgets::parse_formatted_text(m_it.label, plain_label,
							D2D1::ColorF(D2D1::ColorF::Black), formatting);

						if (!plain_label.empty() && plain_label == form_.d_.tray_item_default_) {
							if (m_it.action != nullptr)
								m_it.action();
						}
					}
					break;

				default:
					break;
				}
				break;

			default:
				// check if caller is checking this form's unique registration id
				if (form_.d_.reg_id_ && msg == form_.d_.reg_id_)
					return form_.d_.reg_id_;	// another instance is checking if it should proceed
				break;
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}
