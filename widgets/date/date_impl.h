/*
** date_impl.h - date_impl interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

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
				widgets::specs& generic_specs() override {
					return specs_;
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
				widgets::date::date_specs& specs();
				widgets::date::date_specs& operator()();
				void set_date_label_specs(widgets::label::label_specs& weekday,
					widgets::label::label_specs& day,
					widgets::label::label_specs& seperator_1,
					widgets::label::label_specs& month,
					widgets::label::label_specs& seperator_2,
					widgets::label::label_specs& year);
				void set_date_specs(widgets::rectangle::rectangle_specs& day,
					widgets::rectangle::rectangle_specs& month,
					widgets::rectangle::rectangle_specs& year);

			private:
				// Default constructor and copying an object of this class are not allowed
				date_impl() = delete;
				date_impl(const date_impl&) = delete;
				date_impl& operator=(const date_impl&) = delete;

				/// Private variables
				widgets::date::date_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;

				std::optional<std::reference_wrapper<widgets::label::label_specs>>
					weekday_label_specs_, day_label_specs_, seperator_1_specs_,
					month_label_specs_, seperator_2_specs_, year_label_specs_;
				std::optional<std::reference_wrapper<widgets::rectangle::rectangle_specs>>
					day_specs_, month_specs_, year_specs_;
			};
		}
	}
}
