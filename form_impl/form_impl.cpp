//
// form_impl.cpp - form::impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "form_impl.h"

#include "../font/font.h"

#include "../utilities/timer.h"

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
#include <optional>

namespace liblec {
	namespace lecui {
		// initialize static variables
		std::atomic<unsigned long> form::impl::_instances = 0;
		std::atomic<bool> form::impl::_initialized = false;
		ID2D1Factory* form::impl::_p_direct2d_factory = nullptr;
		IDWriteFactory* form::impl::_p_directwrite_factory = nullptr;
		IWICImagingFactory* form::impl::_p_iwic_factory = nullptr;
		limit_single_instance* form::impl::_p_instance = nullptr;

		form::impl::impl(form& fm, const std::string& caption_formatted) :
			_fm(fm),
			_p_parent(nullptr),
			_menu_form(caption_formatted == form::menu_form_caption()),
			_tooltip_form(caption_formatted == form::tooltip_form_caption()),
			_create_called(false),
			_reg_id(0),
			_receiving(false),
			_data_received(std::string()),
			_caption_bar_height(_menu_form || _tooltip_form ? 0.f : 30.f),
			_caption_and_menu_gap(25.f),
			_form_menu_margin(10.f),
			_form_border_thickness(1.f),
			_content_margin(_form_border_thickness / 2.f),
			_control_button_margin(2.f),
			_caption_icon_minimum_margin(7.f),
			_caption_icon_maximum_size(24.f),
			_receive_data_timer_alias("liblec::lecui::receive_data_timer"),
			_current_thread_id(GetCurrentThreadId()),
			_resource_dll_filename(std::string()),
			_resource_module_handle(nullptr),
			_idi_icon(0),
			_idi_icon_small(0),
			_theme(themes::light),
			_clr_background(defaults::color(_theme, element::form)),
			_clr_titlebar_background(defaults::color(_theme, element::titlebar)),
			_clr_theme(defaults::color(_theme, element::accent)),
			_clr_theme_hot(defaults::color(_theme, element::accent_hover)),
			_clr_theme_disabled(defaults::color(_theme, element::disabled)),
			_clr_theme_text(defaults::color(_theme, element::label)),
			_top_most(false),
			_hWnd(nullptr),
			_hWnd_parent(nullptr),
			_caption_formatted(caption_formatted),
			_activate(true),
			_point({ 0L, 0L }),
			_design_size({ 780.f, 480.f }),
			_min_size({ 500.f, 300.f }),
			_allow_resizing(true),
			_allow_minimize(true),
			_user_pos(false),
			_preset_pos(false),
			_form_position(form_position::center_to_working_area),
			_dpi_scale(get_process_dpi()),
			_borderless(true),
			_borderless_shadow(true),
			_previous_shadow_setting(_borderless_shadow),
			_shadow_setting_before_maximize(_borderless_shadow),
			_p_render_target(nullptr),
			_p_brush_theme(nullptr),
			_p_brush_theme_hot(nullptr),
			_p_brush_theme_disabled(nullptr),
			_p_brush_titlebar(nullptr),
			_current_page(std::string()),
			_controls_page(fm, ""),
			_p_caption_icon(nullptr),
			_p_caption(nullptr),
			_p_close_button(nullptr),
			_p_maximize_button(nullptr),
			_p_minimize_button(nullptr),
			_point_before({ 0.f, 0.f }),
			_user_sizing(false),
			_unique_id(1000),
			_reverse_tab_navigation(false),
			_shift_pressed(false),
			_ctrl_pressed(false),
			_space_pressed(false),
			_lbutton_pressed(false),
			_h_widget_cursor(nullptr),
			_schedule_refresh(false),
			_close_called(false),
			_force_instance(false),
			_tray_icon_present(false),
			_start_hidden(false),
			_side_pane_present(false),
			_side_pane_thickness(0.f),
			_png_caption_icon(0) {
			++_instances;	// increment instances count

			/// Use HeapSetInformation to specify that the process should terminate if the heap manager
			/// detects an error in any heap used by the process. The return value is ignored, because
			/// we want to continue running in the unlikely event that HeapSetInformation fails.
			HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

			// parse the caption
			// the default color doesn't matter here we're just getting the plain text
			std::vector<formatted_text_parser::text_range_properties> formatting;
			widgets::parse_formatted_text(_caption_formatted, _caption_plain,
				D2D1::ColorF(D2D1::ColorF::Black), formatting);

			if (_instances == 1) {
				// initialize COM
				HRESULT hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

				if (SUCCEEDED(hres)) {
					// Create a Direct2D factory
					hres = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_p_direct2d_factory);
				}
				if (SUCCEEDED(hres)) {
					// Create a DirectWrite factory.
					hres = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
						__uuidof(_p_directwrite_factory),
						reinterpret_cast<IUnknown**>(&_p_directwrite_factory));
				}
				if (SUCCEEDED(hres)) {
					// Create IWIC Imaging factory.
					hres = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
						IID_PPV_ARGS(&_p_iwic_factory));
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
				_initialized = true;
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

			if (_initialized && _instances == 1) {
				// release IWIC Imaging resources used by all instances
				safe_release(&_p_iwic_factory);

				// release DirectWrite resources used by all instances
				safe_release(&_p_directwrite_factory);

				// release Direct2D resources used by all instances
				safe_release(&_p_direct2d_factory);

				// Uninitialize COM
				CoUninitialize();

				if (_p_instance) {
					delete _p_instance;
					_p_instance = nullptr;
				}

				// set initialized flag to false (only here)
				_initialized = false;
			}

			--_instances;	// decrement instances count

			log("exiting form::impl destructor");
		}

