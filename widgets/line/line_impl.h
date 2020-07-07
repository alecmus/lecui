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

#include "../../widgets/h_scrollbar/h_scrollbar_impl.h"
#include "../../widgets/v_scrollbar/v_scrollbar_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class line_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// Prevent the use of the default constructor.
				line_impl() = delete;

				/// constructor and destructor
				line_impl(containers::page& page,
					const std::string& alias);
				~line_impl();

				/// virtual function overrides
				widget_type type() override;
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
				/// Prevent copying an object of this class.
				line_impl(const line_impl&);
				line_impl& operator=(const line_impl&);

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
