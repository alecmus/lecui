/*
** progress_indicator_impl.h - progress_bar_impl interface
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
#include "../progress_bar.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class progress_bar : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// Prevent the use of the default constructor.
				progress_bar() = delete;

				/// constructor and destructor
				progress_bar(containers::page& page,
					const std::string& alias,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~progress_bar();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::progress_bar::progress_bar_specs& specs();
				widgets::progress_bar::progress_bar_specs& operator()();

			private:
				/// Prevent copying an object of this class.
				progress_bar(const progress_bar&);
				progress_bar& operator=(const progress_bar&);

				/// Private variables
				widgets::progress_bar::progress_bar_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
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
