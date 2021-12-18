//
// date_impl.h - date_impl interface
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
#include "../date.h"
#include "../label/label_impl.h"
#include "../rectangle/rectangle_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class date_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				static std::string alias_day();
				static std::string alias_day_label();
				static std::string alias_month();
				static std::string alias_month_label();
				static std::string alias_year();
				static std::string alias_year_label();
				static std::string alias_weekday();
				static std::string alias_weekday_label();

				/// constructor and destructor
				date_impl(containers::page& page,
					const std::string& alias);
				~date_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::date& specs();
				widgets::date& operator()();
				void set_date_label_specs(widgets::label& weekday,
					widgets::label& day,
					widgets::label& seperator_1,
					widgets::label& month,
					widgets::label& seperator_2,
					widgets::label& year);
				void set_date_specs(widgets::rectangle& day,
					widgets::rectangle& month,
					widgets::rectangle& year);

			private:
				// Default constructor and copying an object of this class are not allowed
				date_impl() = delete;
				date_impl(const date_impl&) = delete;
				date_impl& operator=(const date_impl&) = delete;

				/// Private variables
				widgets::date _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;

				std::optional<std::reference_wrapper<widgets::label>>
					_weekday_label_specs, _day_label_specs, _seperator_1_specs,
					_month_label_specs, _seperator_2_specs, _year_label_specs;
				std::optional<std::reference_wrapper<widgets::rectangle>>
					_day_specs, _month_specs, _year_specs;
			};
		}
	}
}
