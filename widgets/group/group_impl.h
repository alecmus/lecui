/*
** group.h - group_impl interface
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
#include "../group.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class group : public widget {
			public:
				/// constructor and destructor
				group(const std::string& page_alias,
					const std::string& alias);
				~group();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::specs::group& specs();

			private:
				/// Prevent the use of the default constructor.
				group() :
					group(std::string(), std::string()) {}

				/// Prevent copying an object of this class.
				group(const group&);
				group& operator=(const group&);

				/// Private variables
				widgets::specs::group specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
			};
		}
	}
}
