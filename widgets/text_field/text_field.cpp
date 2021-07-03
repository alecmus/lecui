//
// text_field.cpp - text_field widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../text_field.h"
#include "../password_field.h"
#include "../../form_impl/form_impl.h"
#include "..\password_field.h"

namespace liblec {
	namespace lecui {
		bool widgets::text_field_specs::operator==(const text_field_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_prompt_ == param.color_prompt_) &&
				(color_caret_ == param.color_caret_);
		}

		bool widgets::text_field_specs::operator!=(const text_field_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::text_field_specs::text() { return text_; }

		widgets::text_field_specs& widgets::text_field_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::text_field_specs::tooltip() { return tooltip_; }

		widgets::text_field_specs& widgets::text_field_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::text_field_specs::rect() { return rect_; }

		widgets::text_field_specs& widgets::text_field_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::text_field_specs::on_resize() { return on_resize_; }

		widgets::text_field_specs& widgets::text_field_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::text_field_specs::cursor() { return cursor_; }

		widgets::text_field_specs& widgets::text_field_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::text_field_specs::font() { return font_; }

		widgets::text_field_specs& widgets::text_field_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::text_field_specs::font_size() { return font_size_; }

		widgets::text_field_specs& widgets::text_field_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::text_field_specs::color_text() { return color_text_; }

		widgets::text_field_specs& widgets::text_field_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::text_field_specs::color_fill() { return color_fill_; }

		widgets::text_field_specs& widgets::text_field_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::text_field_specs::color_hot() { return color_hot_; }

		widgets::text_field_specs& widgets::text_field_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::text_field_specs::color_selected() { return color_selected_; }

		widgets::text_field_specs& widgets::text_field_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::text_field_specs::color_disabled() { return color_disabled_; }

		widgets::text_field_specs& widgets::text_field_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::text_field_specs::border() { return border_; }

		widgets::text_field_specs& widgets::text_field_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::text_field_specs::color_border() { return color_border_; }

		widgets::text_field_specs& widgets::text_field_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::text_field_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::text_field_specs& widgets::text_field_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::text_field_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::text_field_specs& widgets::text_field_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		std::string& widgets::text_field_specs::prompt() { return prompt_; }

		widgets::text_field_specs& widgets::text_field_specs::prompt(const std::string& prompt) {
			prompt_ = prompt;
			return *this;
		}

		lecui::color& widgets::text_field_specs::color_prompt() { return color_prompt_; }

		widgets::text_field_specs& widgets::text_field_specs::color_prompt(const color& color_prompt) {
			color_prompt_ = color_prompt;
			return *this;
		}

		lecui::color& widgets::text_field_specs::color_caret() { return color_caret_; }

		widgets::text_field_specs& widgets::text_field_specs::color_caret(const color& color_caret) {
			color_caret_ = color_caret;
			return *this;
		}

		class widgets::text_field_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_text_field(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_prompt(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_prompt))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_border))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_disabled))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_selected))
					.color_caret(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_caret));
			}
			containers::page& page_;
			text_field_specs& specs_;
		};

		widgets::text_field_builder::text_field_builder(containers::page& page) :
			text_field_builder(page, "") {}

		widgets::text_field_builder::text_field_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::text_field_builder::~text_field_builder() { delete& d_; }

		widgets::text_field_specs&
			widgets::text_field_builder::specs() {
			return d_.specs_;
		}

		widgets::text_field_specs&
			widgets::text_field_builder::operator()() {
			return specs();
		}

		widgets::text_field_specs&
			widgets::text_field_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_text_field(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_text_field(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::password_field_builder::password_field_builder(containers::page& page) :
			password_field_builder(page, "") {}

		widgets::password_field_builder::password_field_builder(containers::page& page, const std::string& alias) :
			text_field_builder(page, alias) {
			d_.specs_.mask = '•';
			d_.specs_.prompt("Enter password here");
		}

		widgets::password_field_builder::~password_field_builder() {}
	}
}
