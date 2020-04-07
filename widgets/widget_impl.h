/*
** widget_impl.h - widget_impl interface
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

#include "../form_common.h"
#include "widget.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			namespace specs {
				class scrollbar : public widget {
				public:
					scrollbar () {
						color_fill = { 241, 241, 241, 255 };
						color_hot = { 231, 231, 231, 255 };
					}
					liblec::lecui::color color_scrollbar_border = { 180, 180, 180, 255 };
					liblec::lecui::color color_hot_pressed = { 221, 221, 221, 255 };
					liblec::lecui::color color_background = { 0, 0, 0, 0 };
				};
			}
		}

		namespace widgets_implementation {
			enum class widget_type {
				close_button,
				maximize_button,
				minimize_button,
				h_scrollbar,
				v_scrollbar,

				tab_control,
				pane,

				rectangle,
				label,
				group,
				button,
				toggle,
				combo,
				list,
				custom,
				image,
				progress_indicator,
				progress_bar,
				checkbox,
				textbox,
			};

			class widget {
			public:
				widget();
				virtual ~widget();

				void press(const bool& pressed);
				bool pressed();
				bool is_static();
				void show(const bool& show);
				bool visible();
				void enable(const bool& enable);
				bool enabled();
				const D2D1_RECT_F& get_rect();
				void select(const bool& selected);
				bool selected();
				bool menu_visible();
				bool hit();
				HCURSOR cursor();

				virtual bool contains(const D2D1_POINT_2F& point);
				virtual bool hit(const bool& hit);
				virtual std::string page() = 0;
				virtual std::string name() = 0;
				virtual liblec::lecui::widgets_implementation::widget_type type() = 0;
				virtual HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) = 0;
				virtual void discard_resources() = 0;
				virtual D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const float& change_in_width, const float& change_in_height,
					float x_off_set, float y_off_set, const bool& render) = 0;
				virtual void on_click() = 0;
				virtual bool on_menu(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_RECT_F& client_area);
				virtual void reset_menu();
				virtual bool on_mousewheel(float units);
				virtual bool on_keydown(WPARAM wParam);

			protected:
				D2D1_RECT_F position(const liblec::lecui::rect& rect,
					const liblec::lecui::widgets::specs::on_resize& resize,
					const float& change_in_width,
					const float& change_in_height);

				virtual bool contains();	// for use with in-widget hit-testing

				std::string page_, name_;
				float dpi_scale_;
				bool is_static_;
				bool hit_;
				bool pressed_;
				D2D1_RECT_F rect_;
				bool visible_;
				bool is_enabled_;
				D2D1_POINT_2F point_;
				bool selected_;
				D2D1_POINT_2F point_on_press_, point_on_release_;
				bool draw_menu_;
				bool resources_created_;	// for the creation of widget resources on-the-fly
				HCURSOR h_cursor_;
			};
		}
	}
}
