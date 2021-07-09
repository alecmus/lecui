//
// table_view_impl.h - table_view_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../table_view.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class table_view_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				table_view_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~table_view_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool on_keydown(WPARAM wParam) override;
				bool hit(const bool& hit) override;
				void on_right_click() override;

				/// widget specific methods
				widgets::table_view_specs& specs();
				widgets::table_view_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				table_view_impl() = delete;
				table_view_impl(const table_view_impl&) = delete;
				table_view_impl& operator=(const table_view_impl&) = delete;

				/// Private variables
				widgets::table_view_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_text_header_;
				ID2D1SolidColorBrush* p_brush_text_header_hot_;
				ID2D1SolidColorBrush* p_brush_text_selected_;
				ID2D1SolidColorBrush* p_brush_fill_header_;
				ID2D1SolidColorBrush* p_brush_fill_alternate_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_grid_;
				ID2D1SolidColorBrush* p_brush_row_hot_;
				ID2D1SolidColorBrush* p_brush_row_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				D2D1_RECT_F rectA_, rectB_;
				float row_height_;
				float margin_;
				D2D1_RECT_F rect_header_;
				std::map<unsigned long, D2D1_RECT_F> hot_spots_;
				std::map<std::string, D2D1_RECT_F> header_hot_spots_;
				unsigned long last_selected_;

				bool book_on_selection_;

				/// Private methods
				void on_selection();
			};
		}
	}
}
