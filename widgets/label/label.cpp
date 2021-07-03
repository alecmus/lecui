//
// label.cpp - label widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../label.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::label_specs::operator==(const label_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(text_ == param.text_) &&
				(color_hot_pressed_ == param.color_hot_pressed_) &&
				(multiline_ == param.multiline_) &&
				(center_h_ == param.center_h_) &&
				(center_v_ == param.center_v_);
		}

		bool widgets::label_specs::operator!=(const label_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::label_specs::text() { return text_; }

		widgets::label_specs& widgets::label_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::label_specs::tooltip() { return tooltip_; }

		widgets::label_specs& widgets::label_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::label_specs::rect() { return rect_; }

		widgets::label_specs& widgets::label_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::label_specs::on_resize() { return on_resize_; }

		widgets::label_specs& widgets::label_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::label_specs::cursor() { return cursor_; }

		widgets::label_specs& widgets::label_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::label_specs::font() { return font_; }

		widgets::label_specs& widgets::label_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::label_specs::font_size() { return font_size_; }

		widgets::label_specs& widgets::label_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::label_specs::color_text() { return color_text_; }

		widgets::label_specs& widgets::label_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::label_specs::color_fill() { return color_fill_; }

		widgets::label_specs& widgets::label_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::label_specs::color_hot() { return color_hot_; }

		widgets::label_specs& widgets::label_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::label_specs::color_selected() { return color_selected_; }

		widgets::label_specs& widgets::label_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::label_specs::color_disabled() { return color_disabled_; }

		widgets::label_specs& widgets::label_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		lecui::color& widgets::label_specs::color_hot_pressed() { return color_hot_pressed_; }

		widgets::label_specs& widgets::label_specs::color_hot_pressed(const color& color_hot_pressed) {
			color_hot_pressed_ = color_hot_pressed;
			return *this;
		}

		bool& widgets::label_specs::multiline() { return multiline_; }

		widgets::label_specs& widgets::label_specs::multiline(const bool& multiline) {
			multiline_ = multiline;
			return *this;
		}

		bool& widgets::label_specs::center_h() { return center_h_; }

		widgets::label_specs& widgets::label_specs::center_h(const bool& center_h) {
			center_h_ = center_h;
			return *this;
		}

		bool& widgets::label_specs::center_v() { return center_v_; }

		widgets::label_specs& widgets::label_specs::center_v(const bool& center_v) {
			center_v_ = center_v;
			return *this;
		}

		class widgets::label_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_label(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_selected))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_hover))
					.color_hot_pressed(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_pressed))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_disabled));
			}
			containers::page& page_;
			label_specs& specs_;
		};

		widgets::label_builder::label_builder(containers::page& page) :
			label_builder(page, "") {}

		widgets::label_builder::label_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::label_builder::~label_builder() { delete& d_; }

		widgets::label_specs&
			widgets::label_builder::specs() {
			return d_.specs_;
		}

		widgets::label_specs&
			widgets::label_builder::operator()() {
			return specs();
		}

		widgets::label_specs&
			widgets::label_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
