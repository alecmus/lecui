//
// line.cpp - line widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../line.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::line_specs::operator==(const line_specs& param) {
			return
				// generic specs
				specs::operator==(param);
		}

		bool widgets::line_specs::operator!=(const line_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::line_specs::text() { return text_; }

		widgets::line_specs& widgets::line_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::line_specs::tooltip() { return tooltip_; }

		widgets::line_specs& widgets::line_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::line_specs::rect() { return rect_; }

		widgets::line_specs& widgets::line_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::line_specs::on_resize() { return on_resize_; }

		widgets::line_specs& widgets::line_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::line_specs::cursor() { return cursor_; }

		widgets::line_specs& widgets::line_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::line_specs::font() { return font_; }

		widgets::line_specs& widgets::line_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::line_specs::font_size() { return font_size_; }

		widgets::line_specs& widgets::line_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::line_specs::color_text() { return color_text_; }

		widgets::line_specs& widgets::line_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::line_specs::color_fill() { return color_fill_; }

		widgets::line_specs& widgets::line_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::line_specs::color_hot() { return color_hot_; }

		widgets::line_specs& widgets::line_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::line_specs::color_selected() { return color_selected_; }

		widgets::line_specs& widgets::line_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::line_specs::color_disabled() { return color_disabled_; }

		widgets::line_specs& widgets::line_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::line_specs::thickness() { return thickness_; }

		widgets::line_specs& widgets::line_specs::thickness(const float& thickness) {
			thickness_ = thickness;
			return *this;
		}

		std::vector<point>& widgets::line_specs::points() { return points_; }

		widgets::line_specs& widgets::line_specs::points(const std::vector<point>& points) {
			points_ = points;
			return *this;
		}

		class widgets::line_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_line(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::line))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::line_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::line_selected));

			}
			containers::page& page_;
			line_specs& specs_;
		};

		widgets::line_builder::line_builder(containers::page& page) :
			line_builder(page, "") {}

		widgets::line_builder::line_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::line_builder::~line_builder() { delete& d_; }

		widgets::line_specs&
			widgets::line_builder::specs() {
			return d_.specs_;
		}

		widgets::line_specs&
			widgets::line_builder::operator()() {
			return specs();
		}

		widgets::line_specs&
			widgets::line_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
