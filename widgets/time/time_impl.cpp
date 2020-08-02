/*
** time_impl.cpp - time_impl implementation
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
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr) {}

		widgets::time_impl::~time_impl() { discard_resources(); }

		widgets::widget_type
			widgets::time_impl::type() {
			return lecui::widgets::widget_type::time;
		}

		HRESULT widgets::time_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);

			resources_created_ = true;
			return hr;
		}

		void widgets::time_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
		}

		D2D1_RECT_F&
			widgets::time_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;

				try {
					// update label specs
					if (hour_label_specs_.has_value())
						hour_label_specs_.value().get().color_text = specs_.color_text;

					if (seperator_1_specs_.has_value())
						seperator_1_specs_.value().get().color_text = specs_.color_text;

					if (minute_label_specs_.has_value())
						minute_label_specs_.value().get().color_text = specs_.color_text;

					if (seperator_2_specs_.has_value())
						seperator_2_specs_.value().get().color_text = specs_.color_text;

					if (second_label_specs_.has_value())
						second_label_specs_.value().get().color_text = specs_.color_text;

					// update border specs and background specs
					if (hour_specs_.has_value()) {
						hour_specs_.value().get().color_border = specs_.color_border;
						hour_specs_.value().get().color_fill = specs_.color_fill;
					}

					if (minute_specs_.has_value()) {
						minute_specs_.value().get().color_border = specs_.color_border;
						minute_specs_.value().get().color_fill = specs_.color_fill;
					}

					if (second_specs_.has_value()) {
						second_specs_.value().get().color_border = specs_.color_border;
						second_specs_.value().get().color_fill = specs_.color_fill;
					}

					// schedule a refresh
					page_.d_page_.get_form().d_.schedule_refresh_ = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			return rect_;
		}

		void widgets::time_impl::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::time::time_specs&
			widgets::time_impl::specs() { return specs_; }

		widgets::time::time_specs&
			widgets::time_impl::operator()() { return specs(); }

		void widgets::time_impl::set_time_label_specs(widgets::label::label_specs& hour,
			widgets::label::label_specs& seperator_1,
			widgets::label::label_specs& minute,
			widgets::label::label_specs& seperator_2,
			widgets::label::label_specs& second) {
			hour_label_specs_ = hour;
			seperator_1_specs_ = seperator_1;
			minute_label_specs_ = minute;
			seperator_2_specs_ = seperator_2;
			second_label_specs_ = second;
		}

		void widgets::time_impl::set_time_specs(widgets::rectangle::rectangle_specs& hour,
			widgets::rectangle::rectangle_specs& minute,
			widgets::rectangle::rectangle_specs& second) {
			hour_specs_ = hour;
			minute_specs_ = minute;
			second_specs_ = second;
		}
	}
}
