/*
** close_button.h - close button widget interface
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

#include "../../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class close_button_specs : public specs {};
		}

		namespace widgets_impl {
			class close_button : public widget {
			public:
				close_button();
				~close_button();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::close_button_specs& specs();
				widgets::close_button_specs& operator()();

			private:
				/// Prevent copying an object of this class.
				close_button(const close_button&);
				close_button& operator=(const close_button&);

				widgets::close_button_specs specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
			};
		}
	}
}
