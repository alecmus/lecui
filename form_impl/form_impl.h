//
// form_impl.h - form::impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
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

		class form::impl {
			// static members
			static std::atomic<unsigned long> instances_;
			static std::atomic<bool> initialized_;
			static ID2D1Factory* p_direct2d_factory_;
			static IDWriteFactory* p_directwrite_factory_;
			static IWICImagingFactory* p_iwic_factory_;
			static limit_single_instance* p_instance_;

			enum instance_messages {
				busy = 1,
				no_handler,
				handled,
			};

			form& fm_;
			form* p_parent_;
			bool menu_form_;	// for use as a menu; not resizable & has neither a caption nor control buttons
			bool tooltip_form_;	// like a menu form but specifically designed for tooltips
			bool parent_closing_;
			std::map<form*, form*> m_children_;
			bool show_called_;
			std::string guid_;
			UINT reg_id_;
			bool receiving_;	// for preventing multiple concurrent receiving operations
			std::string data_received_;

			// constant members
			const float caption_bar_height_;
			const float caption_and_menu_gap_;
			const float form_menu_margin_;
			const float form_border_thickness_;
			const float page_tolerance_;
			const float control_button_margin_;
			const std::string receive_data_timer_alias_;

			// name of dll containing resources like PNGs etc
			std::string resource_dll_filename_;
			HMODULE resource_module_handle_;

			// icons for use by the Windows OS
			int idi_icon_, idi_icon_small_;

			// the theme
			themes theme_;

			// colors
			color clr_background_, clr_titlebar_background_, clr_theme_, clr_theme_hot_,
				clr_theme_disabled_, clr_theme_text_;

			bool top_most_;
			HWND hWnd_, hWnd_parent_;

			// form caption
			std::string caption_formatted_, caption_plain_;

			bool activate_;

			// window coordinates
			point point_;
			size size_, min_size_;
			bool allow_resizing_, allow_minimize_;

			bool user_pos_;
			bool preset_pos_;
			form_position form_position_;

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

			// pages <K = page alias, T>
			std::map<std::string, containers::page> p_status_panes_;
			std::map<std::string, containers::status_pane_specs> p_status_pane_specs_;
			std::map<std::string, containers::page> p_pages_;
			std::string current_page_;

			mouse_track mouse_track_;

			// form widgets <K = widget alias, T>
			std::map<std::string, widgets::widget_impl&> widgets_;
			std::vector<std::string> widgets_order_;
			containers::page controls_page_;
			std::unique_ptr<widgets::close_button_impl> p_close_button_;
			std::unique_ptr<widgets::maximize_button_impl> p_maximize_button_;
			std::unique_ptr<widgets::minimize_button_impl> p_minimize_button_;
			std::unique_ptr<widgets::label_impl> p_caption_;
			std::vector<std::unique_ptr<widgets::label_impl>> p_menu_;

			D2D1_POINT_2F point_before_;
			bool user_sizing_;

			struct timer {
				int unique_id = -1;
				bool running = false;
				unsigned long milliseconds = 1000;
				std::function<void()> on_timer = nullptr;
			};

			// timer map <K = alias, T>
			std::map<std::string, timer> timers_;
			std::atomic<int> unique_id_;

			bool reverse_tab_navigation_;
			bool shift_pressed_;
			bool space_pressed_;
			bool lbutton_pressed_;

			std::function<void()> on_caption_;
			std::string caption_tooltip_;
			std::function<void(const std::string& file)> on_drop_files_;
			std::function<void(const std::string& data)> on_receive_data_;

			std::vector<std::string> scheduled_for_closure_;

			HCURSOR h_widget_cursor_;

			// form menu
			struct form_menu_label {
				std::string text;
				std::string tooltip;
				std::vector<form_menu_item> items;
				rect rc_text;
			};

			std::vector<form_menu_label> form_menu_;

			/// <summary>Flag for scheduling a refresh. Important when the specs of a widget are
			/// changed during the rendering of another widget.</summary>
			/// <remarks>When the specs of a widget are changed the widget's render method has
			/// to be called at least once for the changes to take effect. For the render method to
			/// be called the form's on_render method has to be called. Therefore, if a widget's
			/// specs are changed during the render of another widget the changes can only be
			/// effected in the next form render unless the widget whose specs are changed is
			/// later in the render queue (which is not always the case, obviously). To ensure
			/// consistency, it is important to set this flag to true when changing the specs of a
			/// widget from within the render method of another widget.</remarks>
			bool schedule_refresh_;

			bool close_called_;

			bool force_instance_;

			bool tray_icon_present_;
			std::vector<tray_menu_item> tray_icon_menu_items_;
			std::string tray_item_default_;

			bool start_hidden_;

			std::unique_ptr<widgets::tooltip_form> p_tooltip_form_ = nullptr;

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
			friend class instance_management;
			friend class form_menu;
			friend class tray_icon;
			friend class containers::status_pane;
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

			void enable(const std::string& path, bool enable);
			void show(const std::string& path, bool show);
			void close(const std::string& path);
			void close_container(const std::string& path);
			void clear_selection(containers::page& container);
			void select(const std::string& path);

			lecui::size get_status_size(containers::status_pane_specs::location type);
			HWND find_native_handle(const std::string& guid);
			void open_existing_instance();

			static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		};
	}
}
