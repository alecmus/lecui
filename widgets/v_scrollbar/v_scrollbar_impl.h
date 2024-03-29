//
// v_scrollbar_impl.h - vertical scroll bar widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class v_scrollbar_specs : public scrollbar_specs {};

			class v_scrollbar_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				float _y_displacement_previous;
				float _y_displacement;
				float _y_off_set;
				float _max_displacement_top;
				float _max_displacement_bottom;
				bool _force_translate;

				float _programmatic_v_scroll = 0.f;
				float _scheduled_programmatic_v_scroll = 0.f;

				/// constructor and destructor
				v_scrollbar_impl(containers::page& page);
				~v_scrollbar_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::v_scrollbar_specs& specs();
				widgets::v_scrollbar_specs& operator()();
				void max_displacement(float& top, float& bottom);
				bool translate_y_displacement(const float& y_displacement,
					float& y_displacement_translated, bool force);
				void reverse_translate_y_displacement(const float& y_displacement_translated,
					float& y_displacement);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

			private:
				// Default constructor and copying an object of this class are not allowed
				v_scrollbar_impl() = delete;
				v_scrollbar_impl(const v_scrollbar_impl&) = delete;
				v_scrollbar_impl& operator=(const v_scrollbar_impl&) = delete;

				/// Private variables
				widgets::v_scrollbar_specs _specs;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_hot_pressed;
				ID2D1SolidColorBrush* _p_brush_background;
				D2D1_RECT_F _rectA, _rectB, _rectC, _rectD;
			};
		}
	}
}
