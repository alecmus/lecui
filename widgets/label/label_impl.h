/*
** label_impl.h - label widget interface
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
#include "../label.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class label : public widget {
			public:
				label(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory);
				~label();

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

				liblec::lecui::widgets::specs::label& specs();

			private:
				label() :
					label(std::string(), std::string(), nullptr) {}

				label(const label&);
				label& operator=(const label&);

				struct text_range_properties
				{
					DWRITE_TEXT_RANGE text_range = { 0, 0 };
					std::string font;
					float size = 0.0f;
					bool bold = false;
					bool italic = false;
					bool underline = false;
					D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f };
				};

				liblec::lecui::widgets::specs::label specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				std::string text_;
				std::vector<text_range_properties> formatting_;
			};
		}
	}
}
