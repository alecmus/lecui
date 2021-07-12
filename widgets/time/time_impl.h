//
// time_impl.h - time_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include <optional>
#include "../widget_impl.h"
#include "../time.h"
#include "../label/label_impl.h"
#include "../rectangle/rectangle_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class time_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return _specs;
				}

			public:
				static std::string alias_hour();
				static std::string alias_hour_label();
				static std::string alias_minute();
				static std::string alias_minute_label();
				static std::string alias_second();
				static std::string alias_second_label();

				/// constructor and destructor
				time_impl(containers::page& page,
					const std::string& alias);
				~time_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				void on_right_click() override;

				/// widget specific methods
				widgets::time_specs& specs();
				widgets::time_specs& operator()();
				void set_time_label_specs(widgets::label_specs& hour,
					widgets::label_specs& seperator_1,
					widgets::label_specs& minute,
					widgets::label_specs& seperator_2,
					widgets::label_specs& second);
				void set_time_specs(widgets::rectangle_specs& hour,
					widgets::rectangle_specs& minute,
					widgets::rectangle_specs& second);

			private:
				// Default constructor and copying an object of this class are not allowed
				time_impl() = delete;
				time_impl(const time_impl&) = delete;
				time_impl& operator=(const time_impl&) = delete;

				/// Private variables
				widgets::time_specs _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;

				std::optional<std::reference_wrapper<widgets::label_specs>>
					_hour_label_specs, _seperator_1_specs, _minute_label_specs,
					_seperator_2_specs, _second_label_specs;
				std::optional<std::reference_wrapper<widgets::rectangle_specs>>
					_hour_specs, _minute_specs, _second_specs;
			};
		}
	}
}
