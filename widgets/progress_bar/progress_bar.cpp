//
// progress_bar.cpp - progress bar widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../progress_bar.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::progress_bar_specs::operator==(const progress_bar_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::progress_bar_specs::operator!=(const progress_bar_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::progress_bar_specs::text() { return text_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::progress_bar_specs::tooltip() { return tooltip_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::progress_bar_specs::rect() { return rect_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::progress_bar_specs::on_resize() { return on_resize_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::progress_bar_specs::cursor() { return cursor_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::progress_bar_specs::font() { return font_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::progress_bar_specs::font_size() { return font_size_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::progress_bar_specs::color_text() { return color_text_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::progress_bar_specs::color_fill() { return color_fill_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::progress_bar_specs::color_hot() { return color_hot_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::progress_bar_specs::color_selected() { return color_selected_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::progress_bar_specs::color_disabled() { return color_disabled_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::progress_bar_specs::border() { return border_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::progress_bar_specs::color_border() { return color_border_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::progress_bar_specs::percentage() { return percentage_; }

		widgets::progress_bar_specs& widgets::progress_bar_specs::percentage(const float& percentage) {
			percentage_ = percentage;
			return *this;
		}

		class widgets::progress_bar_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_progress_bar(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar_border));
			}
			containers::page& page_;
			progress_bar_specs& specs_;
		};

		widgets::progress_bar_builder::progress_bar_builder(containers::page& page) :
			progress_bar_builder(page, "") {}

		widgets::progress_bar_builder::progress_bar_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::progress_bar_builder::~progress_bar_builder() { delete& d_; }

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::specs() {
			return d_.specs_;
		}

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::operator()() {
			return specs();
		}

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_progress_bar(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_progress_bar(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
