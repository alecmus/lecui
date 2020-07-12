/*
** h_scrollbar_impl.h - horizontal scroll bar widget interface
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

namespace liblec {
	namespace lecui {
		namespace widgets {
			class h_scrollbar_specs : public scrollbar_specs {};

			class h_scrollbar_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				float x_displacement_previous_;
				float x_displacement_;
				float x_off_set_;
				float max_displacement_left_;
				float max_displacement_right_;
				bool force_translate_;

				/// Prevent the use of the default constructor.
				h_scrollbar_impl() = delete;

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
				void on_click() override;

				/// widget specific methods
				widgets::h_scrollbar_specs& specs();
				widgets::h_scrollbar_specs& operator()();
				void max_displacement(float& left, float& right);
				bool translate_x_displacement(const float& x_displacement,
					float& x_displacement_translated, bool force);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

			private:
				/// Prevent copying an object of this class.
				h_scrollbar_impl(const h_scrollbar_impl&);
				h_scrollbar_impl& operator=(const h_scrollbar_impl&);

				/// Private variables
				widgets::h_scrollbar_specs specs_;
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