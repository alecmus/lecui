/*
** combobox_impl.h - combobox_impl interface
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
#include "../combobox.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class combobox : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				combobox(const std::string& page_alias,
					const std::string& alias,
					form& fm,
					IDWriteFactory* p_directwrite_factory);
				~combobox();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources();
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::combobox::combobox_specs& specs();
				widgets::combobox::combobox_specs& operator()();

			private:
				/// Prevent the use of the default constructor.
				combobox();

				/// Prevent copying an object of this class.
				combobox(const combobox&);
				combobox& operator=(const combobox&);

				/// Private variables
				widgets::combobox::combobox_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_dropdown_arrow_;
				ID2D1SolidColorBrush* p_brush_dropdown_arrow_hot_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				D2D1_RECT_F rect_dropdown_, rect_text_, rect_combobox_;

				form& fm_;

				std::string dropdown(D2D1_RECT_F rect);
			};
		}
	}
}
