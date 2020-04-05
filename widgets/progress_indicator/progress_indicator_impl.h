/*
** progress_indicator_impl.h - progress_indicator widget interface
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
#include "../progress_indicator.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class progress_indicator : public widget {
			public:
				progress_indicator(const std::string& page,
					const std::string& name,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~progress_indicator();

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

				liblec::lecui::widgets::specs::progress_indicator& specs();

			private:
				progress_indicator() :
					progress_indicator(std::string(), std::string(), nullptr, nullptr) {}

				progress_indicator(const progress_indicator&);
				progress_indicator& operator=(const progress_indicator&);

				liblec::lecui::widgets::specs::progress_indicator specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_empty_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				ID2D1Factory* p_direct2d_factory_;
				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
			};
		}
	}
}
