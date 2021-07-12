//
// h_scrollbar_impl.h - horizontal scroll bar widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class h_scrollbar_specs : public scrollbar_specs {};

			class h_scrollbar_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return _specs;
				}

			public:
				float _x_displacement_previous;
				float _x_displacement;
				float _x_off_set;
				float _max_displacement_left;
				float _max_displacement_right;
				bool _force_translate;

				/// constructor and destructor
				h_scrollbar_impl(containers::page& page);
				~h_scrollbar_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::h_scrollbar_specs& specs();
				widgets::h_scrollbar_specs& operator()();
				void max_displacement(float& left, float& right);
				bool translate_x_displacement(const float& x_displacement,
					float& x_displacement_translated, bool force);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

			private:
				// Default constructor and copying an object of this class are not allowed
				h_scrollbar_impl() = delete;
				h_scrollbar_impl(const h_scrollbar_impl&) = delete;
				h_scrollbar_impl& operator=(const h_scrollbar_impl&) = delete;

				/// Private variables
				widgets::h_scrollbar_specs _specs;
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
