/*
** table_impl.h - table_impl interface
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
#include "../table.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class table : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// Prevent the use of the default constructor.
				table() = delete;

				/// constructor and destructor
				table(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~table();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool on_mousewheel(float units) override;
				bool on_keydown(WPARAM wParam) override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::table::table_specs& specs();
				widgets::table::table_specs& operator()();

			private:
				/// Prevent copying an object of this class.
				table(const table&);
				table& operator=(const table&);

				/// Private variables
				widgets::table::table_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_scrollbar_border_;
				ID2D1SolidColorBrush* p_brush_text_header_;
				ID2D1SolidColorBrush* p_brush_fill_header_;
				ID2D1SolidColorBrush* p_brush_fill_alternate_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_menu_;
				ID2D1SolidColorBrush* p_brush_grid_;
				ID2D1SolidColorBrush* p_brush_row_hot_;
				ID2D1SolidColorBrush* p_brush_row_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				unsigned long scrollbar_thickness_;

				color color_scrollbar_;
				color color_scrollbar_border_;
				color color_scrollbar_hot_;
				color color_scrollbar_hot_pressed_;
				color color_scrollbar_background_;

				ID2D1SolidColorBrush* p_brush_scrollbar_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_scrollbar_background_;

				bool v_scrollbar_hit_;
				bool h_scrollbar_hit_;
				bool v_scrollbar_pressed_;
				bool h_scrollbar_pressed_;
				float v_displacement_;	// should never be greater than zero
				float h_displacement_;	// should never be greater than zero
				float v_displacement_previous_;
				float h_displacement_previous_;

				float row_height_;
				float margin_;
				D2D1_RECT_F rect_header_;
				D2D1_RECT_F rectA_;
				D2D1_RECT_F rectB_;
				bool v_scrollbar_visible_;
				bool h_scrollbar_visible_;
				float change_in_height_previous_;
				float change_in_width_previous_;
				std::map<unsigned long, D2D1_RECT_F> hot_spots_;
				unsigned long last_selected_;

				bool book_on_selection_;

				/// Private methods
				void on_selection();
			};
		}
	}
}
