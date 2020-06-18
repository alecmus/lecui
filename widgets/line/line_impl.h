/*
** line_impl.h - line_impl interface
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
#include "../line.h"

#include "../../widgets/h_scrollbar/h_scrollbar.h"
#include "../../widgets/v_scrollbar/v_scrollbar.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class line : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				line(const std::string& page_alias,
					const std::string& alias);
				~line();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::line::line_specs& specs();
				widgets::line::line_specs& operator()();

			private:
				/// Prevent the use of the default constructor.
				line();

				/// Prevent copying an object of this class.
				line(const line&);
				line& operator=(const line&);

				/// Private variables
				widgets::line::line_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
			};
		}
	}
}
