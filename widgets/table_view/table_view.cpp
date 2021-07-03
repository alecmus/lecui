//
// table_view.cpp - table view widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../table_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::table_view_specs::operator==(const table_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_grid_ == param.color_grid_) &&
				(color_text_header_ == param.color_text_header_) &&
				(color_fill_header_ == param.color_fill_header_) &&
				(color_fill_alternate_ == param.color_fill_alternate_) &&
				(color_row_hot_ == param.color_row_hot_) &&
				(color_row_selected_ == param.color_row_selected_);
		}

		bool widgets::table_view_specs::operator!=(const table_view_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::table_view_specs::text() { return text_; }

		widgets::table_view_specs& widgets::table_view_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::table_view_specs::tooltip() { return tooltip_; }

		widgets::table_view_specs& widgets::table_view_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::table_view_specs::rect() { return rect_; }

		widgets::table_view_specs& widgets::table_view_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::table_view_specs::on_resize() { return on_resize_; }

		widgets::table_view_specs& widgets::table_view_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::table_view_specs::cursor() { return cursor_; }

		widgets::table_view_specs& widgets::table_view_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::table_view_specs::font() { return font_; }

		widgets::table_view_specs& widgets::table_view_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::table_view_specs::font_size() { return font_size_; }

		widgets::table_view_specs& widgets::table_view_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::table_view_specs::color_text() { return color_text_; }

		widgets::table_view_specs& widgets::table_view_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::table_view_specs::color_fill() { return color_fill_; }

		widgets::table_view_specs& widgets::table_view_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::table_view_specs::color_hot() { return color_hot_; }

		widgets::table_view_specs& widgets::table_view_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::table_view_specs::color_selected() { return color_selected_; }

		widgets::table_view_specs& widgets::table_view_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::table_view_specs::color_disabled() { return color_disabled_; }

		widgets::table_view_specs& widgets::table_view_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::table_view_specs::border() { return border_; }

		widgets::table_view_specs& widgets::table_view_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_border() { return color_border_; }

		widgets::table_view_specs& widgets::table_view_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::table_view_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::table_view_specs& widgets::table_view_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::table_view_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::table_view_specs& widgets::table_view_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_text_selected() { return color_text_selected_; }

		widgets::table_view_specs& widgets::table_view_specs::color_text_selected(const color& color_text_selected) {
			color_text_selected_ = color_text_selected;
			return *this;
		}

		color& widgets::table_view_specs::color_grid() { return color_grid_; }

		widgets::table_view_specs& widgets::table_view_specs::color_grid(const color& color_grid) {
			color_grid_ = color_grid;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_text_header() { return color_text_header_; }

		widgets::table_view_specs& widgets::table_view_specs::color_text_header(const color& color_text_header) {
			color_text_header_ = color_text_header;
			return *this;
		}

		color& widgets::table_view_specs::color_fill_header() { return color_fill_header_; }

		widgets::table_view_specs& widgets::table_view_specs::color_fill_header(const color& color_fill_header) {
			color_fill_header_ = color_fill_header;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_fill_alternate() { return color_fill_alternate_; }

		widgets::table_view_specs& widgets::table_view_specs::color_fill_alternate(const color& color_fill_alternate) {
			color_fill_alternate_ = color_fill_alternate;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_row_hot() { return color_row_hot_; }

		widgets::table_view_specs& widgets::table_view_specs::color_row_hot(const color& color_row_hot) {
			color_row_hot_ = color_row_hot;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_row_selected() { return color_row_selected_; }

		widgets::table_view_specs& widgets::table_view_specs::color_row_selected(const color& color_row_selected) {
			color_row_selected_ = color_row_selected;
			return *this;
		}

		std::vector<lecui::table_column>& widgets::table_view_specs::columns() { return columns_; }

		widgets::table_view_specs& widgets::table_view_specs::columns(const std::vector<table_column>& columns) {
			columns_ = columns;
			return *this;
		}

		std::vector<std::map<std::string, std::string>>& widgets::table_view_specs::data() { return data_; }

		widgets::table_view_specs& widgets::table_view_specs::data(const std::vector<std::map<std::string, std::string>>& data) {
			data_ = data;
			return *this;
		}

		std::vector<long>& widgets::table_view_specs::selected() { return selected_; }

		widgets::table_view_specs& widgets::table_view_specs::selected(const std::vector<long>& selected) {
			selected_ = selected;
			return *this;
		}

		class widgets::table_view_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_table_view(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_text_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_text_selected))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_border))
					.color_fill_alternate(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_alternate))
					.color_row_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_row_hover))
					.color_row_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_row_selected))
					.color_text_header(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_text_header))
					.color_fill_header(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_header))
					.color_grid(defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_grid));
			}
			containers::page& page_;
			table_view_specs& specs_;
		};

		widgets::table_view_builder::table_view_builder(containers::page& page) :
			table_view_builder(page, "") {}

		widgets::table_view_builder::table_view_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::table_view_builder::~table_view_builder() { delete& d_; }

		widgets::table_view_specs&
			widgets::table_view_builder::specs() {
			return d_.specs_;
		}

		widgets::table_view_specs&
			widgets::table_view_builder::operator()() {
			return specs();
		}

		widgets::table_view_specs&
			widgets::table_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
