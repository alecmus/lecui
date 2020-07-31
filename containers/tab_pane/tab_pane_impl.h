/*
** tab_pane_impl.h - tab_pane_impl interface
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

#include "../../widgets/widget_impl.h"
#include "../tab_pane.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class tab_pane_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				// pages <K = tab name, T>
				std::map<std::string, containers::page> p_tabs_;
				std::vector<std::string> tab_order_;
				std::string current_tab_;

				/// constructor and destructor
				tab_pane_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~tab_pane_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				containers::tab_pane::tab_pane_specs& specs();
				containers::tab_pane::tab_pane_specs& operator()();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& tab_pane_area();
				float caption_bar_height();
				float tab_height();
				float bar_height();
				float padding();
				float margin();
				void set_tab_height(const float height);
				bool tab_height_set();
				void close_tab(const std::string& tab_name);

			private:
				// Default constructor and copying an object of this class are not allowed
				tab_pane_impl() = delete;
				tab_pane_impl(const tab_pane_impl&) = delete;
				tab_pane_impl& operator=(const tab_pane_impl&) = delete;

				/// Private variables
				containers::tab_pane::tab_pane_specs specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_tabs_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				const float margin_, padding_;
				const float tab_gap_;
				float tab_height_;
				bool tab_height_set_;
				const float bar_height_;
				D2D1_RECT_F rect_tabs_;
				D2D1_RECT_F rect_client_area_;
				D2D1_RECT_F rect_tab_pane_;

				// tab rectangles <K = tab name, T>
				std::map<std::string, D2D1_RECT_F> p_tab_rects_;

				/// Important override for tab pane to work properly.
				bool contains();
			};
		}
	}
}
