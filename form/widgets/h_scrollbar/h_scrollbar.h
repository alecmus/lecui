/*
** h_scrollbar.h - horizontal scroll bar widget interface
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

#include "../../form_common.h"
#include "../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			namespace specs {
				class h_scrollbar : public widget {
				public:
					liblec::lecui::color color_hot_pressed;
					liblec::lecui::color color_background;
				};
			}
		}

		namespace widgets_implementation {
			class h_scrollbar : public widget {
			public:
				h_scrollbar(const std::string& page);
				~h_scrollbar();

				// virtual function override

				std::string page();
				std::string name();
				virtual liblec::lecui::widgets_implementation::widget_type type();
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target);
				void discard_resources();
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const float& change_in_width, const float& change_in_height, float x_off_set,
					float y_off_set, const bool& render);
				void on_click();

				// widget specific

				liblec::lecui::widgets::specs::h_scrollbar& specs();
				void max_displacement(float& left, float& right);
				bool translate_x_displacement(const float& x_displacement,
					float& x_displacement_translated, bool force);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

				float x_displacement_previous_, x_displacement_, x_off_set_;
				float max_displacement_left_, max_displacement_right_;
				bool force_translate_;

			private:
				h_scrollbar() :
					h_scrollbar(std::string()) {}

				h_scrollbar(const h_scrollbar&);
				h_scrollbar& operator=(const h_scrollbar&);

				liblec::lecui::widgets::specs::h_scrollbar specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_background_;
				D2D1_RECT_F rectA_, rectB_, rectC_, rectD_;
			};
		}
	}
}
