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
				widgets::widget& generic_specs() override {
					return _specs;
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
				void on_right_click() override;
				bool contains(const D2D1_POINT_2F& point) override;
				bool contains() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::slider& specs();
				widgets::slider& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				slider_impl() = delete;
				slider_impl(const slider_impl&) = delete;
				slider_impl& operator=(const slider_impl&) = delete;

				/// Private variables
				widgets::slider _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_knob;
				ID2D1SolidColorBrush* _p_brush_knob_hot;
				ID2D1SolidColorBrush* _p_brush_knob_border;
				ID2D1SolidColorBrush* _p_brush_tick;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;
				D2D1_RECT_F _rect_slider;
				float _perc_along;
			};
		}
	}
}
