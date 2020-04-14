/*
** v_scrollbar.h - vertical scroll bar widget interface
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
			class v_scrollbar_specs : public scrollbar_specs {};
		}

		namespace widgets_impl {
			class v_scrollbar : public widget {
			public:
				float y_displacement_previous_;
				float y_displacement_;
				float y_off_set_;
				float max_displacement_top_;
				float max_displacement_bottom_;
				bool force_translate_;

				/// constructor and destructor
				v_scrollbar(const std::string& page_alias);
				~v_scrollbar();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::v_scrollbar_specs& specs();
				void max_displacement(float& top, float& bottom);
				bool translate_y_displacement(const float& y_displacement,
					float& y_displacement_translated, bool force);
				void setup(const D2D1_RECT_F& rectA, const D2D1_RECT_F& rectB);

			private:
				/// Prevent the use of the default constructor.
				v_scrollbar() :
					v_scrollbar(std::string()) {}

				/// Prevent copying an object of this class.
				v_scrollbar(const v_scrollbar&);
				v_scrollbar& operator=(const v_scrollbar&);

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
