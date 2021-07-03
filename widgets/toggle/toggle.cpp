//
// toggle.cpp - toggle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../toggle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::toggle_specs::operator==(const toggle_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_on_ == param.color_on_) &&
				(color_off_ == param.color_off_);
		}

		bool widgets::toggle_specs::operator!=(const toggle_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::toggle_specs::text() { return text_; }

		widgets::toggle_specs& widgets::toggle_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::toggle_specs::tooltip() { return tooltip_; }

		widgets::toggle_specs& widgets::toggle_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::toggle_specs::rect() { return rect_; }

		widgets::toggle_specs& widgets::toggle_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::toggle_specs::on_resize() { return on_resize_; }

		widgets::toggle_specs& widgets::toggle_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::toggle_specs::cursor() { return cursor_; }

		widgets::toggle_specs& widgets::toggle_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::toggle_specs::font() { return font_; }

		widgets::toggle_specs& widgets::toggle_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::toggle_specs::font_size() { return font_size_; }

		widgets::toggle_specs& widgets::toggle_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::toggle_specs::color_text() { return color_text_; }

		widgets::toggle_specs& widgets::toggle_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::toggle_specs::color_fill() { return color_fill_; }

		widgets::toggle_specs& widgets::toggle_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::toggle_specs::color_hot() { return color_hot_; }

		widgets::toggle_specs& widgets::toggle_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::toggle_specs::color_selected() { return color_selected_; }

		widgets::toggle_specs& widgets::toggle_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::toggle_specs::color_disabled() { return color_disabled_; }

		widgets::toggle_specs& widgets::toggle_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		std::string& widgets::toggle_specs::text_off() { return text_off_; }

		widgets::toggle_specs& widgets::toggle_specs::text_off(const std::string& text_off) {
			text_off_ = text_off;
			return *this;
		}

		color& widgets::toggle_specs::color_on() { return color_on_; }

		widgets::toggle_specs& widgets::toggle_specs::color_on(const color& color_on) {
			color_on_ = color_on;
			return *this;
		}

		lecui::color& widgets::toggle_specs::color_off() { return color_off_; }

		widgets::toggle_specs& widgets::toggle_specs::color_off(const color& color_off) {
			color_off_ = color_off;
			return *this;
		}

		bool& widgets::toggle_specs::on() { return on_; }

		widgets::toggle_specs& widgets::toggle_specs::on(const bool& on) {
			on_ = on;
			return *this;
		}

		class widgets::toggle_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_toggle(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle))
					.color_on(defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_on))
					.color_off(defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_off))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_selected));
			}
			containers::page& page_;
			toggle_specs& specs_;
		};

		widgets::toggle_builder::toggle_builder(containers::page& page) :
			toggle_builder(page, "") {}

		widgets::toggle_builder::toggle_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::toggle_builder::~toggle_builder() { delete& d_; }

		widgets::toggle_specs&
			widgets::toggle_builder::specs() {
			return d_.specs_;
		}

		widgets::toggle_specs&
			widgets::toggle_builder::operator()() {
			return specs();
		}

		widgets::toggle_specs&
			widgets::toggle_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
