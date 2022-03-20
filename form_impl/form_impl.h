//
// form_impl.h - form::impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../form.h"
#include "../appearance.h"
#include "../limit_single_instance/limit_single_instance.h"
#include "../containers/page/page_impl.h"
#include "../containers/status_pane.h"
#include "../menus/form_menu.h"
#include "../utilities/tray_icon.h"

#include "../widgets/widget_impl.h"
#include "../widgets/control_buttons/close_button/close_button_impl.h"
#include "../widgets/control_buttons/maximize_button/maximize_button_impl.h"
#include "../widgets/control_buttons/minimize_button/minimize_button_impl.h"

#include "../widgets/tooltip.h"

// Windows headers
#include <Windows.h>

// Direct2D, DirectWrite and WIC (Windows Imaging Component) headers
#include <d2d1.h>
#include <dwrite.h>

// C++ STL headers
#include <atomic>
#include <memory>

namespace liblec {
	namespace lecui {
		class mouse_track {
			bool _mouse_tracking;

		public:
			mouse_track() : _mouse_tracking(false) {}

			void on_mouse_move(HWND hWnd) {
				if (!_mouse_tracking) {
					// Enable mouse tracking
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(tme);
					tme.hwndTrack = hWnd;
					tme.dwFlags = TME_HOVER | TME_LEAVE;
					tme.dwHoverTime = HOVER_DEFAULT;
					TrackMouseEvent(&tme);
					_mouse_tracking = true;
				}
			}

			void reset(HWND hWnd) { _mouse_tracking = false; }
		};

		class form::impl {
			// static members
			static std::atomic<unsigned long> _instances;
			static std::atomic<bool> _initialized;
			static ID2D1Factory* _p_direct2d_factory;
			static IDWriteFactory* _p_directwrite_factory;
			static IWICImagingFactory* _p_iwic_factory;
			static limit_single_instance* _p_instance;

			enum instance_messages {
				busy = 1,
				no_handler,
				handled,
			};

			form& _fm;
			form* _p_parent;
			bool _menu_form;	// for use as a menu; not resizable & has neither a caption nor control buttons
			bool _tooltip_form;	// like a menu form but specifically designed for tooltips
			std::map<form*, form*> _m_children;
			bool _create_called;
			std::string _guid;
			UINT _reg_id;
			bool _receiving;	// for preventing multiple concurrent receiving operations
			std::string _data_received;

			// constant members
			const float _caption_bar_height;
			const float _caption_and_menu_gap;
			const float _form_menu_margin;
			const float _form_border_thickness;
			const float _content_margin;
			const float _control_button_margin;
			const float _caption_icon_minimum_margin;
			const float _caption_icon_maximum_size;
			const std::string _receive_data_timer_alias;
			const DWORD _current_thread_id;

			// name of dll containing resources like PNGs etc
			std::string _resource_dll_filename;
			HMODULE _resource_module_handle;

			// icons for use by the Windows OS
			int _idi_icon, _idi_icon_small;

			// the theme
			themes _theme;

			// colors
			color _clr_background, _clr_titlebar_background, _clr_theme, _clr_theme_hot,
				_clr_theme_disabled, _clr_theme_text;

			bool _top_most;
			HWND _hWnd, _hWnd_parent;

			// form caption
			std::string _caption_formatted, _caption_plain;

			bool _activate;

			// window coordinates
			point _point;
			size _design_size, _min_size;
			bool _allow_resizing, _allow_minimize;

			bool _user_pos;
			bool _preset_pos;
			form_position _form_position;

			float _dpi_scale;

			bool _borderless;			// should the window be borderless
			bool _borderless_shadow;	// should the window display a native aero shadow while borderless
			bool _previous_shadow_setting;
			bool _shadow_setting_before_maximize;

			// Direct2D resources
			ID2D1HwndRenderTarget* _p_render_target;
			ID2D1SolidColorBrush* _p_brush_theme;
			ID2D1SolidColorBrush* _p_brush_theme_hot;
			ID2D1SolidColorBrush* _p_brush_theme_disabled;
			ID2D1SolidColorBrush* _p_brush_titlebar;

			// pages <K = page alias, T>
			std::map<std::string, containers::status_pane> _p_status_panes;
			std::map<std::string, containers::status_pane_specs> _p_status_pane_specs;
			std::map<std::string, containers::page> _p_pages;
			std::string _current_page;
			std::vector<std::string> _page_list;	// for managing pages

			mouse_track _mouse_track;

			// form widgets <K = widget alias, T>
			std::map<std::string, widgets::widget_impl&> _widgets;
			std::vector<std::string> _widgets_order;
			containers::page _controls_page;
			std::unique_ptr<widgets::close_button_impl> _p_close_button;
			std::unique_ptr<widgets::maximize_button_impl> _p_maximize_button;
			std::unique_ptr<widgets::minimize_button_impl> _p_minimize_button;
			std::unique_ptr<widgets::image_view_impl> _p_caption_icon;
			std::unique_ptr<widgets::label_impl> _p_caption;
			std::vector<std::unique_ptr<widgets::label_impl>> _p_menu;

			D2D1_POINT_2F _point_before;
			bool _user_sizing;

			struct timer {
				int unique_id = -1;
				bool running = false;
				unsigned long milliseconds = 1000;
				std::function<void()> on_timer = nullptr;
			};

			// timer map <K = alias, T>
			std::map<std::string, timer> _timers;
			std::atomic<int> _unique_id;

			bool _reverse_tab_navigation;
			bool _shift_pressed;
			bool _ctrl_pressed;
			bool _space_pressed;
			bool _lbutton_pressed;

			std::vector<std::string> _scheduled_for_closure;

			HCURSOR _h_widget_cursor;

