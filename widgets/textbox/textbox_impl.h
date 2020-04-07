/*
** textbox_impl.h - textbox_impl interface
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
#include "../textbox.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class textbox : public widget {
			public:
				textbox(const std::string& page,
					const std::string& name,
					form& fm,
					IDWriteFactory* p_directwrite_factory);
				~textbox();

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
				void on_selection_change(const bool& selected) override;

				// widget specific

				liblec::lecui::widgets::specs::textbox& specs();
				void insert_character(const char& c);
				void key_backspace();
				void key_delete();
				void key_left();
				void key_right();

			private:
				textbox(const textbox&);
				textbox& operator=(const textbox&);

				liblec::lecui::widgets::specs::textbox specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_caret_;
				ID2D1SolidColorBrush* p_brush_prompt_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				ID2D1Factory* p_direct2d_factory_;
				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				
				const std::string caret_blink_timer_name_;
				UINT32 caret_position_;
				bool caret_visible_;

				form& fm_;
			};
		}
	}
}
