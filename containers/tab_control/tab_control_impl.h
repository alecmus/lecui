/*
** tab_control_impl.h - tab_control_impl interface
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
#include "../tab_control.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class tab_control : public widget {
			public:
				// pages <K = tab name, T>
				std::map<std::string, containers::page> p_tabs_;
				std::string current_tab_;

				/// constructor and destructor
				tab_control(const std::string& page_alias,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~tab_control();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				containers::specs::tab_control& specs();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& tab_control_area();
				float caption_bar_height();

			private:
				/// Prevent the use of the default constructor.
				tab_control() :
					tab_control(std::string(), std::string(), nullptr) {}

				/// Prevent copying an object of this class.
				tab_control(const tab_control&);
				tab_control& operator=(const tab_control&);

				/// Private variables
				containers::specs::tab_control specs_;
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
				const float margin_;
				const float tab_gap_;
				const float tab_height_;
				const float bar_height_;
				D2D1_RECT_F rect_tabs_;
				D2D1_RECT_F rect_client_area_;
				D2D1_RECT_F rect_tab_control_;

				// tab rectangles <K = tab name, T>
				std::map<std::string, D2D1_RECT_F> p_tab_rects_;

				/// Important override for tab control to work properly.
				bool contains();
			};
		}
	}
}