			// form menu
			struct form_menu_label {
				std::string text;
				std::string tooltip;
				std::vector<form_menu_item> items;
				rect rc_text;
			};

			std::vector<form_menu_label> _form_menu;

			/// <summary>Flag for scheduling a refresh. Important when the widget of a widget are
			/// changed during the rendering of another widget.</summary>
			/// <remarks>When the widget of a widget are changed the widget's render method has
			/// to be called at least once for the changes to take effect. For the render method to
			/// be called the form's on_render method has to be called. Therefore, if a widget's
			/// widget are changed during the render of another widget the changes can only be
			/// effected in the next form render unless the widget whose widget are changed is
			/// later in the render queue (which is not always the case, obviously). To ensure
			/// consistency, it is important to set this flag to true when changing the widget of a
			/// widget from within the render method of another widget.</remarks>
			bool _schedule_refresh;

			bool _close_called;

			bool _force_instance;

			bool _tray_icon_present;
			std::vector<tray_menu_item> _tray_icon_menu_items;
			std::string _tray_item_default;

			bool _start_hidden;

			std::unique_ptr<widgets::tooltip_form> _p_tooltip_form = nullptr;

			bool _side_pane_present;
			float _side_pane_thickness;
			int _png_caption_icon;

			form::form_events _form_events;

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

			friend class form;
			friend class dimensions;
			friend class appearance;
			friend class controls;
			friend class page_manager;
			friend class widget_manager;
			friend class timer_manager;
			friend class filesystem;
			friend class color_picker;
			friend class splash;
			friend class instance_manager;
			friend class form_menu;
			friend class tray_icon;
			friend class containers::status_pane_builder;
			friend class containers::tab_pane_builder;
			friend class containers::tab_builder;
			friend class containers::pane_builder;
			friend class containers::group_builder;
			friend class widgets::label_builder;
			friend class widgets::image_view_builder;
			friend class widgets::button_builder;
			friend class widgets::custom_builder;
			friend class widgets::table_view_builder;
			friend class widgets::rectangle_builder;
			friend class widgets::toggle_builder;
			friend class widgets::progress_indicator_builder;
			friend class widgets::progress_bar_builder;
			friend class widgets::checkbox_builder;
			friend class widgets::text_field_builder;
			friend class widgets::tree_view_builder;
			friend class widgets::slider_builder;
			friend class widgets::html_editor_builder;
			friend class widgets::combobox_builder;
			friend class widgets::line_builder;
			friend class widgets::time_builder;
			friend class widgets::date_builder;
			friend class widgets::icon_builder;
			friend class widgets::strength_bar_builder;

			friend class widgets::widget_impl;
			friend class widgets::combobox_impl;
			friend class widgets::image_view_impl;
			friend class widgets::tree_view_impl;
			friend class widgets::time_impl;
			friend class widgets::date_impl;
			friend class widgets::html_editor_impl;
			friend class widgets::icon_impl;

		public:
			impl(form& fm, const std::string& caption_formatted);
			~impl();

			HRESULT create_device_resources();
			void discard_device_resources(bool in_destructor = false);
			void create_close_button(std::function<void()> on_click);
			void create_maximize_button();
			void create_minimize_button();
			void create_form_caption();
			void create_form_menu();
			void update();
			void move_trees();
			void move_html_editors();
			void move_times();
			void move_dates();
			void move_icons();
			void move_tables();
			HRESULT on_render();
			void on_resize(UINT width, UINT height);
			RECT get_working_area(HWND hWnd);
			float get_dpi_scale();
			
			void set_position(const float& ix, const float& iy,
				const float& icx, const float& icy);
			void set_position(const form_position& wndPos,
				const float& icx_in, const float& icy_in);

			static bool composition_enabled();
			static style select_borderless_style();
			static void set_shadow(HWND hWnd, bool enabled);
			void set_borderless(HWND hWnd, bool enabled);
			void set_borderless_shadow(HWND hWnd, bool enabled, bool save_value = true);
			static bool maximized(HWND hwnd);
			static void adjust_maximized_client_rect(HWND window, RECT& rect);

			/// cursor
			D2D1_POINT_2F get_cursor_position();
			D2D1_SIZE_F get_cursor_size();

			/// mouse
			void on_lbuttondown(const D2D1_POINT_2F& point);
			void on_lbuttonup(const D2D1_POINT_2F& point);
			void on_rbuttondown(const D2D1_POINT_2F& point);
			void on_rbuttonup(const D2D1_POINT_2F& point);
			void client_hittest(const D2D1_POINT_2F& point);
			LRESULT non_client_hittest(const POINT& cursor);
			void on_wheel(WPARAM wParam);
			void on_hwheel(WPARAM wParam);
			void on_dropfiles(WPARAM wParam);

			/// keyboard
			void on_keydown(WPARAM wParam);
			void on_keyup(WPARAM wParam);
			void on_char(WPARAM wParam);

			/// size
			void on_form_pos_changing(LPARAM lParam);

			int make_unique_id();
			void start_timer(const std::string& alias);

			struct widget_search_results {
				widgets::widget_impl& widget;
				containers::page& page;
			};

			widget_search_results find_widget(containers::page& container, const std::string& path);
			containers::page& find_page(containers::page& container, const std::string& path);

			bool enable(const std::string& path, bool enable, std::string& error);
			bool show(const std::string& path, bool show, std::string& error);
			void close(const std::string& path);
			void close_container(const std::string& path);
			void clear_selection(containers::page& container);
			void select(const std::string& path);
			bool refresh(const std::string& path, std::string& error);

			lecui::size get_status_size(containers::status_pane_specs::pane_location type);
			HWND find_native_handle(const std::string& guid);
			void open_existing_instance();

			void close_tooltips();

			static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};
	}
}
