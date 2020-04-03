/*
** combo_impl.h - combo widget interface
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

#include "../widget_impl.h"
#include "../combo.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class combo : public widget {
			public:
				combo(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory);
				~combo();

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
				bool on_menu(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_RECT_F& client_area);
				void reset_menu();
				bool hit(const bool& hit);

				// widget specific

				liblec::lecui::widgets::specs::combo& specs();

			private:
				combo() :
					combo(std::string(), std::string(), nullptr) {}

				combo(const combo&);
				combo& operator=(const combo&);

				liblec::lecui::widgets::specs::combo specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_menu_;
				ID2D1SolidColorBrush* p_brush_menu_hot_;
				ID2D1SolidColorBrush* p_brush_menu_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				
				D2D1_RECT_F rect_dropdown_, rect_text_;
				unsigned long scrollbar_thickness_;

				liblec::lecui::color color_scrollbar_;
				liblec::lecui::color color_scrollbar_hot_;
				liblec::lecui::color color_scrollbar_hot_pressed_;
				liblec::lecui::color color_scrollbar_background_;

				ID2D1SolidColorBrush* p_brush_scrollbar_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_scrollbar_background_;

				D2D1_RECT_F rect_menu_;
				bool menu_below_;
				D2D1_RECT_F rect_combo_;

				std::map<std::string, D2D1_RECT_F> hot_spots_;
				bool scrollbar_hit_;
				bool scrollbar_pressed_;
				long move_by_;
				float displacement_;
				float displacement_previous_;
			};
		}
	}
}
