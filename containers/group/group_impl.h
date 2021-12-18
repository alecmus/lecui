//
// group_impl.h - group_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../../widgets/widget_impl.h"
#include "../group.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class group_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				group_impl(containers::page& page,
					const std::string& alias);
				~group_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				containers::group& specs();
				containers::group& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				group_impl() = delete;
				group_impl(const group_impl&) = delete;
				group_impl& operator=(const group_impl&) = delete;

				/// Private variables
				containers::group _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
			};
		}
	}
}
