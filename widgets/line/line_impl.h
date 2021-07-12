//
// line_impl.h - line_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../line.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class line_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return _specs;
				}

			public:
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

				/// widget specific methods
				widgets::line_specs& specs();
				widgets::line_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				line_impl() = delete;
				line_impl(const line_impl&) = delete;
				line_impl& operator=(const line_impl&) = delete;

				/// Private variables
				widgets::line_specs _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
			};
		}
	}
}
