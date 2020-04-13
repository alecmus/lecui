/*
** button_impl.h - button_impl interface
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
#include "../button.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class button : public widget {
			public:
				/// constructor and destructor
				button(const std::string& page_alias,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~button();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::specs::button& specs();

			private:
				/// Prevent the use of the default constructor.
				button() :
					button(std::string(), std::string(), nullptr) {}

				/// Prevent copying an object of this class.
				button(const button&);
				button& operator=(const button&);

				/// Private variables
				widgets::specs::button specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
			};
		}
	}
}
