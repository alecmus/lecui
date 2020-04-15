/*
** checkbox_impl.h - checkbox_impl interface
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
#include "../checkbox.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class checkbox : public widget {
			public:
				/// constructor and destructor
				checkbox(const std::string& page_alias,
					const std::string& alias,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~checkbox();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::checkbox::checkbox_specs& specs();
				widgets::checkbox::checkbox_specs& operator()();

			private:
				/// Prevent the use of the default constructor.
				checkbox() :
					checkbox(std::string(), std::string(), nullptr, nullptr) {}

				/// Prevent copying an object of this class.
				checkbox(const checkbox&);
				checkbox& operator=(const checkbox&);

				/// Private variables
				widgets::checkbox::checkbox_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_check_;
				ID2D1SolidColorBrush* p_brush_border_;
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
