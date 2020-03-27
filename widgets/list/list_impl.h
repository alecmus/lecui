/*
** list_impl.h - list widget interface
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
#include "../list.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class list : public widget {
			public:
				list(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory);
				~list();

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
				bool on_mousewheel(float units);
				bool on_keydown(WPARAM wParam);
				bool hit(const bool& hit);

				// widget specific

				liblec::lecui::widgets::specs::list& specs();

			private:
				list() :
					list(std::string(), std::string(), nullptr) {}

				list(const list&);
				list& operator=(const list&);

				liblec::lecui::widgets::specs::list specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_scrollbar_border_;
				ID2D1SolidColorBrush* p_brush_text_header_;
				ID2D1SolidColorBrush* p_brush_fill_header_;
				ID2D1SolidColorBrush* p_brush_fill_alternate_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_menu_;
				ID2D1SolidColorBrush* p_brush_grid_;
				ID2D1SolidColorBrush* p_brush_row_hot_;
				ID2D1SolidColorBrush* p_brush_row_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				unsigned long scrollbar_thickness_;

				liblec::lecui::color color_scrollbar_;
				liblec::lecui::color color_scrollbar_border_;
				liblec::lecui::color color_scrollbar_hot_;
				liblec::lecui::color color_scrollbar_hot_pressed_;
				liblec::lecui::color color_scrollbar_background_;

				ID2D1SolidColorBrush* p_brush_scrollbar_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_scrollbar_background_;

				bool v_scrollbar_hit_;
				bool h_scrollbar_hit_;
				bool v_scrollbar_pressed_;
				bool h_scrollbar_pressed_;
				float v_displacement_;	// should never be greater than zero
				float h_displacement_;	// should never be greater than zero
				float v_displacement_previous_;
				float h_displacement_previous_;

				float row_height_;
				float margin_;
				D2D1_RECT_F rect_header_;
				D2D1_RECT_F rectA_;
				D2D1_RECT_F rectB_;
				bool v_scrollbar_visible_;
				bool h_scrollbar_visible_;
				float change_in_height_previous_;
				float change_in_width_previous_;
				std::map<unsigned long, D2D1_RECT_F> hot_spots_;
				unsigned long last_selected_;

				void on_selection();
				bool book_on_selection_;
			};
		}
	}
}
