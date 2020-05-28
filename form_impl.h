/*
** form_impl.h - form::impl interface
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

#pragma once

#include "form.h"
#include "appearance.h"
#include "containers/page/page_impl.h"
#include "containers/status_pane.h"

#include "widgets/widget_impl.h"
#include "widgets/control_buttons/close_button/close_button.h"
#include "widgets/control_buttons/maximize_button/maximize_button.h"
#include "widgets/control_buttons/minimize_button/minimize_button.h"

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

			form* p_parent_;
			bool menu_form_;	// for use as a menu; not resizable & has neither a caption nor control buttons
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
			std::map<std::string, containers::status_pane::status_pane_specs> p_status_pane_specs_;
			std::map<std::string, containers::page> p_pages_;
			std::string current_page_;

			mouse_track mouse_track_;

			// form widgets <K = widget alias, T>
			std::map<std::string, widgets_impl::widget&> widgets_;
			std::vector<std::string> widgets_order_;
			std::unique_ptr<widgets_impl::close_button> p_close_button_;
			std::unique_ptr<widgets_impl::maximize_button> p_maximize_button_;
			std::unique_ptr<widgets_impl::minimize_button> p_minimize_button_;
			std::unique_ptr<widgets_impl::label> p_caption_;

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
			std::function<void(const std::string& file)> on_drop_files_;

			std::vector<std::string> scheduled_for_closure_;

			HCURSOR h_widget_cursor_;

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
			friend class page_management;
			friend class widget_management;
			friend class timer_management;
			friend class containers::status_pane;
			friend class containers::tab_pane;
			friend class containers::tab;
			friend class containers::pane;
			friend class containers::group;
			friend class widgets::label;
			friend class widgets::image;
			friend class widgets::button;
			friend class widgets::combo;
			friend class widgets::custom;
			friend class widgets::table;
			friend class widgets::rectangle;
			friend class widgets::toggle;
			friend class widgets::progress_indicator;
			friend class widgets::progress_bar;
			friend class widgets::checkbox;
			friend class widgets::textbox;
			friend class widgets::tree;
			friend class widgets::slider;

		public:
			impl(const std::string& caption_formatted);
			~impl();

			HRESULT create_device_resources();
			void discard_device_resources(bool in_destructor = false);
			void create_close_button(std::function<void()> on_click);
			void create_maximize_button();
			void create_minimize_button();
			void create_form_caption();
			void update();
			void move_trees();
			HRESULT on_render();
			void on_resize(UINT width, UINT height);
			RECT get_working_area(HWND hWnd);
			D2D1_POINT_2F get_cursor_position();
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

			LRESULT hit_test(const POINT& cursor);
			void client_hittest(const D2D1_POINT_2F& point);
			void on_lbuttondown(const D2D1_POINT_2F& point);
			void on_lbuttonup(const D2D1_POINT_2F& point);
			bool destroy_menus();
			int make_unique_id();
			void start_timer(const std::string& alias);

			struct widget_search_results {
				widgets_impl::widget& widget;
				containers::page& page;
			};

			widget_search_results find_widget(containers::page& container, const std::string& path);
			containers::page& find_page(containers::page& container, const std::string& path);

			void enable(const std::string& path, bool enable);
			void show(const std::string& path, bool show);
			void close(const std::string& path);

			static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

			lecui::size get_status_size(containers::status_pane::location type);
		};
	}
}
