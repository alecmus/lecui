//
// slider_impl.h - slider_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../slider.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class slider_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				slider_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~slider_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				void on_action() override;
				bool contains(const D2D1_POINT_2F& point) override;
				bool contains() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::slider_specs& specs();
				widgets::slider_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				slider_impl() = delete;
				slider_impl(const slider_impl&) = delete;
				slider_impl& operator=(const slider_impl&) = delete;

				/// Private variables
				widgets::slider_specs specs_, specs_old_;
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
