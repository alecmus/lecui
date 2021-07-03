//
// button.cpp - button widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../button.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::button_specs::operator==(const button_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::button_specs::operator!=(const button_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::button_specs::text() { return text_; }

		widgets::button_specs& widgets::button_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::button_specs::tooltip() { return tooltip_; }

		widgets::button_specs& widgets::button_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::button_specs::rect() { return rect_; }

		widgets::button_specs& widgets::button_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::button_specs::on_resize() { return on_resize_; }

		widgets::button_specs& widgets::button_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::button_specs::cursor() { return cursor_; }

		widgets::button_specs& widgets::button_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::button_specs::font() { return font_; }

		widgets::button_specs& widgets::button_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::button_specs::font_size() { return font_size_; }

		widgets::button_specs& widgets::button_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::button_specs::color_text() { return color_text_; }

		widgets::button_specs& widgets::button_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::button_specs::color_fill() { return color_fill_; }

		widgets::button_specs& widgets::button_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::button_specs::color_hot() { return color_hot_; }

		widgets::button_specs& widgets::button_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::button_specs::color_selected() { return color_selected_; }

		widgets::button_specs& widgets::button_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::button_specs::color_disabled() { return color_disabled_; }

		widgets::button_specs& widgets::button_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::button_specs::border() { return border_; }

		widgets::button_specs& widgets::button_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::button_specs::color_border() { return color_border_; }

		widgets::button_specs& widgets::button_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::button_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::button_specs& widgets::button_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::button_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::button_specs& widgets::button_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		class widgets::button_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_button(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::button))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::button_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::button_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::button_selected));
			}
			containers::page& page_;
			button_specs& specs_;
		};

		widgets::button_builder::button_builder(containers::page& page) :
			button_builder(page, "") {}

		widgets::button_builder::button_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::button_builder::~button_builder() { delete& d_; }

		widgets::button_specs&
			widgets::button_builder::specs() {
			return d_.specs_;
		}

		widgets::button_specs&
			widgets::button_builder::operator()() {
			return specs();
		}

		widgets::button_specs&
			widgets::button_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_button(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_button(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
