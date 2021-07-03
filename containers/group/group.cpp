//
// group.cpp - group container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../group.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool containers::group_specs::operator==(const group_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool containers::group_specs::operator!=(const group_specs& param) {
			return !operator==(param);
		}

		std::string& containers::group_specs::text() { return text_; }

		containers::group_specs& containers::group_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& containers::group_specs::tooltip() { return tooltip_; }

		containers::group_specs& containers::group_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& containers::group_specs::rect() { return rect_; }

		containers::group_specs& containers::group_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& containers::group_specs::on_resize() { return on_resize_; }

		containers::group_specs& containers::group_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::group_specs::cursor() { return cursor_; }

		containers::group_specs& containers::group_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& containers::group_specs::font() { return font_; }

		containers::group_specs& containers::group_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& containers::group_specs::font_size() { return font_size_; }

		containers::group_specs& containers::group_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& containers::group_specs::color_text() { return color_text_; }

		containers::group_specs& containers::group_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& containers::group_specs::color_fill() { return color_fill_; }

		containers::group_specs& containers::group_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& containers::group_specs::color_hot() { return color_hot_; }

		containers::group_specs& containers::group_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& containers::group_specs::color_selected() { return color_selected_; }

		containers::group_specs& containers::group_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& containers::group_specs::color_disabled() { return color_disabled_; }

		containers::group_specs& containers::group_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& containers::group_specs::border() { return border_; }

		containers::group_specs& containers::group_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& containers::group_specs::color_border() { return color_border_; }

		containers::group_specs& containers::group_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& containers::group_specs::corner_radius_x() { return corner_radius_x_; }

		containers::group_specs& containers::group_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& containers::group_specs::corner_radius_y() { return corner_radius_y_; }

		containers::group_specs& containers::group_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		float& containers::group_specs::margin() { return margin_; }

		containers::group_specs& containers::group_specs::margin(const float& margin) {
			margin_ = margin;
			return *this;
		}

		std::vector<std::string>& containers::group_specs::widgets() { return widgets_; }

		containers::group_specs& containers::group_specs::widgets(const std::vector<std::string>& widgets) {
			widgets_ = widgets;
			return *this;
		}

		class containers::group_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_group(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::group))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::group_border));
			}
			containers::page& page_;
			group_specs& specs_;
		};

		containers::group_builder::group_builder(containers::page& page) :
			group_builder(page, "") {}

		containers::group_builder::group_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		containers::group_builder::~group_builder() { delete& d_; }

		containers::group_specs&
			containers::group_builder::specs() {
			return d_.specs_;
		}

		containers::group_specs&
			containers::group_builder::operator()() {
			return specs();
		}

		containers::group_specs&
			containers::group_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
