/*
** combo_impl.h - combo_impl interface
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
#include "../combo.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class combo : public widget {
			public:
				/// constructor and destructor
				combo(const std::string& page_alias,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~combo();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources();
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool on_menu(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_RECT_F& client_area) override;
				void reset_menu() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::combo_specs& specs();

			private:
				/// Prevent the use of the default constructor.
				combo() :
					combo(std::string(), std::string(), nullptr) {}

				/// Prevent copying an object of this class.
				combo(const combo&);
				combo& operator=(const combo&);

				/// Private variables
				widgets::combo_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_menu_;
				ID2D1SolidColorBrush* p_brush_menu_hot_;
				ID2D1SolidColorBrush* p_brush_menu_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				
				D2D1_RECT_F rect_dropdown_, rect_text_;
				unsigned long scrollbar_thickness_;

				color color_scrollbar_;
				color color_scrollbar_hot_;
				color color_scrollbar_hot_pressed_;
				color color_scrollbar_background_;

				ID2D1SolidColorBrush* p_brush_scrollbar_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_;
				ID2D1SolidColorBrush* p_brush_scrollbar_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_scrollbar_background_;

				D2D1_RECT_F rect_menu_;
				bool menu_below_;
				D2D1_RECT_F rect_combo_;

				std::map<std::string, D2D1_RECT_F> hot_spots_;
				bool scrollbar_hit_;
				bool scrollbar_pressed_;
				long move_by_;
				float displacement_;
				float displacement_previous_;
			};
		}
	}
}
