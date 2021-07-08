//
// date_impl.cpp - date_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "date_impl.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets::date_impl::alias_day() {
			return std::string("lecui::widgets::date::day");
		}

		std::string widgets::date_impl::alias_day_label() {
			return std::string("lecui::widgets::date::day_lbl");
		}

		std::string widgets::date_impl::alias_month() {
			return std::string("lecui::widgets::date::month");
		}

		std::string widgets::date_impl::alias_month_label() {
			return std::string("lecui::widgets::date::month_lbl");
		}

		std::string widgets::date_impl::alias_year() {
			return std::string("lecui::widgets::date::year");
		}

		std::string widgets::date_impl::alias_year_label() {
			return std::string("lecui::widgets::date::year_lbl");
		}

		std::string widgets::date_impl::alias_weekday() {
			return std::string("lecui::widgets::date::weekday");
		}

		std::string widgets::date_impl::alias_weekday_label() {
			return std::string("lecui::widgets::date::weekday_lbl");
		}

		widgets::date_impl::date_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr) {}

		widgets::date_impl::~date_impl() { discard_resources(); }

		widgets::widget_type
			widgets::date_impl::type() {
			return lecui::widgets::widget_type::date;
		}

		HRESULT widgets::date_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot()),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled()),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected()),
					&p_brush_selected_);

			resources_created_ = true;
			return hr;
		}

		void widgets::date_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
		}

		D2D1_RECT_F&
			widgets::date_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;

				try {
					// update label specs
					if (weekday_label_specs_.has_value())
						weekday_label_specs_.value().get().color_text() = specs_.color_text();

					if (seperator_1_specs_.has_value())
						seperator_1_specs_.value().get().color_text() = specs_.color_text();

					if (day_label_specs_.has_value())
						day_label_specs_.value().get().color_text() = specs_.color_text();

					if (month_label_specs_.has_value())
						month_label_specs_.value().get().color_text() = specs_.color_text();

					if (seperator_2_specs_.has_value())
						seperator_2_specs_.value().get().color_text() = specs_.color_text();

					if (year_label_specs_.has_value())
						year_label_specs_.value().get().color_text() = specs_.color_text();

					// update border specs and background specs
					if (day_specs_.has_value()) {
						day_specs_.value().get().color_border() = specs_.color_border();
						day_specs_.value().get().color_fill() = specs_.color_fill();
					}

					if (month_specs_.has_value()) {
						month_specs_.value().get().color_border() = specs_.color_border();
						month_specs_.value().get().color_fill() = specs_.color_fill();
					}

					if (year_specs_.has_value()) {
						year_specs_.value().get().color_border() = specs_.color_border();
						year_specs_.value().get().color_fill() = specs_.color_fill();
					}

					// schedule a refresh
					page_.d_page_.get_form().d_.schedule_refresh_ = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			// use specs_.rect_ not specs_.rect() and specs_.on_resize_ not specs_.on_resize() due to redirection to special pane
			rect_ = position(specs_.rect_, specs_.on_resize_, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			return rect_;
		}

		widgets::date_specs&
			widgets::date_impl::specs() { return specs_; }

		widgets::date_specs&
			widgets::date_impl::operator()() { return specs(); }

		void widgets::date_impl::set_date_label_specs(widgets::label_specs& weekday,
			widgets::label_specs& seperator_1,
			widgets::label_specs& day,
			widgets::label_specs& month,
			widgets::label_specs& seperator_2,
			widgets::label_specs& year) {
			weekday_label_specs_ = weekday;
			day_label_specs_ = day;
			seperator_1_specs_ = seperator_1;
			month_label_specs_ = month;
			seperator_2_specs_ = seperator_2;
			year_label_specs_ = year;
		}

		void widgets::date_impl::set_date_specs(widgets::rectangle_specs& day,
			widgets::rectangle_specs& month, widgets::rectangle_specs& year) {
			day_specs_ = day;
			month_specs_ = month;
			year_specs_ = year;
		}
	}
}
