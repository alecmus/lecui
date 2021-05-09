//
// v_scrollbar_impl.h - vertical scroll bar widget interface
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
			class v_scrollbar_specs : public scrollbar_specs {};

			class v_scrollbar_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				float y_displacement_previous_;
				float y_displacement_;
				float y_off_set_;
				float max_displacement_top_;
				float max_displacement_bottom_;
				bool force_translate_;

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
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

			private:
				// Default constructor and copying an object of this class are not allowed
				v_scrollbar_impl() = delete;
				v_scrollbar_impl(const v_scrollbar_impl&) = delete;
				v_scrollbar_impl& operator=(const v_scrollbar_impl&) = delete;

				/// Private variables
				widgets::v_scrollbar_specs specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_background_;
				D2D1_RECT_F rectA_, rectB_, rectC_, rectD_;
			};
		}
	}
}
