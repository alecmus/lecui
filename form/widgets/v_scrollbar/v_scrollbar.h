/*
** v_scrollbar.h - vertical scroll bar widget interface
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
				class v_scrollbar : public scrollbar {};
			}
		}

		namespace widgets_implementation {
			class v_scrollbar : public widget {
			public:
				v_scrollbar(const std::string& page);
				~v_scrollbar();

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

				liblec::lecui::widgets::specs::v_scrollbar& specs();
				void max_displacement(float& top, float& bottom);
				bool translate_y_displacement(const float& y_displacement,
					float& y_displacement_translated, bool force);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

				float y_displacement_previous_, y_displacement_, y_off_set_;
				float max_displacement_top_, max_displacement_bottom_;
				bool force_translate_;

			private:
				v_scrollbar() :
					v_scrollbar(std::string()) {}

				v_scrollbar(const v_scrollbar&);
				v_scrollbar& operator=(const v_scrollbar&);

				liblec::lecui::widgets::specs::v_scrollbar specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_background_;
				D2D1_RECT_F rectA_, rectB_, rectC_, rectD_;
			};
		}
	}
}
