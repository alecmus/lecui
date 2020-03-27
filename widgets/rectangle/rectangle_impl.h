/*
** rectangle_impl.h - rectangle widget interface
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
#include "../rectangle.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class rectangle : public widget {
			public:
				rectangle(const std::string& page,
					const std::string& name);
				~rectangle();

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

				liblec::lecui::widgets::specs::rectangle& specs();

			private:
				rectangle() :
					rectangle(std::string(), std::string()) {}

				rectangle(const rectangle&);
				rectangle& operator=(const rectangle&);

				liblec::lecui::widgets::specs::rectangle specs_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
			};
		}
	}
}
