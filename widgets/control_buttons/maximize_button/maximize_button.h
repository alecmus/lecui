/*
** maximize_button.h - maximize button widget interface
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

#include "../../../form_common.h"
#include "../../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			namespace specs {
				class maximize_button : public widget {};
			}
		}

		namespace widgets_implementation {
			class maximize_button : public widget {
			public:
				maximize_button();
				~maximize_button();

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

				void set_hwnd(HWND hWnd);
				liblec::lecui::widgets::specs::maximize_button& specs();

			private:
				maximize_button(const maximize_button&);
				maximize_button& operator=(const maximize_button&);

				HWND hWnd_;
				liblec::lecui::widgets::specs::maximize_button specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;

				bool maximized(HWND hwnd);
			};
		}
	}
}
