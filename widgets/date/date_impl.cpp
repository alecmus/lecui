//
// date_impl.cpp - date_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr) {}

		widgets::date_impl::~date_impl() { discard_resources(); }

		widgets::widget_type
			widgets::date_impl::type() {
			return lecui::widgets::widget_type::date;
		}

		HRESULT widgets::date_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);

			_resources_created = true;
			return hr;
		}

		void widgets::date_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
		}

		D2D1_RECT_F&
			widgets::date_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;

				try {
					// update label widget
					if (_weekday_label_specs.has_value())
						_weekday_label_specs.value().get().color_text() = _specs.color_text();

					if (_seperator_1_specs.has_value())
						_seperator_1_specs.value().get().color_text() = _specs.color_text();

					if (_day_label_specs.has_value())
						_day_label_specs.value().get().color_text() = _specs.color_text();

					if (_month_label_specs.has_value())
						_month_label_specs.value().get().color_text() = _specs.color_text();

					if (_seperator_2_specs.has_value())
						_seperator_2_specs.value().get().color_text() = _specs.color_text();

					if (_year_label_specs.has_value())
						_year_label_specs.value().get().color_text() = _specs.color_text();

					// update border widget and background widget
					if (_day_specs.has_value()) {
						_day_specs.value().get().color_border() = _specs.color_border();
						_day_specs.value().get().color_fill() = _specs.color_fill();
					}

					if (_month_specs.has_value()) {
						_month_specs.value().get().color_border() = _specs.color_border();
						_month_specs.value().get().color_fill() = _specs.color_fill();
					}

					if (_year_specs.has_value()) {
						_year_specs.value().get().color_border() = _specs.color_border();
						_year_specs.value().get().color_fill() = _specs.color_fill();
					}

					// schedule a refresh
					_page._d_page.get_form()._d._schedule_refresh = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// use _specs._rect not _specs.rect() and _specs._on_resize not _specs.on_resize() due to redirection to special pane
			_rect = position(_specs._rect, _specs._on_resize, change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			return _rect;
		}

		widgets::date&
			widgets::date_impl::specs() { return _specs; }

		widgets::date&
			widgets::date_impl::operator()() { return specs(); }

		void widgets::date_impl::set_date_label_specs(widgets::label& weekday,
			widgets::label& seperator_1,
			widgets::label& day,
			widgets::label& month,
			widgets::label& seperator_2,
			widgets::label& year) {
			_weekday_label_specs = weekday;
			_day_label_specs = day;
			_seperator_1_specs = seperator_1;
			_month_label_specs = month;
			_seperator_2_specs = seperator_2;
			_year_label_specs = year;
		}

		void widgets::date_impl::set_date_specs(widgets::rectangle& day,
			widgets::rectangle& month, widgets::rectangle& year) {
			_day_specs = day;
			_month_specs = month;
			_year_specs = year;
		}
	}
}
