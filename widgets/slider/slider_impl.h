/*
** slider_impl.h - slider_impl interface
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
#include "../slider.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class slider : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				slider(const std::string& page_alias,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~slider();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool contains(const D2D1_POINT_2F& point) override;
				bool contains() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::slider::slider_specs& specs();
				widgets::slider::slider_specs& operator()();

			private:
				/// Prevent the use of the default constructor.
				slider() :
					slider(std::string(), std::string(), nullptr) {}

				/// Prevent copying an object of this class.
				slider(const slider&);
				slider& operator=(const slider&);

				/// Private variables
				widgets::slider::slider_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_knob_;
				ID2D1SolidColorBrush* p_brush_knob_hot_;
				ID2D1SolidColorBrush* p_brush_knob_border_;
				ID2D1SolidColorBrush* p_brush_tick_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				D2D1_RECT_F rect_slider_;
				float perc_along_;
			};
		}
	}
}