		/// Creates resources that are bound to a particular Direct3D device. These resources need to
		/// be recreated if the Direct3D device disappears, such as when the display changes, etc
		/// 
		/// This function will only create widget resources for those pages that have been
		/// added in layout(). The resources for pages added later will be created
		/// on-the-fly through the use of the
		/// widgets::widget::_resources_created flag (this flag is
		/// used internally by each widget's implementation to check if resources have been created
		/// and to create them if not).
		HRESULT form::impl::create_device_resources() {
			HRESULT hr = S_OK;

			if (!_p_render_target) {
				RECT rc;
				GetClientRect(_hWnd, &rc);

				D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

				// Create a Direct2D render target.
				hr = _p_direct2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(_hWnd, size),
					&_p_render_target);

				if (SUCCEEDED(hr)) {
					// create the theme brushes
					hr = _p_render_target->CreateSolidColorBrush(
						convert_color(_clr_theme), &_p_brush_theme);
					hr = _p_render_target->CreateSolidColorBrush(
						convert_color(_clr_theme_hot), &_p_brush_theme_hot);
					hr = _p_render_target->CreateSolidColorBrush(
						convert_color(_clr_theme_disabled), &_p_brush_theme_disabled);
					hr = _p_render_target->CreateSolidColorBrush(
						convert_color(_clr_titlebar_background), &_p_brush_titlebar);
				}
				if (SUCCEEDED(hr)) {
					try {
						// create form widget resources
						for (auto& widget : _widgets)
							hr = widget.second.create_resources(_p_render_target);
					}
					catch (const std::exception& e) { log(e.what()); }
				}
				if (SUCCEEDED(hr)) {
					class helper {
					public:
						static void create_resources(const containers::page& page,
							ID2D1HwndRenderTarget* _p_render_target) {
							// create widget resources
							for (auto& widget : page._d_page.widgets()) {
								HRESULT hr = widget.second.create_resources(_p_render_target);

								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									try {
										// get this tab pane
										auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

										for (auto& tab : tab_pane._p_tabs)
											create_resources(tab.second, _p_render_target);
									}
									catch (const std::exception&) {}
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										try {
											// get this pane
											auto& pane = page._d_page.get_pane_impl(widget.first);

											for (auto& page : pane._p_panes)
												create_resources(page.second, _p_render_target);
										}
										catch (const std::exception&) {}
									}
							}
						}
					};

					try {
						// create page resources
						for (auto& p_page : _p_pages)
							helper::create_resources(p_page.second, _p_render_target);
					}
					catch (const std::exception& e) { log(e.what()); }
				}
			}

			return hr;
		}

		/// Discards device-dependent resources. These resources must be recreated when the Direct3D
		/// device is lost
		void form::impl::discard_device_resources(bool in_destructor) {
			safe_release(&_p_render_target);
			safe_release(&_p_brush_theme);
			safe_release(&_p_brush_theme_hot);
			safe_release(&_p_brush_theme_disabled);
			safe_release(&_p_brush_titlebar);

			class helper {
			public:
				static void discard(const containers::page& page, bool in_destructor) {
					// discard widget resources
					for (const auto& widget : page._d_page.widgets()) {
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
								const auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								for (const auto& tab : tab_pane._p_tabs)
									discard(tab.second, in_destructor);			// recursion
							}
							catch (const std::exception&) {}
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								try {
									// get this pane
									const auto& pane = page._d_page.get_pane_impl(widget.first);

									for (const auto& page : pane._p_panes)
										discard(page.second, in_destructor);	// recursion
								}
								catch (const std::exception&) {}
							}
					}
				}
			};

			// discard page resources
			for (const auto& p_page : _p_pages)
				helper::discard(p_page.second, in_destructor);

			// discard form widget resources
			for (const auto& widget : _widgets)
				widget.second.discard_resources();
		}

		void form::impl::create_close_button(std::function<void()> on_click) {
			_p_close_button =
				std::unique_ptr<widgets::close_button_impl>(new
					widgets::close_button_impl(_controls_page));
			_widgets.emplace(_p_close_button->alias(), *_p_close_button);
			_widgets_order.emplace_back(_p_close_button->alias());
			
			_p_close_button->specs()
				.tooltip("Close")
				.color_fill(_clr_theme)
				.color_hot(_theme == themes::dark ?
					defaults::color(_theme, element::form).lighten(5.f) :
					defaults::color(_theme, element::form).darken(5.f))
				.color_disabled(_clr_theme_disabled)
				.on_resize().x_rate(100.f);

			_p_close_button->specs().rect()
				.right(_design_size.get_width() - _control_button_margin)
				.top(_control_button_margin)
				.left(_p_close_button->specs().rect().right() - (1.5f * _caption_bar_height - 2.f * _control_button_margin))
				.bottom(_p_close_button->specs().rect().top() + (_caption_bar_height - 2.f * _control_button_margin));

			_p_close_button->specs().events().click = on_click;
		}

		// should be called after create_close_button()
		void form::impl::create_maximize_button() {
			_p_maximize_button =
				std::unique_ptr<widgets::maximize_button_impl>(new
					widgets::maximize_button_impl(_controls_page, _p_direct2d_factory));
			_widgets.emplace(_p_maximize_button->alias(), *_p_maximize_button);
			_widgets_order.emplace_back(_p_maximize_button->alias());

			_p_maximize_button->specs()
				.tooltip("Maximize")
				.color_fill(_clr_theme)
				.color_hot(_theme == themes::dark ?
					defaults::color(_theme, element::form).lighten(5.f) :
					defaults::color(_theme, element::form).darken(5.f))
				.color_disabled(_clr_theme_disabled)
				.on_resize().x_rate(100.f);

			const auto right_edge = _p_close_button->specs().rect().left();

			_p_maximize_button->specs().rect()
				.right(right_edge - _control_button_margin)
				.top(_control_button_margin)
				.left(_p_maximize_button->specs().rect().right() - (1.5f * _caption_bar_height - 2.f * _control_button_margin))
				.bottom(_p_maximize_button->specs().rect().top() + (_caption_bar_height - 2.f * _control_button_margin));
		}

		// should be called after create_close_button() and create_maximize_button()
		void form::impl::create_minimize_button() {
			_p_minimize_button =
				std::unique_ptr<widgets::minimize_button_impl>(new
					widgets::minimize_button_impl(_controls_page));
			_widgets.emplace(_p_minimize_button->alias(), *_p_minimize_button);
			_widgets_order.emplace_back(_p_minimize_button->alias());

			_p_minimize_button->specs()
				.tooltip("Minimize")
				.color_fill(_clr_theme)
				.color_hot(_theme == themes::dark ?
					defaults::color(_theme, element::form).lighten(5.f) :
					defaults::color(_theme, element::form).darken(5.f))
				.color_disabled(_clr_theme_disabled)
				.on_resize().x_rate(100.f);

			const auto right_edge = _allow_resizing ?
				_p_maximize_button->specs().rect().left() : _p_close_button->specs().rect().left();

			_p_minimize_button->specs().rect()
				.right(right_edge - _control_button_margin)
				.top(_control_button_margin)
				.left(_p_minimize_button->specs().rect().right() - (1.5f* _caption_bar_height - 2.f * _control_button_margin))
				.bottom(_p_minimize_button->specs().rect().top() + (_caption_bar_height - 2.f * _control_button_margin));
		}

		void form::impl::create_form_caption() {
			_p_caption =
				std::unique_ptr<widgets::label_impl>(new
					widgets::label_impl(_controls_page, "form_caption",
						_p_directwrite_factory));
			_widgets.emplace(_p_caption->alias(), *_p_caption);
			_widgets_order.emplace_back(_p_caption->alias());

			_p_caption->specs()
				.text(_caption_formatted)
				.paragraph_alignment(paragraph_alignment::middle)
				.multiline(false)

				// load label settings
				.color_text(defaults::color(_theme, element::label))
				.color_selected(defaults::color(_theme, element::label_selected))
				.color_hot(defaults::color(_theme, element::label_hover))
				.color_hot_pressed(defaults::color(_theme, element::label_pressed))
				.color_disabled(defaults::color(_theme, element::label_disabled));

			// determine right-most edge based on available control buttons
			const auto right_edge = _allow_minimize ?
				_p_minimize_button->specs().rect().left() :
				(_allow_resizing ? _p_maximize_button->specs().rect().left() : _p_close_button->specs().rect().left());

			// With a caption icon margin of 7 pixels on a 30 pixel size caption bar, the icon is 16 pixels
			const float caption_icon_size = smallest(_caption_bar_height - 2.f * _caption_icon_minimum_margin, _caption_icon_maximum_size);

			const float vertical_margin = (_caption_bar_height - caption_icon_size) / 2.f;
			const float horizontal_margin = _png_caption_icon ? vertical_margin : 10.f;

			// determine the largest rect that the caption can occupy
			D2D1_RECT_F max_rect = D2D1::RectF(horizontal_margin + (_side_pane_present ? _side_pane_thickness : 0.f), _control_button_margin,
				right_edge - _control_button_margin, _caption_bar_height - _control_button_margin);

			if (_png_caption_icon) {
				// form caption icon
				D2D1_RECT_F _caption_icon = D2D1::RectF(max_rect.left, vertical_margin, max_rect.left + caption_icon_size, vertical_margin + caption_icon_size);

				_p_caption_icon =
					std::unique_ptr<widgets::image_view_impl>(new
						widgets::image_view_impl(_controls_page, "form_caption_icon",
							_p_iwic_factory, _p_directwrite_factory, _p_direct2d_factory));
				_widgets.emplace(_p_caption_icon->alias(), *_p_caption_icon);
				_widgets_order.emplace_back(_p_caption_icon->alias());

				_p_caption_icon->specs()
					.rect(convert_rect(_caption_icon))
					.png_resource(_png_caption_icon)
					.quality(image_quality::high)
					
					// load image settings
					.color_fill(defaults::color(_theme, element::image_view))
					.color_border(defaults::color(_theme, element::image_view_border))
					.color_hot(defaults::color(_theme, element::image_view_hover))
					.color_selected(defaults::color(_theme, element::image_view_selected));

				_p_caption_icon->specs().badge()
					.color(defaults::color(_theme, element::badge))
					.color_border(defaults::color(_theme, element::badge_border))
					.color_text(defaults::color(_theme, element::badge_text));
					;

				// adjust max_rect to factor in for the space taken by the icon
				max_rect.left = _caption_icon.right + horizontal_margin;
			}

			// determine the optimal rect for the caption
			const auto rect = widgets::measure_label(_p_directwrite_factory,
				_p_caption->specs().text(), _p_caption->specs().font(), _p_caption->specs().font_size(),
				_p_caption->specs().alignment(), _p_caption->specs().paragraph_alignment(), max_rect);

			// to-do: address this ... when form is downsized enough, caption creeps behind control
			// buttons since we've eliminated resizing caption rect; perhaps use clip???
			_p_caption->specs().rect(convert_rect(rect));

			// capture the caption handler
			_p_caption->specs().events().action = _fm.events().caption;
		}

		void form::impl::create_form_menu() {
			float left = _p_caption->specs().rect().right() + _caption_and_menu_gap;

			for (auto& main_menu_item : _form_menu) {
				_p_menu.push_back(
					std::unique_ptr<widgets::label_impl>(new
						widgets::label_impl(_controls_page, "menu::" + main_menu_item.text,
							_p_directwrite_factory)));
				auto& it = _p_menu.back();

				_widgets.emplace(it->alias(), *it);
				_widgets_order.emplace_back(it->alias());

				it->specs()
					.tooltip(main_menu_item.tooltip)
					.text(main_menu_item.text)
					.paragraph_alignment(paragraph_alignment::middle)
					.multiline(false)

					// load label settings
					.color_text(defaults::color(_theme, element::label))
					.color_selected(defaults::color(_theme, element::label_selected))
					.color_hot(defaults::color(_theme, element::label_hover))
					.color_hot_pressed(defaults::color(_theme, element::label_pressed))
					.color_disabled(defaults::color(_theme, element::label_disabled));

				// determine right-most edge based on available control buttons
				const auto right_edge = _allow_minimize ?
					_p_minimize_button->specs().rect().left() :
					(_allow_resizing ? _p_maximize_button->specs().rect().left() : _p_close_button->specs().rect().left());

				// determine the largest rect that the caption can occupy
				const D2D1_RECT_F max_rect = D2D1::RectF(left, _control_button_margin,
					right_edge - _control_button_margin, _caption_bar_height - _control_button_margin);

				// determine the optimal rect for the caption
				const auto rect = widgets::measure_label(_p_directwrite_factory,
					it->specs().text(), it->specs().font(), it->specs().font_size(),
					it->specs().alignment(), it->specs().paragraph_alignment(), max_rect);

				// to-do: address this ... when form is downsized enough, caption creeps behind control
				// buttons since we've eliminated resizing caption rect; perhaps use clip???
				it->specs().rect(convert_rect(rect));

				main_menu_item.rc_text = it->specs().rect();

				left = it->specs().rect().right() + _form_menu_margin;

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
						mi.image_file = item.image_file;
						mi.image_png_resource = item.image_png_resource;
						menu_specs.items.push_back(mi);
					}

					menu_specs.pin = main_menu_item.rc_text;
					menu_specs.type = context_menu::pin_type::bottom;

					POINT pt = { 0, 0 };
					ClientToScreen(_hWnd, &pt);

					menu_specs.pin.left() += (pt.x / get_dpi_scale());
					menu_specs.pin.right() += (pt.x / get_dpi_scale());
					menu_specs.pin.top() += (pt.y / get_dpi_scale());
					menu_specs.pin.bottom() += (pt.y / get_dpi_scale());

					auto result = context_menu()(_fm, menu_specs);
					
					for (auto& m_it : main_menu_item.items) {
						if (m_it.label == result) {
							if (m_it.action != nullptr)
								m_it.action();
						}
					}
				};
			}
		}

		void form::impl::update() { InvalidateRect(_hWnd, nullptr, FALSE); }

		/// The tree view widget is constructed as follows:
		/// 1. A special pane is made for carrying the tree view
		/// 2. The tree view widget is moved into the container
		void form::impl::move_trees() {
			// check if this page has a tree pane
			auto page_iterator = _p_pages.find(_current_page);

			struct tree_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::tree_view tree;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				containers::pane_specs& destination_specs;
			};

			std::vector<tree_info> trees;

			if (page_iterator != _p_pages.end()) {
				auto& page = page_iterator->second;

				class helper {
				public:
					static void find_trees_to_move(lecui::containers::page& page,
						std::vector<tree_info>& trees) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is a tree pane
							if (alias.find(widgets::pane_impl::tree_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a tree pane (it has a tree inside. move was already done), continue to next widget

							// check if this is a tree
							if (widget.type() == widgets::widget_type::tree_view) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_trees_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_trees_to_move(page.second, trees);	// recursion
								}
						}

						if (!booked.empty()) {
							// this is a tree, we need to "move" it into a special pane

							// get the tree widget
							auto& tree_specs = page._d_page.get_tree_view_impl(booked).specs();

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::tree_pane_alias_prefix() + booked);

							// clone essential properties to pane
							pane
								.rect(tree_specs.rect())
								.on_resize(tree_specs.on_resize())
								.color_fill(tree_specs.color_fill())
								.color_border(tree_specs.color_border())
								.border(tree_specs.border());

							// save move info so we can move the tree into the pane later
							// we cannot do it here because we're iterating
							trees.push_back({ booked, tree_specs, page, pane, pane });
						}
					}
				};

				helper::find_trees_to_move(page, trees);

				// move the trees
				for (auto& it : trees) {
					log("moving tree: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// clone into destination
						auto& tree = widgets::tree_view::add(it.destination, it.alias);
						// copy widget
						tree = it.tree;

						// adjust widget
						tree
							.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width())
								.top(0.f)
								.bottom(it.destination.size().get_height()))
							.on_resize({ 0.f, 0.f, 0.f, 0.f });	// critical because tree will change size as tree is browsed or changed. the pane scroll bars will do the job.

						// capture tree pane widget
						it.destination._d_page.get_tree_view_impl(it.alias).set_tree_pane_specs(it.destination_specs);

						// copy pointer to pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						tree._p_special_pane_specs = &it.destination_specs;

						// close widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::tree_view, error);
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
			struct html_editor_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::html_editor html_editor;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				containers::pane_specs& html_control_pane_specs;
				containers::pane_specs& html_pane_specs;
			};

			auto do_move_html_editors = [this](containers::page& page) {
				class move_helper {
				public:
					static void find_html_editors_to_move(lecui::containers::page& page,
						std::vector<html_editor_info>& trees) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is an html pane
							if (alias.find(widgets::pane_impl::html_pane_alias_prefix()) != std::string::npos)
								continue;	// this is an html pane (it has an html widget inside. move was already done), continue to next widget

							// check if this is an html editor
							if (widget.type() == widgets::widget_type::html_editor) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_html_editors_to_move(tab.second, trees);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_html_editors_to_move(page.second, trees);	// recursion
								}
						}

						if (!booked.empty()) {
							// this is an html editor, we need to "move" it into a special pane

							// get the html editor widget
							auto& html_editor_specs = page._d_page.get_html_editor_impl(booked).specs();

							// make controls pane in source (predefined, fixed height)
							auto& controls_pane = containers::pane::add(page, widgets::pane_impl::html_controls_pane_alias_prefix() + booked);
							controls_pane.rect(html_editor_specs.rect());
							controls_pane.rect().height(
								// to-do: eliminate magic numbers
								(10.f * 2) +	// top and bottom margin
								25.f +			// first row (font name, font size ...)
								5.f + 20.f		// seond row (bold, italic ...)
							);
							controls_pane
								.color_fill(html_editor_specs.color_control_fill())
								.color_border(html_editor_specs.color_control_border())
								.border(html_editor_specs.control_border())
								.on_resize(html_editor_specs.on_resize())
								.on_resize().height_rate(0.f).min_height(0.f);

							if (html_editor_specs._view_only) {
								// to-do: remove controls pane completely after finding a way of handling the redirection in the html_editor.cpp
								
								// make pane of zero height and zero width
								controls_pane.rect().width(0.f);
								controls_pane.rect().height(0.f);

								// make pane invisible
								controls_pane
									.border(0.f);
								controls_pane
									.color_fill().alpha(0);
							}

							// cause controls pane to be initialized by calling get()
							containers::page& controls_pane_page = controls_pane;

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::html_pane_alias_prefix() + booked);

							// clone essential properties to pane
							pane
								.color_fill(html_editor_specs.color_fill())
								.color_border(html_editor_specs.color_border())
								.border(html_editor_specs.border())
								.rect(html_editor_specs.rect())
								.on_resize(html_editor_specs.on_resize())
								.rect().top(html_editor_specs._view_only ? controls_pane.rect().top() : controls_pane.rect().bottom());

							if (pane.on_resize().min_height())
								pane.on_resize().min_height(largest(pane.on_resize().min_height() - controls_pane.rect().height(), 0.f));

							// save move info so we can move the tree into the pane later
							// we cannot do it here because we're iterating
							trees.push_back({ booked, html_editor_specs, page, pane, controls_pane, pane });
						}
					}
				};

				std::vector<html_editor_info> html_editors;
				move_helper::find_html_editors_to_move(page, html_editors);

				// move the html editors
				for (auto& it : html_editors) {
					log("moving html_editor: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// clone into destination
						auto& html_editor = widgets::html_editor::add(it.destination, it.alias);
						// copy widget
						html_editor = it.html_editor;

						// adjust widget
						html_editor
							.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width())
								.top(0.f)
								.bottom(it.destination.size().get_height()))
							.on_resize({ 0.f, 0.f, 100.f, 0.f });	// critical because html_editor will change height as user types or contents are changed. the pane scroll bars will do the job.

						// capture html pane widget
						it.destination._d_page.get_html_editor_impl(it.alias).set_pane_specs(it.html_control_pane_specs, it.html_pane_specs);

						// copy pointer to controls pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						html_editor._p_controls_pane_specs = &it.html_control_pane_specs;

						// copy pointer to html pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						html_editor._p_html_pane_specs = &it.html_pane_specs;

						// get overall widget resize parameters
						html_editor._on_resize_overall = it.html_editor._on_resize;

						// get overall widget rectangle
						html_editor._rect_widget_overall = it.html_control_pane_specs.rect();
						html_editor._rect_widget_overall.bottom(it.html_pane_specs.rect().bottom());

						// close the widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::html_editor, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_html_controls(lecui::containers::page& page) {
						for (auto& widget : page._d_page.widgets()) {
							if (widget.first.find(widgets::pane_impl::html_controls_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated html editor widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get the pages
										auto& html_controls_page = page._d_page.get_pane_impl(widgets::pane_impl::html_controls_pane_alias_prefix() + widget_alias)._p_panes.at("pane");
										auto& html_page = page._d_page.get_pane_impl(widgets::pane_impl::html_pane_alias_prefix() + widget_alias)._p_panes.at("pane");

										// get the html editor and controls
										auto& html_editor = html_page._d_page.get_html_editor_impl(widget_alias);

										if (!html_editor.controls_initialized()) {
											if (!html_editor.specs()._view_only) {
												/// add controls to controls pane

												/// add basic font formatting

												/// add font selection combobox
												auto& font = widgets::combobox::add(html_controls_page, html_editor.alias_font());
												font.rect({ 0.f, 150.f, 0.f, 25.f })
													.editable(false);

												// get list of fonts
												const auto& font_list = font::available_fonts();

												for (const auto& font_name : font_list) {
													widgets::combobox::combobox_item item;
													item.label = font_name;
													item.font = font_name;	// essential for preview
													font.items().push_back(item);
												}

												font
													.selected(html_editor.specs().font())
													.events().selection = [&](const std::string& font_name) {
													html_editor.selection_font(font_name);
												};

												/// add bold control
												auto& bold = widgets::rectangle::add(html_controls_page, html_editor.alias_bold());
												bold.rect().size(20.f, 20.f);
												bold.rect().snap_to(font.rect(), rect::snap_type::bottom_left, 5.f);
												bold.color_fill().alpha(0);
												bold.color_border().alpha(0);
												bold.events().action = [&]() {
													html_editor.selection_bold();
												};

												auto& bold_label = widgets::label::add(html_controls_page);
												bold_label
													.rect(bold.rect())
													.text("<strong>B</strong>")
													.font_size(11.f)
													.alignment(text_alignment::center)
													.paragraph_alignment(paragraph_alignment::middle);

												/// add italic control
												auto& italic = widgets::rectangle::add(html_controls_page, html_editor.alias_italic());
												italic.rect().size(20.f, 20.f);
												italic.rect().snap_to(bold.rect(), rect::snap_type::right, 5.f);
												italic.color_fill().alpha(0);
												italic.color_border().alpha(0);
												italic.events().action = [&]() {
													html_editor.selection_italic();
												};

												auto& italic_label = widgets::label::add(html_controls_page);
												italic_label
													.rect(italic.rect())
													.text("<em><strong>I</strong></em>")
													.font_size(11.f)
													.alignment(text_alignment::center)
													.paragraph_alignment(paragraph_alignment::middle);

												/// add underline control
												auto& underline = widgets::rectangle::add(html_controls_page, html_editor.alias_underline());
												underline.rect().size(20.f, 20.f);
												underline.rect().snap_to(italic_label.rect(), rect::snap_type::right, 5.f);
												underline.color_fill().alpha(0);
												underline.color_border().alpha(0);
												underline.events().action = [&]() {
													html_editor.selection_underline();
												};

												auto& underline_label = widgets::label::add(html_controls_page);
												underline_label
													.rect(underline.rect())
													.text("<u>U</u>")
													.font_size(11.f)
													.alignment(text_alignment::center)
													.paragraph_alignment(paragraph_alignment::middle);

												/// add strikethrough control
												auto& strikethrough = widgets::rectangle::add(html_controls_page, html_editor.alias_strikethrough());
												strikethrough.rect().size(30.f, 20.f);
												strikethrough.rect().snap_to(underline_label.rect(), rect::snap_type::right, 5.f);
												strikethrough.color_fill().alpha(0);
												strikethrough.color_border().alpha(0);
												strikethrough.events().action = [&]() {
													html_editor.selection_strikethrough();
												};

												auto& strikethrough_label = widgets::label::add(html_controls_page);
												strikethrough_label
													.rect(strikethrough.rect())
													.text("<s>abc</s>")
													.font_size(11.f)
													.alignment(text_alignment::center)
													.paragraph_alignment(paragraph_alignment::middle);

												/// add color control
												auto& font_color = widgets::rectangle::add(html_controls_page, html_editor.alias_font_color());
												font_color.rect().size(20.f, 20.f);
												font_color.rect().snap_to(strikethrough.rect(), rect::snap_type::right, 5.f);
												font_color.color_fill().alpha(0);
												font_color.color_border().alpha(0);
												font_color.events().action = [&]() {
													html_editor.selection_color();
												};

												auto& font_color_label = widgets::label::add(html_controls_page);
												font_color_label
													.text("<strong>A</strong>")
													.font_size(9.5f)
													.alignment(text_alignment::center)
													.paragraph_alignment(paragraph_alignment::middle)
													.rect(font_color.rect())
													.rect().bottom() -= 5.f;

												auto& font_color_bar = widgets::rectangle::add(html_controls_page, html_editor.alias_font_color_bar());
												font_color_bar
													.color_fill(html_editor.get_last_color())
													.border(.2f)
													.rect(font_color.rect())
													.rect().top(font_color_label.rect().bottom());
												font_color_bar.rect().bottom() -= 1.f;
												font_color_bar.rect().left() += 2.f;
												font_color_bar.rect().right() -= 2.f;


												auto& font_color_menu = widgets::rectangle::add(html_controls_page);
												font_color_menu.rect().size(10.f, 20.f);
												font_color_menu.rect().snap_to(font_color.rect(), rect::snap_type::right, 2.f);
												font_color_menu.color_fill().alpha(0);
												font_color_menu.color_border().alpha(0);
												font_color_menu.events().action = [&]() {
													color font_color;
													if (color_picker(page._d_page._fm).pick(font_color)) {
														html_editor.selection_color(font_color);

														try {
															// update font color bar
															auto& font_color_bar_impl = html_controls_page._d_page.get_rectangle_impl(html_editor.alias_font_color_bar());
															font_color_bar_impl.specs().color_fill(font_color);
														}
														catch (const std::exception&) {}

														page._d_page._fm.update();
													}
												};

												auto& font_color_menu_dropdown = widgets::line::add(html_controls_page);
												font_color_menu_dropdown.rect().size(font_color_menu.rect().width(), font_color_menu.rect().width());	// important for it to be a square
												font_color_menu_dropdown.rect().place(font_color_menu.rect(), 50.f, 50.f);
												font_color_menu_dropdown.points().push_back({ .1f * font_color_menu_dropdown.rect().width(), .3f * font_color_menu_dropdown.rect().height() });
												font_color_menu_dropdown.points().push_back({ .5f * font_color_menu_dropdown.rect().width(), .7f * font_color_menu_dropdown.rect().height() });
												font_color_menu_dropdown.points().push_back({ .9f * font_color_menu_dropdown.rect().width(), .3f * font_color_menu_dropdown.rect().height() });

												/// add font size combobox
												auto& font_size = widgets::combobox::add(html_controls_page, html_editor.alias_font_size());
												font_size.rect().size(70.f, 25.f);
												font_size.rect().snap_to(font.rect(), rect::snap_type::right, 10.f);
												font_size
													.editable(true)
													.force_numerical_sort(true)
													.items({
													{"8", "Segoe UI", 8 },
													{"9", "Segoe UI", 9},
													{"10", "Segoe UI", 10},
													{"11", "Segoe UI", 11},
													{"12", "Segoe UI", 12},
													{"13", "Segoe UI", 13},
													{"14", "Segoe UI", 14},
													{"18", "Segoe UI", 18},
													{"24", "Segoe UI", 24},
													{"36", "Segoe UI", 36} })
													.text(round_off::to_string(html_editor.specs().font_size(), 1))
													.events().selection = [&](const std::string& font_size) {
													float _font_size = 0.f;
													std::stringstream ss;
													ss << font_size;
													ss >> _font_size;
													html_editor.selection_font_size(_font_size);
												};

												/// increase font size control
											}

											html_editor.initialize_controls(true);
										}
									}
								}
								catch (const std::exception& e) { log(e.what()); }
							}
							else
								if (widget.second.type() == widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane._p_tabs)
										add_html_controls(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane_impl(widget.first);

										// initialize panes
										for (auto& page : pane._p_panes)
											add_html_controls(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_html_controls(page);
			};

			// check if this page has html widgets and move them if they exist
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end()) {
				auto& page = page_iterator->second;
				do_move_html_editors(page);
			}

			// check if status panes have html editors and move them if they exist
			for (auto& [alias, page] : _p_status_panes)
				do_move_html_editors(page);
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
			struct time_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::time time;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				lecui::containers::pane_specs& destination_specs;
			};

			auto do_move_times = [this](containers::page& page) {
				class helper {
				public:
					static void find_times_to_move(lecui::containers::page& page,
						std::vector<time_info>& times) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is a time pane
							if (alias.find(widgets::pane_impl::time_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a time pane (it has a time widget inside. move was already done), continue to next widget

							// check if this is a time widget
							if (widget.type() == widgets::widget_type::time) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_times_to_move(tab.second, times);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_times_to_move(page.second, times);	// recursion
								}
						}

						if (!booked.empty()) {
							// this is a time widget, we need to "move" it into a special pane

							// get the time widget
							auto& time = page._d_page.get_time_impl(booked).specs();

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::time_pane_alias_prefix() + booked, 0.f);

							// clone essential properties to pane
							pane
								.rect(time.rect())
								.on_resize(time.on_resize());
							pane.color_fill().alpha(0);
							pane.color_border().alpha(0);

							// save move info so we can move the tree into the pane later
							// we cannot do it here because we're iterating
							times.push_back({ booked, time, page, pane, pane });
						}
					}
				};

				std::vector<time_info> times;
				helper::find_times_to_move(page, times);

				// move times
				for (auto& it : times) {
					log("moving time: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// clone into destination
						auto& time = widgets::time::add(it.destination, it.alias);
						// copy widget
						time = it.time;

						// adjust widget
						time
							.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width())
								.top(0.f)
								.bottom(it.destination.size().get_height()))
							.on_resize({ 0.f, 0.f, 0.f, 0.f });

						// add hour destination
						auto& hour = widgets::rectangle::add(it.destination, widgets::time_impl::alias_hour());
						hour
							.tooltip(time.tooltip())
							.rect({ 0, 18, 0, 20 })
							.on_resize({ 0, 0, 0, 0 })
							.corner_radius_x(time.corner_radius_x())
							.corner_radius_y(time.corner_radius_y())
							.border(time.border())
							.color_fill(time.color_fill())
							.color_border(time.color_border())
							.color_disabled(time.color_disabled())
							.color_selected(time.color_selected());

						auto& hour_label = widgets::label::add(it.destination, widgets::time_impl::alias_hour_label());
						hour_label
							.rect(hour.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(time.color_text())
							.text(time.time_value().hour < 10 ? "0" + std::to_string(time.time_value().hour) :
								std::to_string(time.time_value().hour));

						// add seperator to destination
						auto& seperator_1 = widgets::label::add(it.destination);
						seperator_1
							.on_resize({ 0, 0, 0, 0 })
							.text(":")
							.color_text(time.color_text())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.rect({ 0, 8, 0, 20 })
							.rect().snap_to(hour.rect(), rect::snap_type::right, 0.f);

						// add minute to destination
						auto& minute = widgets::rectangle::add(it.destination, widgets::time_impl::alias_minute());
						minute
							.tooltip(time.tooltip())
							.corner_radius_x(time.corner_radius_x())
							.corner_radius_y(time.corner_radius_y())
							.border(time.border())
							.color_fill(time.color_fill())
							.color_border(time.color_border())
							.color_disabled(time.color_disabled())
							.color_selected(time.color_selected())
							.rect({ 0, 18, 0, 20 })
							.rect().snap_to(seperator_1.rect(), rect::snap_type::right, 0.f);

						auto& minute_label = widgets::label::add(it.destination, widgets::time_impl::alias_minute_label());
						minute_label
							.rect(minute.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(time.color_text())
							.text(time.time_value().minute < 10 ? "0" + std::to_string(time.time_value().minute) :
								std::to_string(time.time_value().minute));

						// add seperator to destination
						auto& seperator_2 = widgets::label::add(it.destination);
						seperator_2
							.on_resize({ 0, 0, 0, 0 })
							.text(":")
							.color_text(time.color_text())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.rect({ 0, 8, 0, 20 })
							.rect().snap_to(minute.rect(), rect::snap_type::right, 0.f);

						// add second to destination
						auto& second = widgets::rectangle::add(it.destination, widgets::time_impl::alias_second());
						second
							.tooltip(time.tooltip())
							.corner_radius_x(time.corner_radius_x())
							.corner_radius_y(time.corner_radius_y())
							.border(time.border())
							.color_fill(time.color_fill())
							.color_border(time.color_border())
							.color_disabled(time.color_disabled())
							.color_selected(time.color_selected())
							.rect({ 0, 18, 0, 20 })
							.rect().snap_to(seperator_2.rect(), rect::snap_type::right, 0.f);

						auto& second_label = widgets::label::add(it.destination, widgets::time_impl::alias_second_label());
						second_label
							.rect(second.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(time.color_text())
							.text(time.time_value().second < 10 ? "0" + std::to_string(time.time_value().second) :
								std::to_string(time.time_value().second));

						// capture time label widget
						it.destination._d_page.get_time_impl(it.alias).set_time_label_specs(
							hour_label, seperator_1, minute_label, seperator_2, second_label);

						// capture rectangle widget
						it.destination._d_page.get_time_impl(it.alias).set_time_specs(hour, minute, second);

						// copy pointer to pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						time._p_special_pane_specs = &it.destination_specs;

						// close widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::time, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_times(lecui::containers::page& page) {
						for (auto& widget : page._d_page.widgets()) {
							if (widget.first.find(widgets::pane_impl::time_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated time widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get time pane
										auto& time_page = page._d_page.get_pane_impl(widgets::pane_impl::time_pane_alias_prefix() + widget_alias)._p_panes.at("pane");

										// get time widget widget
										auto& specs = time_page._d_page.get_time_impl(widget_alias).specs();

										// get hour
										auto& hour = time_page._d_page.get_rectangle_impl(widgets::time_impl::alias_hour());
										auto& hour_lbl = time_page._d_page.get_label_impl(widgets::time_impl::alias_hour_label());

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

												auto selected = context_menu()(time_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													hour_lbl().text(selected);
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value().hour;

													if (specs.events().change)
														specs.events().change(specs.time_value());
												}
											};
										}

										// get minute label
										auto& minute = time_page._d_page.get_rectangle_impl(widgets::time_impl::alias_minute());
										auto& minute_lbl = time_page._d_page.get_label_impl(widgets::time_impl::alias_minute_label());

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

												auto selected = context_menu()(time_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													minute_lbl().text(selected);
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value().minute;

													if (specs.events().change)
														specs.events().change(specs.time_value());
												}
											};
										}

										// get second label
										auto& second = time_page._d_page.get_rectangle_impl(widgets::time_impl::alias_second());
										auto& second_lbl = time_page._d_page.get_label_impl(widgets::time_impl::alias_second_label());

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

												auto selected = context_menu()(time_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													second_lbl().text(selected);
													std::stringstream ss;
													ss << selected;
													ss >> specs.time_value().second;

													if (specs.events().change)
														specs.events().change(specs.time_value());
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
									auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane._p_tabs)
										add_times(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane_impl(widget.first);

										// initialize panes
										for (auto& page : pane._p_panes)
											add_times(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_times(page);
			};

			// check if this page has time widgets and move them if they exist
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end()) {
				auto& page = page_iterator->second;
				do_move_times(page);
			}

			// check if status panes have time widgets and move them if they exist
			for (auto& [alias, page] : _p_status_panes)
				do_move_times(page);
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
			struct date_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::date date;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				lecui::containers::pane_specs& destination_specs;
			};

			auto do_move_dates = [this](lecui::containers::page& page) {
				class helper {
				public:
					static void find_dates_to_move(lecui::containers::page& page,
						std::vector<date_info>& dates) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is a date pane
							if (alias.find(widgets::pane_impl::date_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a date pane (it has a date widget inside. move was already done), continue to next widget

							// check if this is a date widget
							if (widget.type() == widgets::widget_type::date) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_dates_to_move(tab.second, dates);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_dates_to_move(page.second, dates);	// recursion
								}
						}

						if (!booked.empty()) {
							// this is a date widget, we need to "move" it into a special pane

							// get the date widget
							auto& date_specs = page._d_page.get_date_impl(booked).specs();

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::date_pane_alias_prefix() + booked, 0.f);

							// clone essential properties to pane
							pane
								.rect(date_specs.rect())
								.on_resize(date_specs.on_resize());
							pane.color_fill().alpha(0);
							pane.color_border().alpha(0);

							// save move info so we can move the tree into the pane later
							// we cannot do it here because we're iterating
							dates.push_back({ booked, date_specs, page, pane, pane });
						}
					}
				};

				std::vector<date_info> dates;
				helper::find_dates_to_move(page, dates);

				// move dates
				for (auto& it : dates) {
					log("moving date: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// clone into destination
						auto& date = widgets::date::add(it.destination, it.alias);
						// copy widget
						date = it.date;

						// impose limits
						date.date_value().year = largest(date.date_value().year, (unsigned short)1970);

						// adjust widget
						date.rect(rect()
							.left(0.f)
							.right(it.destination.size().get_width())
							.top(0.f)
							.bottom(it.destination.size().get_height()));
						date.on_resize({ 0.f, 0.f, 0.f, 0.f });
						date.color_fill().alpha(0);

						// add week day to destination
						auto& weekday_label = widgets::label::add(it.destination, widgets::date_impl::alias_weekday_label());
						weekday_label
							.rect({ 0, 90, 0, 20 })
							.on_resize({ 0, 0, 0, 0 })
							.color_text(date.color_text())
							.text(date_time::weekday_to_string(date_time::day_of_week(date.date_value())))
							.font_size(8.5f);	// to-do: eliminate magic number

						// add day to destination
						auto& day = widgets::rectangle::add(it.destination, widgets::date_impl::alias_day());
						day
							.tooltip(date.tooltip())
							.rect({ 0, 18, 20, 20 + 20 })
							.on_resize({ 0, 0, 0, 0 })
							.corner_radius_x(date.corner_radius_x())
							.corner_radius_y(date.corner_radius_y())
							.border(date.border())
							.color_fill(date.color_fill())
							.color_border(date.color_border())
							.color_disabled(date.color_disabled())
							.color_selected(date.color_selected());

						auto& day_label = widgets::label::add(it.destination, widgets::date_impl::alias_day_label());
						day_label
							.rect(day.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(date.color_text())
							.text(date.date_value().day < 10 ? "0" + std::to_string(date.date_value().day) :
								std::to_string(date.date_value().day));

						// add seperator to destination
						auto& seperator_1 = widgets::label::add(it.destination);
						seperator_1
							.on_resize({ 0, 0, 0, 0 })
							.text("-")
							.color_text(date.color_text())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.rect({ 0, 8, 0, 20 })
							.rect().snap_to(day.rect(), rect::snap_type::right, 0.f);

						// add month to destination
						auto& month = widgets::rectangle::add(it.destination, widgets::date_impl::alias_month());
						month
							.tooltip(date.tooltip())
							.corner_radius_x(date.corner_radius_x())
							.corner_radius_y(date.corner_radius_y())
							.border(date.border())
							.color_fill(date.color_fill())
							.color_border(date.color_border())
							.color_disabled(date.color_disabled())
							.color_selected(date.color_selected())
							.rect({ 0, 25, 0, 20 })
							.rect().snap_to(seperator_1.rect(), rect::snap_type::right, 0.f);

						auto& month_label = widgets::label::add(it.destination, widgets::date_impl::alias_month_label());
						month_label
							.rect(month.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(date.color_text())
							.text(date_time::month_to_string(date.date_value().month));

						// add seperator to destination
						auto& seperator_2 = widgets::label::add(it.destination);
						seperator_2
							.on_resize({ 0, 0, 0, 0 })
							.text("-")
							.color_text(date.color_text())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.rect({ 0, 8, 0, 20 })
							.rect().snap_to(month.rect(), rect::snap_type::right, 0.f);

						// add year to destination
						auto& year = widgets::rectangle::add(it.destination, widgets::date_impl::alias_year());
						year
							.tooltip(date.tooltip())
							.corner_radius_x(date.corner_radius_x())
							.corner_radius_y(date.corner_radius_y())
							.border(date.border())
							.color_fill(date.color_fill())
							.color_border(date.color_border())
							.color_disabled(date.color_disabled())
							.color_selected(date.color_selected())
							.rect({ 0, 31, 0, 20 })
							.rect().snap_to(seperator_2.rect(), rect::snap_type::right, 0.f);


						auto& year_label = widgets::label::add(it.destination, widgets::date_impl::alias_year_label());
						year_label
							.rect(year.rect())
							.alignment(text_alignment::center)
							.paragraph_alignment(paragraph_alignment::middle)
							.on_resize({ 0, 0, 0, 0 })
							.color_text(date.color_text())
							.text(std::to_string(date.date_value().year));

						// capture date label widget
						it.destination._d_page.get_date_impl(it.alias).set_date_label_specs(
							weekday_label, day_label, seperator_1, month_label, seperator_2, year_label);

						// capture rectangle widget
						it.destination._d_page.get_date_impl(it.alias).set_date_specs(day, month, year);

						// copy pointer to pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						date._p_special_pane_specs = &it.destination_specs;

						// close widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::date, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}

				class controls_helper {
				public:
					static void add_dates(lecui::containers::page& page) {
						for (auto& widget : page._d_page.widgets()) {
							if (widget.first.find(widgets::pane_impl::date_pane_alias_prefix()) != std::string::npos) {
								try {
									// get alias of associated date widget
									const auto idx = widget.first.rfind("::");

									if (idx != std::string::npos) {
										auto widget_alias = widget.first.substr(idx + 2);

										// get date pane
										auto& date_page = page._d_page.get_pane_impl(widgets::pane_impl::date_pane_alias_prefix() + widget_alias)._p_panes.at("pane");

										// get date widget widget
										auto& specs = date_page._d_page.get_date_impl(widget_alias).specs();

										// get weekday label
										auto& weekday_lbl = date_page._d_page.get_label_impl(widgets::date_impl::alias_weekday_label());

										// get day
										auto& day = date_page._d_page.get_rectangle_impl(widgets::date_impl::alias_day());
										auto& day_lbl = date_page._d_page.get_label_impl(widgets::date_impl::alias_day_label());

										if (day().events().action == nullptr) {
											day().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												for (int i = 1; i < date_time::last_day_of_month(specs.date_value()).day + 1; i++) {
													std::string dy = std::to_string(i);
													if (i < 10)
														dy = "0" + dy;
													menu_specs.items.push_back({ dy });
												}

												auto selected = context_menu()(date_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													day_lbl().text(selected);
													std::stringstream ss;
													ss << selected;
													ss >> specs.date_value().day;

													// update weekday
													weekday_lbl().text(date_time::weekday_to_string(date_time::day_of_week(specs.date_value())));

													if (specs.events().change)
														specs.events().change(specs.date_value());
												}
											};
										}

										// get month label
										auto& month = date_page._d_page.get_rectangle_impl(widgets::date_impl::alias_month());
										auto& month_lbl = date_page._d_page.get_label_impl(widgets::date_impl::alias_month_label());

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

												auto selected = context_menu()(date_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													month_lbl().text(selected);
													specs.date_value().month = date_time::month_from_string(selected);

													// ensure day is within range
													specs.date_value().day = smallest(specs.date_value().day,
														date_time::last_day_of_month({ 1, specs.date_value().month, specs.date_value().year }).day);
													day_lbl().text(std::to_string(specs.date_value().day));
													for (size_t i = day_lbl().text().length(); i < 2; i++) day_lbl().text("0" + day_lbl().text());

													// update weekday
													weekday_lbl().text(date_time::weekday_to_string(date_time::day_of_week(specs.date_value())));

													if (specs.events().change)
														specs.events().change(specs.date_value());
												}
											};
										}

										// get year label
										auto& year = date_page._d_page.get_rectangle_impl(widgets::date_impl::alias_year());
										auto& year_lbl = date_page._d_page.get_label_impl(widgets::date_impl::alias_year_label());

										if (year().events().action == nullptr) {
											year().events().action = [&]() {
												context_menu::specs menu_specs;
												menu_specs.type = context_menu::pin_type::right;

												// to-do: make possible to use year prior to 1970 (mktime issue)
												for (int i = 1970; i < 2100; i++) {
													std::string yr = std::to_string(i);
													menu_specs.items.push_back({ yr });
												}

												auto selected = context_menu()(date_page._d_page.get_form(), menu_specs);

												if (!selected.empty()) {
													year_lbl().text(selected);
													std::stringstream ss;
													ss << selected;
													ss >> specs.date_value().year;

													// ensure day is within range
													specs.date_value().day = smallest(specs.date_value().day,
														date_time::last_day_of_month({ 1, specs.date_value().month, specs.date_value().year }).day);
													day_lbl().text(std::to_string(specs.date_value().day));
													for (size_t i = day_lbl().text().length(); i < 2; i++) day_lbl().text("0" + day_lbl().text());

													// update weekday
													weekday_lbl().text(date_time::weekday_to_string(date_time::day_of_week(specs.date_value())));

													if (specs.events().change)
														specs.events().change(specs.date_value());
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
									auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

									// initialize tabs
									for (auto& tab : tab_pane._p_tabs)
										add_dates(tab.second);	// recursion
								}
								else
									if (widget.second.type() == widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane_impl(widget.first);

										// initialize panes
										for (auto& page : pane._p_panes)
											add_dates(page.second);	// recursion
									}
						}
					}
				};

				controls_helper::add_dates(page);
			};

			// check if this page has a date widget
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end()) {
				auto& page = page_iterator->second;
				do_move_dates(page);
			}

			// check if status panes have date widgets and move them if they exist
			for (auto& [alias, page] : _p_status_panes)
				do_move_dates(page);
		}

		/// The icon widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. A rectangle is placed into the pane, this is for hit testing
		/// 3. An image view is added above the rectangle
		/// 4. Two labels are added beside the image view, one is the icon text and the other is
		/// the descriptive text
		void form::impl::move_icons() {
			struct icon_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::icon icon;
				lecui::containers::page& source;
				lecui::containers::page& destination;
				lecui::containers::pane_specs& destination_specs;
			};

			auto do_move_icons = [this](lecui::containers::page& page) {
				class helper {
				public:
					static void find_icons_to_move(lecui::containers::page& page,
						std::vector<icon_info>& icons) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is a icon pane
							if (alias.find(widgets::pane_impl::icon_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a icon pane (it has a icon widget inside. move was already done), continue to next widget

							// check if this is a icon widget
							if (widget.type() == widgets::widget_type::icon) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_icons_to_move(tab.second, icons);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_icons_to_move(page.second, icons);	// recursion
								}
						}

						// do booked move
						if (!booked.empty()) {
							// this is a icon widget, we need to "move" it into a special pane

							// get the icon widget
							auto& icon_specs = page._d_page.get_icon_impl(booked).specs();

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::icon_pane_alias_prefix() + booked, 0.f);

							// clone essential properties to pane
							pane
								.rect(icon_specs.rect())
								.on_resize(icon_specs.on_resize());
							pane.color_fill().alpha(0);
							pane.color_border().alpha(0);

							// save move info so we can move the tree into the pane later
							// we cannot do it here because we're iterating
							icons.push_back({ booked, icon_specs, page, pane, pane });
						}
					}
				};

				std::vector<icon_info> icons;
				helper::find_icons_to_move(page, icons);

				// move icons
				for (auto& it : icons) {
					log("moving icon: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// add rectangle (for hit-testing)
						auto& icn = widgets::rectangle::add(it.destination, widgets::icon_impl::alias_icon());

						// add image
						auto& image = widgets::image_view::add(it.destination, widgets::icon_impl::alias_image());

						// add text
						auto& text = widgets::label::add(it.destination, widgets::icon_impl::alias_text());

						// add description
						auto& description = widgets::label::add(it.destination, widgets::icon_impl::alias_description());

						// clone into destination (do this last so badge will be on top since badge is the only thing that remains)
						auto& icon = widgets::icon::add(it.destination, it.alias);

						// copy widget
						icon = it.icon;

						// adjust widget
						icon
							.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width())
								.top(0.f)
								.bottom(it.destination.size().get_height()))
							.on_resize(resize_params()
								.width_rate(100.f)
								.height_rate(100.f));

						// rectangle specs
						icn
							.tooltip(icon.tooltip())
							.corner_radius_x(icon.corner_radius_x())
							.corner_radius_y(icon.corner_radius_y())
							.border(icon.border())
							.color_fill(icon.color_fill())
							.color_border(icon.color_border())
							.color_hot(icon.color_hot())
							.cursor(widgets::widget::cursor_type::hand)
							.on_resize(resize_params()
								.width_rate(100.f)
								.height_rate(100.f))
							.rect().size(it.destination.size());

						// move the click and action handler from the icon to the rectangle
						icn.events().action = icon.events().action;
						icn.events().click = icon.events().click;
						icon.events().action = nullptr;
						icon.events().click = nullptr;

						// move the opacity from the icon to the image
						image.opacity(icon.opacity());
						icon.opacity(100.f);

						// compute size of image side
						auto image_side = smallest(it.destination.size().get_width(), it.destination.size().get_height()) - 2.f * icon._padding;
						if (icon.max_image_size() > 0.f)
							image_side = smallest(image_side, icon.max_image_size());

						// image specs
						image
							.rect({ 0, image_side, 0, image_side });

						const auto ref_rect = rect()
							.left(icon._padding)
							.right(it.destination.size().get_width() - icon._padding)
							.top(icon._padding)
							.bottom(it.destination.size().get_height() - icon._padding);

						switch (icon.text_position()) {
						case widgets::icon::icon_text_position::right:
							image.rect().place(ref_rect, 0.f, 0.f);
							break;
						case widgets::icon::icon_text_position::left:
							image.rect().place(ref_rect, 100.f, 0.f);
							break;
						case widgets::icon::icon_text_position::top:
							image.rect().place(ref_rect, 50.f, 100.f);
							break;
						case widgets::icon::icon_text_position::bottom:
							image.rect().place(ref_rect, 50.f, 0.f);
							break;
						default:
							break;
						}

						image.file(icon.file());
						image.png_resource(icon.png_resource());
						image.quality(icon.quality());

						// text specs
						text
							.text(icon.text())
							.color_text(icon.color_text())
							.font_size(icon.font_size())
							.on_resize(resize_params().width_rate(100.f));

						switch (icon.text_position()) {
						case widgets::icon::icon_text_position::right: {
							text.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width() - (image_side + icon._gap) - icon._padding)
								.top(0.f)
								.bottom(it.destination.size().get_height()));
							const auto ideal_rect = widgets::measure_label(_p_directwrite_factory, text.text(), text.font(), text.font_size(),
								text_alignment::left, paragraph_alignment::top, convert_rect(text.rect()));
							text.rect().height(ideal_rect.bottom - ideal_rect.top);
							text.rect().snap_to(image.rect(), rect::snap_type::right_top, icon._gap);
						}
																	 break;
						case widgets::icon::icon_text_position::left: {
							text.rect(rect()
								.left(icon._padding)
								.right(it.destination.size().get_width() - (image_side + icon._gap) - icon._padding)
								.top(icon._padding)
								.bottom(it.destination.size().get_height()));
							const auto ideal_rect = widgets::measure_label(_p_directwrite_factory, text.text(), text.font(), text.font_size(),
								text_alignment::left, paragraph_alignment::top, convert_rect(text.rect()));
							text.rect().height(ideal_rect.bottom - ideal_rect.top);
							text.rect().snap_to(image.rect(), rect::snap_type::left_top, icon._gap);
						}
																	break;
						case widgets::icon::icon_text_position::top: {
							text.rect(rect()
								.left(icon._padding)
								.right(it.destination.size().get_width() - icon._padding)
								.top(0.f)
								.bottom(it.destination.size().get_height() - icon._padding));
							const auto ideal_rect = widgets::measure_label(_p_directwrite_factory, text.text(), text.font(), text.font_size(),
								text_alignment::left, paragraph_alignment::top, convert_rect(text.rect()));
							text.rect().height(ideal_rect.bottom - ideal_rect.top);
							text.alignment(text_alignment::center);
						}
																   break;
						case widgets::icon::icon_text_position::bottom: {
							text.rect(rect()
								.left(icon._padding)
								.right(it.destination.size().get_width() - icon._padding)
								.top(0.f)
								.bottom(it.destination.size().get_height() - icon._padding));
							const auto ideal_rect = widgets::measure_label(_p_directwrite_factory, text.text(), text.font(), text.font_size(),
								text_alignment::left, paragraph_alignment::top, convert_rect(text.rect()));
							text.rect().height(ideal_rect.bottom - ideal_rect.top);
							text.alignment(text_alignment::center);
							text.rect().snap_to(image.rect(), rect::snap_type::bottom, icon._gap);
						}
																	  break;
						default:
							break;
						}

						// description specs
						description.text(icon.description())
							.color_text(icon.color_text_description())
							.font_size(icon.font_size_description())
							.multiline(true)
							.on_resize(resize_params().width_rate(100.f));

						switch (icon.text_position()) {
						case widgets::icon::icon_text_position::right: {
							description.rect(rect()
								.left(icon._padding)
								.right(text.rect().width())
								.top(icon._padding)
								.bottom(it.destination.size().get_height() - text.rect().height()));
							description.rect().snap_to(text.rect(), rect::snap_type::bottom_left, 0.f);
						}
																	 break;
						case widgets::icon::icon_text_position::left: {
							description.rect(rect()
								.left(icon._padding)
								.right(text.rect().width())
								.top(icon._padding)
								.bottom(it.destination.size().get_height() - text.rect().height()));
							description.rect().snap_to(text.rect(), rect::snap_type::bottom_left, 0.f);
						}
																	break;
						case widgets::icon::icon_text_position::top: {
							description.rect(rect()
								.left(icon._padding)
								.right(text.rect().width())
								.top(0.f)
								.bottom(it.destination.size().get_height() - text.rect().height()));
							description.rect().snap_to(text.rect(), rect::snap_type::bottom, 0.f);
							description.alignment(text_alignment::center);
						}
																   break;
						case widgets::icon::icon_text_position::bottom: {
							description.rect(rect()
								.left(icon._padding)
								.right(text.rect().width())
								.top(0.f)
								.bottom(it.destination.size().get_height() - text.rect().height()));
							description.rect().snap_to(text.rect(), rect::snap_type::bottom, 0.f);
							description.alignment(text_alignment::center);
						}
																	  break;
						default:
							break;
						}

						// prevent overlap
						text.rect().clip_to(icn.rect(), icon._padding);
						description.rect().clip_to(icn.rect(), icon._padding);

						// capture image view widget
						it.destination._d_page.get_icon_impl(it.alias).set_icon_specs(icn, image, text, description);

						// copy pointer to pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						icon._p_special_pane_specs = &it.destination_specs;

						// copy pointer to image widget so we can return the image widget for those properties that pertain to the image, like the opacity
						icon._p_image_specs = &image;

						// close widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::icon, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			};

			// check if this page has icon widgets and move them if they exist
			auto page_iterator = _p_pages.find(_current_page);

			if (_p_pages.count(_current_page)) {
				auto& page = page_iterator->second;
				do_move_icons(page);
			}

			// check if status panes have icon widgets and move them if they exist
			for (auto& [alias, page] : _p_status_panes)
				do_move_icons(page);
		}

		/// The table view widget is constructed as follows:
		/// 1. A special pane is made
		/// 2. The table view is moved into the pane
		void form::impl::move_tables() {
			struct table_info {
				std::string alias;

				// important that it's not a reference because of deletion in a ranged for loop later
				lecui::widgets::table_view table;
				lecui::containers::page& source;
				lecui::containers::pane& destination;
				lecui::containers::pane_specs& destination_specs;
			};

			auto do_move_tables = [this](lecui::containers::page& page) {
				class helper {
				public:
					static void find_tables_to_move(lecui::containers::page& page,
						std::vector<table_info>& tables) {
						std::string booked;

						for (const auto& alias : page._d_page.widgets_order()) {
							auto& widget = page._d_page.widgets().at(alias);

							// check if this is a table pane
							if (alias.find(widgets::pane_impl::table_pane_alias_prefix()) != std::string::npos)
								continue;	// this is a table pane (it has a table inside. move was already done), continue to next widget

							// check if this is a table
							if (widget.type() == widgets::widget_type::table_view) {
								// book move so we don't modify the page widgets while iterating through them
								booked = alias;
								break;
							}

							if (widget.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

								// initialize tabs
								for (auto& tab : tab_pane._p_tabs)
									find_tables_to_move(tab.second, tables);	// recursion
							}
							else
								if (widget.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(alias);

									// initialize panes
									for (auto& page : pane._p_panes)
										find_tables_to_move(page.second, tables);	// recursion
								}
						}

						if (!booked.empty()) {
							// this is a table, we need to "move" it into a special pane

							// get the table widget
							auto& table_specs = page._d_page.get_table_view_impl(booked).specs();

							// make pane whose alias is prefixed by the special string
							auto& pane = containers::pane::add(page, widgets::pane_impl::table_pane_alias_prefix() + booked);

							// clone essential properties to pane (the pane will handle these and all later calls to this widget's widget will be redirected to the pane)
							pane
								.rect(table_specs.rect())
								.on_resize(table_specs.on_resize())
								.color_fill(table_specs.color_fill())
								.color_border(table_specs.color_border())
								.border(table_specs.border());

							// save move info so we can move the table into the pane later
							// we cannot do it here because we're iterating
							tables.push_back({ booked, table_specs, page, pane, pane });
						}
					}
				};

				std::vector<table_info> tables;
				helper::find_tables_to_move(page, tables);

				// move the tables
				for (auto& it : tables) {
					log("moving table: " + it.alias + " from " + it.source._d_page.alias() + " to " + it.destination._d_page.alias());

					try {
						// clone into destination
						auto& table = widgets::table_view::add(it.destination, it.alias);
						// copy widget
						table = it.table;

						// adjust widget
						table
							.rect(rect()
								.left(0.f)
								.right(it.destination.size().get_width())
								.top(0.f)
								.bottom(it.destination.size().get_height()))
							.on_resize({ 0.f, 0.f, 0.f, 0.f });	// critical because table will change size as table is browsed or changed. the pane scroll bars will do the job.

						// copy pointer to pane widget so we can return the pane widget for those properties that are handled by the pane, like the bounding rectangle
						table._p_special_pane = &it.destination;

						// close widget
						std::string error;
						it.source._d_page.close_widget(it.alias, widgets::widget_type::table_view, error);
						log("moving " + it.alias + " successful!");
					}
					catch (const std::exception& e) { log("moving " + it.alias + " failed: " + e.what()); }
				}
			};

			// check if this page has tables to move and move them if they exist
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end()) {
				auto& page = page_iterator->second;
				do_move_tables(page);
			}

			// check if status panes have tables and move them if they exist
			for (auto& [alias, page] : _p_status_panes)
				do_move_tables(page);
		}

		/// If the application receives a WM_SIZE message, this method resizes the render target
		/// appropriately
		void form::impl::on_resize(UINT width, UINT height) {
			if (_p_render_target) {
				/// Note: This method can fail, but it's okay to ignore the error here, because the
				/// error will be returned again the next time EndDraw is called
				_p_render_target->Resize(D2D1::SizeU(width, height));
			}
		}

		RECT form::impl::get_working_area(HWND hWnd) {
			HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO monitorinfo = { 0 };
			monitorinfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(hMonitor, &monitorinfo);

			unscale_RECT(monitorinfo.rcWork, _dpi_scale);
			return monitorinfo.rcWork;
		}

		float form::impl::get_dpi_scale() {
			return _dpi_scale;
		}

		void form::impl::set_position(const float& ix, const float& iy,
			const float& icx, const float& icy) {
			_design_size.width(icx);
			_design_size.height(icy);

			// ensure visibility of top left
			_point.x(largest(ix, 0.f));
			_point.y(largest(iy, 0.f));
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
				_hWnd_parent &&
				IsWindow(_hWnd_parent) &&
				(!IsWindowVisible(_hWnd_parent) || IsIconic(_hWnd_parent)))
				m_wndPos = form_position::center_to_working_area;

			switch (m_wndPos) {
			case form_position::center_to_parent: {
				if (_hWnd_parent && IsWindow(_hWnd_parent)) {
					// get coordinates of parent window
					RECT rcParent;
					GetWindowRect(_hWnd_parent, &rcParent);
					unscale_RECT(rcParent, _dpi_scale);

					const float user_width = static_cast<float>(rcParent.right - rcParent.left);
					const float user_height = static_cast<float>(rcParent.bottom - rcParent.top);

					// center to working area
					const float ix = rcParent.left + ((user_width - icx) / 2.f);
					const float iy = rcParent.top + ((user_height - icy) / 2.f);

					_point.x(ix);
					_point.y(iy);
					_design_size.width(icx);
					_design_size.height(icy);

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

				_point.x(ix);
				_point.y(iy);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::top_left: {
				_point.x(rcWork.left + 0.f);
				_point.y(rcWork.top + 0.f);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::top_left_offset: {
				_point.x(rcWork.left + offset);
				_point.y(rcWork.top + offset);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::top_right: {
				_point.x(rcWork.right - icx);
				_point.y(rcWork.top + 0.f);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::top_right_offset: {
				_point.x(rcWork.right - icx - offset);
				_point.y(rcWork.top + offset);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::bottom_right: {
				_point.x(rcWork.right - icx);
				_point.y(rcWork.bottom - icy);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::bottom_right_offset: {
				_point.x(rcWork.right - icx - offset);
				_point.y(rcWork.bottom - icy - offset);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::bottom_left: {
				_point.x(rcWork.left + 0.f);
				_point.y(rcWork.bottom - icy);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			case form_position::bottom_left_offset: {
				_point.x(rcWork.left + offset);
				_point.y(rcWork.bottom - icy - offset);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;

			default: {
				// default to top left
				_point.x(rcWork.left + 0.f);
				_point.y(rcWork.top + 0.f);
				_design_size.width(icx);
				_design_size.height(icy);
			} break;
			}

			// ensure visibility of top left
			_point.x(largest(_point.get_x(), 0.f));
			_point.y(largest(_point.get_y(), 0.f));
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
				_borderless = enabled;

				::SetWindowLongPtrW(hWnd, GWL_STYLE, static_cast<LONG>(new_style));

				if (!_allow_resizing) {
					SetWindowLong(_hWnd, GWL_STYLE,
						GetWindowLong(_hWnd,
							GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
				}

				if (!_allow_minimize) {
					SetWindowLong(_hWnd, GWL_STYLE,
						GetWindowLong(_hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
				}

				// when switching between borderless and windowed, restore appropriate shadow state
				set_shadow(hWnd, _borderless_shadow && (new_style != style::windowed));

				// redraw frame
				::SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
			}
		}

		void form::impl::set_borderless_shadow(HWND hWnd, bool enabled, bool save_value) {
			if (_borderless) {
				if (save_value)
					_borderless_shadow = enabled;

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
			++_unique_id;
			return _unique_id;
		}

		void form::impl::start_timer(const std::string& alias) {
			if (_timers.find(alias) != _timers.end()) {
				SetTimer(_hWnd, (UINT_PTR)_timers.at(alias).unique_id,
					(UINT)_timers.at(alias).milliseconds, NULL);
				_timers.at(alias).running = true;
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
					return form::impl::widget_search_results{ container._d_page._widgets.at(path), container };
				}
				catch (const std::exception&) {}

				// check if widget is in special pane
				try {
					// check special tree pane
					auto& tree_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::tree_pane_alias_prefix() + path);

					// tree pane control confirmed ... get the pane page
					auto& tree_pane = tree_pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_widget(tree_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special html pane
					auto& html_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::html_pane_alias_prefix() + path);

					// html pane control confirmed ... get the pane page
					auto& html_pane = html_pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_widget(html_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special time pane
					auto& time_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::time_pane_alias_prefix() + path);

					// time pane control confirmed ... get the pane page
					auto& time_pane = time_pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_widget(time_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special date pane
					auto& date_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::date_pane_alias_prefix() + path);

					// date pane control confirmed ... get the pane page
					auto& date_pane = date_pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_widget(date_pane, path);
				}
				catch (const std::exception&) {}

				try {
					// check special table view pane
					auto& table_view_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::table_pane_alias_prefix() + path);

					// table view pane control confirmed ... get the pane page
					auto& table_view_pane = table_view_pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_widget(table_view_pane, path);
				}
				catch (const std::exception&) {}

				// check special icon pane
				auto& icon_pane_control = container._d_page.get_pane_impl(widgets::pane_impl::icon_pane_alias_prefix() + path);

				// icon pane control confirmed ... get the pane page
				auto& icon_pane = icon_pane_control._p_panes.at("pane");

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
					auto& tab_pane = container._d_page.get_tab_pane_impl(container_alias);

					// tab pane confirmed ... get the tab
					try {
						idx = path_left.find("/");

						if (idx != std::string::npos) {
							// get the tab name
							const auto tab_name = path_left.substr(0, idx);

							// get the rest of the path
							const auto tab_path_left = path_left.substr(idx + 1);

							// get the tab
							auto& tab = tab_pane._p_tabs.at(tab_name);

							// tab confirmed ... recurse
							return find_widget(tab, tab_path_left);	// recursion
						}
					}
					catch (const std::exception&) {}
				}
				catch (const std::exception&) {}

				// check if the container is a pane
				auto& pane_control = container._d_page.get_pane_impl(container_alias);

				// pane control confirmed ... get the pane page
				auto& pane = pane_control._p_panes.at("pane");

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
					auto& pane_control = container._d_page.get_pane_impl(path);

					// pane control confirmed ... get the pane page
					return pane_control._p_panes.at("pane");
				}
				else {
					// get the container's alias
					const auto container_alias = path.substr(0, idx);

					// get the rest of the path
					const auto path_left = path.substr(idx + 1);

					// check if the container is a tab pane
					try {
						auto& tab_pane = container._d_page.get_tab_pane_impl(container_alias);

						// tab pane confirmed ... get the tab
						try {
							idx = path_left.find("/");

							if (idx == std::string::npos) {
								// check if this is a tab
								const auto tab_name = path_left;
								return tab_pane._p_tabs.at(tab_name);
							}
							else {
								// get the tab name
								const auto tab_name = path_left.substr(0, idx);

								// get the rest of the path
								const auto tab_path_left = path_left.substr(idx + 1);

								// get the tab
								auto& tab = tab_pane._p_tabs.at(tab_name);

								// tab confirmed ... recurse
								return find_page(tab, tab_path_left);	// recursion
							}
						}
						catch (const std::exception&) {}
					}
					catch (const std::exception&) {}

					// check if the container is a pane
					auto& pane_control = container._d_page.get_pane_impl(container_alias);

					// pane control confirmed ... get the pane page
					auto& pane = pane_control._p_panes.at("pane");

					// pane confirmed ... recurse
					return find_page(pane, path_left);
				}
			}
		}

		bool form::impl::enable(const std::string& path, bool enable, std::string& error) {
			bool success = false;

			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(_p_pages.at(page_alias), path_remaining);
						result.widget.enable(enable);
						update();
						success = true;
					}
					catch (const std::exception& e) { error = e.what(); }

					if (!success) {
						try {
							// check status panes
							auto result = find_widget(_p_status_panes.at(page_alias), path_remaining);
							result.widget.enable(enable);
							update();
							success = true;
						}
						catch (const std::exception& e) { error = e.what(); }
					}
				}
			}
			catch (const std::exception& e) { error = e.what(); }

			bool _allow_resizing_before = _allow_resizing;

			try { _allow_resizing = _widgets.at("maximize_button").enabled(); }
			catch (const std::exception&) { _allow_resizing = false; }

			if (_allow_resizing_before != _allow_resizing) {
				if (_allow_resizing)
					SetWindowLong(_hWnd, GWL_STYLE,
						GetWindowLong(_hWnd, GWL_STYLE) | (WS_SIZEBOX | WS_MAXIMIZEBOX));
				else
					SetWindowLong(_hWnd, GWL_STYLE,
						GetWindowLong(_hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
			}

			try { _allow_minimize = _widgets.at("minimize_button").enabled(); }
			catch (const std::exception&) { _allow_minimize = false; }

			if (_allow_minimize)
				SetWindowLong(_hWnd, GWL_STYLE,
					GetWindowLong(_hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
			else
				SetWindowLong(_hWnd, GWL_STYLE,
					GetWindowLong(_hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);

			if (success)
				error.clear();

			return success;
		}

		bool form::impl::show(const std::string& path, bool show, std::string& error) {
			bool success = false;

			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(_p_pages.at(page_alias), path_remaining);
						result.widget.show(show);
						update();
						success = true;
					}
					catch (const std::exception& e) { error = e.what(); }

					if (!success) {
						try {
							// check status pages
							auto result = find_widget(_p_status_panes.at(page_alias), path_remaining);
							result.widget.show(show);
							update();
							success = true;
						}
						catch (const std::exception& e) { error = e.what(); }
					}
				}
			}
			catch (const std::exception& e) { error = e.what(); }

			if (success)
				error.clear();

			return success;
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
					result.page._d_page.close_widget(result.widget.alias(), result.widget.type(), error);
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
						auto result = find_widget(_p_pages.at(page_alias), path_remaining);
						do_close(result);
						update();
					}
					catch (const std::exception&) {
						try {
							// check status pages
							auto result = find_widget(_p_status_panes.at(page_alias), path_remaining);
							do_close(result);
							update();
						}
						catch (const std::exception&) {
							// check tab controls
							const auto tab_name = find_page(_p_pages.at(page_alias), path_remaining)._d_page.alias();

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
									auto& page = find_page(_p_pages.at(page_alias), tab_control_container_path_remaining);

									// get the tab control implementation
									auto& tab_pane_impl = page._d_page.get_tab_pane_impl(alias);

									// close tab
									tab_pane_impl.close_tab(tab_name);
									update();
								}
								else {
									// this tab control is in a top level container
									const auto alias = tab_control_container_path_remaining;
									
									// get tab control's container
									auto& page = _p_pages.at(page_alias);

									// get the tab control implementation
									auto& tab_pane_impl = page._d_page.get_tab_pane_impl(alias);

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
			for (auto& widget : container._d_page.widgets()) {
				widget.second.select(false);

				if (widget.second.type() ==
					widgets::widget_type::tab_pane) {
					// get this tab pane
					auto& tab_pane = container._d_page.get_tab_pane_impl(widget.first);

					auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

					if (page_iterator != tab_pane._p_tabs.end())
						clear_selection(page_iterator->second);	// recursion
				}
				else
					if (widget.second.type() ==
						widgets::widget_type::pane) {
						// get this pane
						auto& pane = container._d_page.get_pane_impl(widget.first);

						auto page_iterator = pane._p_panes.find(pane._current_pane);

						if (page_iterator != pane._p_panes.end())
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
					clear_selection(_p_pages.at(page_alias));

					try {
						// check form pages
						auto result = find_widget(_p_pages.at(page_alias), path_remaining);
						result.widget.select(true);
						update();
					}
					catch (const std::exception&) {}

					try {
						// check status pages
						auto result = find_widget(_p_status_panes.at(page_alias), path_remaining);
						result.widget.select(true);
						update();
					}
					catch (const std::exception&) {}
				}
			}
			catch (const std::exception&) {}
		}

		bool form::impl::refresh(const std::string& path, std::string& error) {
			bool success = false;

			try {
				// get the page alias
				const auto idx = path.find("/");

				if (idx != std::string::npos) {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);

					try {
						// check form pages
						auto result = find_widget(_p_pages.at(page_alias), path_remaining);
						result.widget.discard_resources();
						update();
						success = true;
					}
					catch (const std::exception& e) { error = e.what(); }

					if (!success) {
						try {
							// check status panes
							auto result = find_widget(_p_status_panes.at(page_alias), path_remaining);
							result.widget.discard_resources();
							update();
							success = true;
						}
						catch (const std::exception& e) { error = e.what(); }
					}
				}
			}
			catch (const std::exception& e) { error = e.what(); }

			if (success)
				error.clear();

			return success;
		}

		lecui::size form::impl::get_status_size(containers::status_pane_specs::pane_location type) {
			std::string alias;
			switch (type) {
			case containers::status_pane_specs::pane_location::top: alias = "status::top"; break;
			case containers::status_pane_specs::pane_location::left: alias = "status::left"; break;
			case containers::status_pane_specs::pane_location::right: alias = "status::right"; break;
			case containers::status_pane_specs::pane_location::bottom:
			default: alias = "status::bottom"; break;
			}

			lecui::size size;
			auto it = _p_status_panes.find(alias);
			if (it != _p_status_panes.end()) {
				try {
					if (!_p_status_pane_specs.at(it->first).floating())
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
			if (!_guid.empty()) {
				auto hWnd = find_native_handle(_guid);

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

		void form::impl::close_tooltips() {
			class helper {
			public:
				static void check_widgets(containers::page& page) {
					// check widgets
					for (auto& widget : page._d_page.widgets()) {
						bool is_scroll_bar = (widget.second.type() ==
							widgets::widget_type::h_scrollbar) ||
							(widget.second.type() ==
								widgets::widget_type::v_scrollbar);

						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						widget.second.hide_tooltip();
					}

					for (auto& widget : page._d_page.widgets()) {
						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							if (page_iterator != tab_pane._p_tabs.end())
								helper::check_widgets(page_iterator->second);	// recursion
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(widget.first);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								if (page_iterator != pane._p_panes.end())
									helper::check_widgets(page_iterator->second);	// recursion
							}
					}
				}
			};

			// form widgets
			for (auto& widget : _widgets) {
				bool is_scroll_bar = (widget.second.type() ==
					widgets::widget_type::h_scrollbar) ||
					(widget.second.type() ==
						widgets::widget_type::v_scrollbar);

				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				widget.second.hide_tooltip();
			}

			// pages
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second);

			// status panes
			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second);
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

			auto& _form = form_optional.value().get();

			switch (msg) {
			case WM_CREATE:
				_form._d._hWnd = hWnd;

				if (_form._d._p_maximize_button)
					_form._d._p_maximize_button->set_hwnd(hWnd);

				if (_form._d._p_minimize_button)
					_form._d._p_minimize_button->set_hwnd(hWnd);

				if (!_form._d._tooltip_form) {
					_form._d.set_borderless(hWnd, _form._d._borderless);
					_form._d.set_borderless_shadow(hWnd, _form._d._borderless_shadow);
				}

				if (!_form._d._allow_resizing)
					SetWindowLong(hWnd, GWL_STYLE,
						GetWindowLong(hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

				if (!_form._d._allow_minimize) {
					SetWindowLong(hWnd, GWL_STYLE,
						GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
				}

				DragAcceptFiles(hWnd, _form.events().drop_files == nullptr ? FALSE : TRUE);

				// call the start handler
				if (_form.events().start)
					_form.events().start();

				for (auto& it : _form._d._timers)
					if (!it.second.running)
						_form._d.start_timer(it.first);

				return TRUE;

			case WM_LBUTTONDOWN:
				_form._d._lbutton_pressed = true;
				// capture the mouse
				SetCapture(hWnd);
				_form._d.on_lbuttondown(_form._d.get_cursor_position());
				return NULL;

			case WM_LBUTTONUP:
				// release the mouse capture
				ReleaseCapture();
				_form._d.on_lbuttonup(_form._d.get_cursor_position());
				_form._d._lbutton_pressed = false;
				return NULL;

			case WM_RBUTTONDOWN:
				_form._d.on_rbuttondown(_form._d.get_cursor_position());
				return NULL;

			case WM_RBUTTONUP:
				_form._d.on_rbuttonup(_form._d.get_cursor_position());
				return NULL;

			case WM_SIZING:
				_form._d._user_sizing = true;
				break;

			case WM_EXITSIZEMOVE:
				_form._d._user_sizing = false;
				break;

			case WM_WINDOWPOSCHANGING:
				_form._d.on_form_pos_changing(lParam);
				break;

			case WM_SIZE:
				_form._d.on_resize(LOWORD(lParam), HIWORD(lParam));
				if (_form._d._form_events.size) {
					dimensions dim{ _form };
					_form._d._form_events.size(dim.get_size());
				}
				return NULL;

			case WM_MOUSEMOVE:
				_form._d._mouse_track.on_mouse_move(hWnd);
				_form._d.client_hittest(_form._d.get_cursor_position());
				return NULL;

			case WM_MOUSELEAVE:
				// _form._d.get_cursor_position() doesn't work here
				_form._d.client_hittest({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
				_form._d._mouse_track.reset(hWnd);
				return NULL;

			case WM_MOUSEHOVER:
				_form._d._mouse_track.reset(hWnd);
				return NULL;

			case WM_SETCURSOR:
				if (LOWORD(lParam) == HTCLIENT) {
					HCURSOR h_cursor = _form._d._h_widget_cursor;
					if (h_cursor) {
						SetCursor(h_cursor);
						return TRUE;
					}
				}
				break;

			case WM_DISPLAYCHANGE:
				_form._d.update();
				return NULL;

			case WM_SETFOCUS:
				return NULL;

			case WM_KILLFOCUS:
				// check if it's the parent that now has focus
				if (_form._d._menu_form || _form._d._tooltip_form) {
					if (IsWindow(_form._d._hWnd_parent)) {
						if (GetForegroundWindow() != _form._d._hWnd_parent) {
							// focus lost, but not to parent
							_form.close();
						}
						else {
							// let parent decide whether to close child in WM_LBUTTONDOWN
						}
					}
					else {
						// focus lost, and there is no parent
						_form.close();
					}
				}
				return NULL;

			case WM_PAINT:
				_form._d.move_trees();
				_form._d.move_html_editors();
				_form._d.move_times();
				_form._d.move_dates();
				_form._d.move_icons();
				_form._d.move_tables();
				_form._d.on_render();

				if (_form._d._schedule_refresh)
					_form._d._schedule_refresh = false;
				else
					ValidateRect(hWnd, nullptr);

				return NULL;

			case WM_CLOSE:
				try {
					if (!_form._d._widgets.at("close_button").enabled())
						return NULL;
				}
				catch (const std::exception&) {}
				if (_form.events().close)
					// call the close handler
					_form.events().close();
				else
					_form.close();
				return NULL;

			case WM_DESTROY:
				// call the shutdown handler
				if (_form.events().shutdown)
					_form.events().shutdown();
				return NULL;

			case WM_NCCALCSIZE:
				if (wParam == TRUE && _form._d._borderless) {
					auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
					adjust_maximized_client_rect(hWnd, params.rgrc[0]);

					if (maximized(hWnd)) {
						_form._d._shadow_setting_before_maximize =
							_form._d._borderless_shadow;
						_form._d.set_borderless_shadow(hWnd, false, false);
					}
					else
						_form._d.set_borderless_shadow(hWnd,
							_form._d._shadow_setting_before_maximize, false);

					return NULL;
				}
				break;

			case WM_NCHITTEST:
				if (_form._d._borderless)
					return _form._d.non_client_hittest(
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
				for (auto& [key, child] : _form._d._m_children) {
					if (child && IsWindow(child->_d._hWnd) && (child->_d._menu_form || child->_d._tooltip_form)) {
						// close child menu forms and child tooltip forms
						child->close();
					}
				}
				break;

			case WM_GETMINMAXINFO: {
				// set lower limits to window size
				MINMAXINFO* p_minmaxinfo = (MINMAXINFO*)lParam;
				p_minmaxinfo->ptMinTrackSize.x = static_cast<LONG>(.5f +
					_form._d._min_size.get_width() * _form._d._dpi_scale);
				p_minmaxinfo->ptMinTrackSize.y = static_cast<LONG>(.5f +
					_form._d._min_size.get_height() * _form._d._dpi_scale);
			} break;

			case WM_TIMER: {
				int unique_id = (int)wParam;

				for (auto& it : _form._d._timers)
					if (it.second.unique_id == unique_id) {
						if (it.second.on_timer)
							it.second.on_timer();

						break;
					}
			} break;

			case WM_CHAR:
				_form._d.on_char(wParam);
				break;

			case WM_GETDLGCODE:
				return DLGC_WANTALLKEYS;	// for VK_UP, VK_DOWN to be received in WM_KEYDOWN

			case WM_KEYDOWN:
				_form._d.on_keydown(wParam);
				break;

			case WM_KEYUP:
				_form._d.on_keyup(wParam);
				break;

			case WM_MOUSEWHEEL:
				_form._d.on_wheel(wParam);
				break;

			case WM_MOUSEHWHEEL:
				_form._d.on_hwheel(wParam);
				break;

			case WM_COPYDATA: {
				/// 1. Busy
				/// 2. No handler
				/// 3. Handled
				LRESULT result = 0;
				if (!_form._d._receiving) {
					COPYDATASTRUCT* p_copy_data = (COPYDATASTRUCT*)lParam;

					if (p_copy_data && _form.events().receive_data) {
						_form._d._data_received = std::string((LPSTR)p_copy_data->lpData, p_copy_data->cbData);

						// forward data to the receive data handler through a timer set to 0
						if (!_form._d._data_received.empty()) {
							_form._d._receiving = true;
							timer_manager(_form).add(_form._d._receive_data_timer_alias, 0, [&]() {
								timer_manager(_form).stop(_form._d._receive_data_timer_alias);
								_form.events().receive_data(_form._d._data_received);
								_form._d._receiving = false;
								});
						}

						result = instance_messages::handled;
					}
					else {
						if (!_form.events().receive_data)
							result = instance_messages::no_handler;
					}
				}
				else
					result = instance_messages::busy;

				if (result)
					return result;
			} break;

			case WM_DROPFILES:
				_form._d.on_dropfiles(wParam);
				break;

			case WM_APP:
				switch (lParam) {
				case WM_RBUTTONUP:
					// tray icon right clicked
					if (!_form._d._tray_icon_menu_items.empty()) {
						context_menu::specs menu_specs;
						menu_specs.quality = image_quality::high;

						for (const auto& item : _form._d._tray_icon_menu_items) {
							menu_item mi;
							mi.label = item.label;
							mi.font = item.font;
							mi.font_size = item.font_size;
							menu_specs.items.push_back(mi);
						}

						menu_specs.type = context_menu::pin_type::bottom;

						auto result = context_menu()(_form, menu_specs);

						for (auto& m_it : _form._d._tray_icon_menu_items) {
							if (!m_it.label.empty() && m_it.label == result) {
								if (m_it.action != nullptr)
									m_it.action();
							}
						}
					}
					break;

				case WM_LBUTTONUP:
					// tray icon left clicked, invoke the action of the default item
					for (auto& m_it : _form._d._tray_icon_menu_items) {

						std::string plain_label;
						// parse the label
						// the default color doesn't matter here we're just getting the plain text
						std::vector<formatted_text_parser::text_range_properties> formatting;
						widgets::parse_formatted_text(m_it.label, plain_label,
							D2D1::ColorF(D2D1::ColorF::Black), formatting);

						if (!plain_label.empty() && plain_label == _form._d._tray_item_default) {
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
				if (_form._d._reg_id && msg == _form._d._reg_id)
					return _form._d._reg_id;	// another instance is checking if it should proceed
				break;
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
}
