/*
** tab_control_impl.h - tab control widget interface
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

#include "../../widgets/widget_impl.h"
#include "../tab_control.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class tab_control : public widget {
			public:
				tab_control(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory);
				~tab_control();

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

				liblec::lecui::containers::specs::tab_control& specs();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& tab_control_area();
				float caption_bar_height();

				// pages <K = tab name, T>
				std::map<std::string, liblec::lecui::containers::page> p_tabs_;
				std::string current_tab_;

			private:
				tab_control() :
					tab_control(std::string(), std::string(), nullptr) {}

				tab_control(const tab_control&);
				tab_control& operator=(const tab_control&);

				liblec::lecui::containers::specs::tab_control specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_tabs_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				const float margin_;
				const float tab_gap_;
				const float tab_height_;
				const float bar_height_;
				D2D1_RECT_F rect_tabs_;
				D2D1_RECT_F rect_client_area_;
				D2D1_RECT_F rect_tab_control_;

				// tab rectangles <K = tap name, T>
				std::map<std::string, D2D1_RECT_F> p_tab_rects_;

				bool contains();	// override of function for hit-testing in-widget controls
			};
		}
	}
}
