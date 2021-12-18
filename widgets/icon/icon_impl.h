//
// icon_impl.h - icon_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include <optional>
#include "../widget_impl.h"
#include "../icon.h"
#include "../rectangle/rectangle_impl.h"
#include "../image_view/image_view_impl.h"
#include "../label/label_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class icon_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				static std::string alias_icon();
				static std::string alias_image();
				static std::string alias_text();
				static std::string alias_description();

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

				/// widget specific methods
				widgets::icon& specs();
				widgets::icon& operator()();
				void set_icon_specs(rectangle& rectangle,
					image_view& image_view,
					label& text, label& description);

			private:
				// Default constructor and copying an object of this class are not allowed
				icon_impl() = delete;
				icon_impl(const icon_impl&) = delete;
				icon_impl& operator=(const icon_impl&) = delete;

				/// Private variables
				widgets::icon _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;

				std::optional<std::reference_wrapper<rectangle>> _rectangle_specs;
				std::optional<std::reference_wrapper<image_view>> _image_view_specs;
				std::optional<std::reference_wrapper<label>> _label_specs, _description_specs;
			};
		}
	}
}
