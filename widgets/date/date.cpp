//
// date.cpp - date widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../date.h"
#include "../../form_impl/form_impl.h"
#include "../../utilities/date_time.h"

namespace liblec {
	namespace lecui {
		bool widgets::date_specs::operator==(const date_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::date_specs::operator!=(const date_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::date_specs::text() { return text_; }

		widgets::date_specs& widgets::date_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::date_specs::tooltip() { return tooltip_; }

		widgets::date_specs& widgets::date_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::date_specs::rect() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect();
			else
				return rect_;
		}

		widgets::date_specs& widgets::date_specs::rect(const lecui::rect& rect) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect(rect);
			else
				rect_ = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::date_specs::on_resize() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize();
			else
				return on_resize_;
		}

		widgets::date_specs& widgets::date_specs::on_resize(const resize_params& on_resize) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize(on_resize);
			else
				on_resize_ = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::date_specs::cursor() { return cursor_; }

		widgets::date_specs& widgets::date_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::date_specs::font() { return font_; }

		widgets::date_specs& widgets::date_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::date_specs::font_size() { return font_size_; }

		widgets::date_specs& widgets::date_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::date_specs::color_text() { return color_text_; }

		widgets::date_specs& widgets::date_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::date_specs::color_fill() { return color_fill_; }

		widgets::date_specs& widgets::date_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::date_specs::color_hot() { return color_hot_; }

		widgets::date_specs& widgets::date_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::date_specs::color_selected() { return color_selected_; }

		widgets::date_specs& widgets::date_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::date_specs::color_disabled() { return color_disabled_; }

		widgets::date_specs& widgets::date_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::date_specs::border() { return border_; }

		widgets::date_specs& widgets::date_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::date_specs::color_border() { return color_border_; }

		widgets::date_specs& widgets::date_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::date_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::date_specs& widgets::date_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::date_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::date_specs& widgets::date_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		date& widgets::date_specs::date_value() { return date_value_; }

		widgets::date_specs& widgets::date_specs::date_value(const date& date_value) {
			date_value_ = date_value;
			return *this;
		}

		class widgets::date_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_date(alias)) {
				specs_
					.date_value(date_time::today())
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_border))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_disabled))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_selected));
			}
			containers::page& page_;
			date_specs& specs_;
		};

		widgets::date_builder::date_builder(containers::page& page) :
			date_builder(page, "") {}

		widgets::date_builder::date_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::date_builder::~date_builder() { delete& d_; }

		widgets::date_specs&
			widgets::date_builder::specs() {
			return d_.specs_;
		}

		widgets::date_specs&
			widgets::date_builder::operator()() {
			return specs();
		}

		widgets::date_specs&
			widgets::date_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_date(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_date(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
