/*
** icon_impl.h - icon_impl interface
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
#include "../icon.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class icon_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				static std::string alias_icon();
				static std::string alias_image();
				static std::string alias_text();
				static std::string alias_description();

				/// Prevent the use of the default constructor.
				icon_impl() = delete;

				/// constructor and destructor
				icon_impl(containers::page& page,
					const std::string& alias);
				~icon_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::icon::icon_specs& specs();
				widgets::icon::icon_specs& operator()();

			private:
				/// Prevent copying an object of this class.
				icon_impl(const icon_impl&);
				icon_impl& operator=(const icon_impl&);

				/// Private variables
				widgets::icon::icon_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
			};
		}
	}
}
