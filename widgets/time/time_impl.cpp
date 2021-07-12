//
// time_impl.cpp - time_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "time_impl.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets::time_impl::alias_hour() {
			return std::string("lecui::widgets::time::hour");
		}

		std::string widgets::time_impl::alias_hour_label() {
			return std::string("lecui::widgets::time::hour_lbl");
		}

		std::string widgets::time_impl::alias_minute() {
			return std::string("lecui::widgets::time::minute");
		}

		std::string widgets::time_impl::alias_minute_label() {
			return std::string("lecui::widgets::time::minute_lbl");
		}

		std::string widgets::time_impl::alias_second() {
			return std::string("lecui::widgets::time::second");
		}

		std::string widgets::time_impl::alias_second_label() {
			return std::string("lecui::widgets::time::second_lbl");
		}

		widgets::time_impl::time_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr) {}

		widgets::time_impl::~time_impl() { discard_resources(); }

		widgets::widget_type
			widgets::time_impl::type() {
			return lecui::widgets::widget_type::time;
		}

		HRESULT widgets::time_impl::create_resources(
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

		void widgets::time_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
		}

		D2D1_RECT_F&
			widgets::time_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;

				try {
					// update label specs
					if (_hour_label_specs.has_value())
						_hour_label_specs.value().get().color_text() = _specs.color_text();

					if (_seperator_1_specs.has_value())
						_seperator_1_specs.value().get().color_text() = _specs.color_text();

					if (_minute_label_specs.has_value())
						_minute_label_specs.value().get().color_text() = _specs.color_text();

					if (_seperator_2_specs.has_value())
						_seperator_2_specs.value().get().color_text() = _specs.color_text();

					if (_second_label_specs.has_value())
						_second_label_specs.value().get().color_text() = _specs.color_text();

					// update border specs and background specs
					if (_hour_specs.has_value()) {
						_hour_specs.value().get().color_border() = _specs.color_border();
						_hour_specs.value().get().color_fill() = _specs.color_fill();
					}

					if (_minute_specs.has_value()) {
						_minute_specs.value().get().color_border() = _specs.color_border();
						_minute_specs.value().get().color_fill() = _specs.color_fill();
					}

					if (_second_specs.has_value()) {
						_second_specs.value().get().color_border() = _specs.color_border();
						_second_specs.value().get().color_fill() = _specs.color_fill();
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

		void widgets::time_impl::on_click() {
			if (_specs.events().click)
				_specs.events().click();
		}

		void widgets::time_impl::on_right_click() {
			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		widgets::time_specs&
			widgets::time_impl::specs() { return _specs; }

		widgets::time_specs&
			widgets::time_impl::operator()() { return specs(); }

		void widgets::time_impl::set_time_label_specs(widgets::label_specs& hour,
			widgets::label_specs& seperator_1,
			widgets::label_specs& minute,
			widgets::label_specs& seperator_2,
			widgets::label_specs& second) {
			_hour_label_specs = hour;
			_seperator_1_specs = seperator_1;
			_minute_label_specs = minute;
			_seperator_2_specs = seperator_2;
			_second_label_specs = second;
		}

		void widgets::time_impl::set_time_specs(widgets::rectangle_specs& hour,
			widgets::rectangle_specs& minute,
			widgets::rectangle_specs& second) {
			_hour_specs = hour;
			_minute_specs = minute;
			_second_specs = second;
		}
	}
}
