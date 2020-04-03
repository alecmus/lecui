/*
** toggle_impl.h - toggle widget interface
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
#include "../toggle.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class toggle : public widget {
			public:
				toggle(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory);
				~toggle();

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
				bool contains(const D2D1_POINT_2F& point);
				bool contains();
				bool hit(const bool& hit);

				// widget specific

				liblec::lecui::widgets::specs::toggle& specs();

			private:
				toggle() :
					toggle(std::string(), std::string(), nullptr) {}

				toggle(const toggle&);
				toggle& operator=(const toggle&);

				liblec::lecui::widgets::specs::toggle specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_on_;
				ID2D1SolidColorBrush* p_brush_off_;
				ID2D1SolidColorBrush* p_brush_on_hot_;
				ID2D1SolidColorBrush* p_brush_off_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				D2D1_RECT_F rect_toggle_;
				D2D1_RECT_F rect_text_;
				float perc_along_;
			};
		}
	}
}
