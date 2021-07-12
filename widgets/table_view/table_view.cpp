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
				(_color_border == param._color_border) &&
				(_color_grid == param._color_grid) &&
				(_color_text_header == param._color_text_header) &&
				(_color_text_header_hot == param._color_text_header_hot) &&
				(_color_fill_header == param._color_fill_header) &&
				(_color_fill_alternate == param._color_fill_alternate) &&
				(_color_row_hot == param._color_row_hot) &&
				(_color_row_selected == param._color_row_selected);
		}

		bool widgets::table_view_specs::operator!=(const table_view_specs& param) {
			return !operator==(param);
		}

		widgets::table_view_specs& widgets::table_view_specs::operator=(const table_view_specs& right) {
			// generic specs
			_text = right._text;
			_tooltip = right._tooltip;
			_rect = right._rect;
			_on_resize = right._on_resize;
			_cursor = right._cursor;
			_font = right._font;
			_font_size = right._font_size;
			_color_text = right._color_text;
			_color_fill = right._color_fill;
			_color_hot = right._color_hot;
			_color_selected = right._color_selected;
			_color_disabled = right._color_disabled;

			// events
			_table_events = right._table_events;

			// widget specific specs
			_border = right._border;
			_grid_line = right._grid_line;
			_color_border = right._color_border;
			_corner_radius_x = right._corner_radius_x;
			_corner_radius_y = right._corner_radius_y;
			_color_text_selected = right._color_text_selected;
			_color_grid = right._color_grid;
			_color_text_header = right._color_text_header;
			_color_text_header_hot = right._color_text_header_hot;
			_color_fill_header = right._color_fill_header;
			_color_fill_alternate = right._color_fill_alternate;
			_color_row_hot = right._color_row_hot;
			_color_row_selected = right._color_row_selected;
			_columns = right._columns;
			_data = right._data;
			_selected = right._selected;
			_user_sort = right._user_sort;
			_fixed_number_column = right._fixed_number_column;
			_fixed_number_column_name = right._fixed_number_column_name;

			// NOT copied (_alias, _p_special_pane_specs)

			return *this;
		}

		widgets::table_view_specs::table_view_specs(const table_view_specs& right) {
			*this = right;
		}

		std::string& widgets::table_view_specs::text() { return _text; }

		widgets::table_view_specs& widgets::table_view_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::table_view_specs::tooltip() { return _tooltip; }

		widgets::table_view_specs& widgets::table_view_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::table_view_specs::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::table_view_specs& widgets::table_view_specs::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::table_view_specs::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::table_view_specs& widgets::table_view_specs::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::table_view_specs::cursor() { return _cursor; }

		widgets::table_view_specs& widgets::table_view_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::table_view_specs::font() { return _font; }

		widgets::table_view_specs& widgets::table_view_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::table_view_specs::font_size() { return _font_size; }

		widgets::table_view_specs& widgets::table_view_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::table_view_specs::color_text() { return _color_text; }

		widgets::table_view_specs& widgets::table_view_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::table_view_specs::color_fill() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill();
			else
				return _color_fill;
		}

		widgets::table_view_specs& widgets::table_view_specs::color_fill(const color& color_fill) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill(color_fill);
			else
				_color_fill = color_fill;

			return *this;
		}

		color& widgets::table_view_specs::color_hot() { return _color_hot; }

		widgets::table_view_specs& widgets::table_view_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::table_view_specs::color_selected() { return _color_selected; }

		widgets::table_view_specs& widgets::table_view_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::table_view_specs::color_disabled() { return _color_disabled; }

		widgets::table_view_specs& widgets::table_view_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::table_view_specs::border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border();
			else
				return _border;
		}

		widgets::table_view_specs& widgets::table_view_specs::border(const float& border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border(border);
			else
				_border = border;

			return *this;
		}

		float& widgets::table_view_specs::grid_line() { return _grid_line; }

		widgets::table_view_specs& widgets::table_view_specs::grid_line(const float& grid_line) {
			_grid_line = grid_line;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border();
			else
				return _color_border;
		}

		widgets::table_view_specs& widgets::table_view_specs::color_border(const color& color_border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border(color_border);
			else
				_color_border = color_border;

			return *this;
		}

		float& widgets::table_view_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::table_view_specs& widgets::table_view_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::table_view_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::table_view_specs& widgets::table_view_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_text_selected() { return _color_text_selected; }

		widgets::table_view_specs& widgets::table_view_specs::color_text_selected(const color& color_text_selected) {
			_color_text_selected = color_text_selected;
			return *this;
		}

		color& widgets::table_view_specs::color_grid() { return _color_grid; }

		widgets::table_view_specs& widgets::table_view_specs::color_grid(const color& color_grid) {
			_color_grid = color_grid;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_text_header() { return _color_text_header; }

		widgets::table_view_specs& widgets::table_view_specs::color_text_header(const color& color_text_header) {
			_color_text_header = color_text_header;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_text_header_hot() { return _color_text_header_hot; }

		widgets::table_view_specs& widgets::table_view_specs::color_text_header_hot(const color& color_text_header_hot) {
			_color_text_header_hot = color_text_header_hot;
			return *this;
		}

		color& widgets::table_view_specs::color_fill_header() { return _color_fill_header; }

		widgets::table_view_specs& widgets::table_view_specs::color_fill_header(const color& color_fill_header) {
			_color_fill_header = color_fill_header;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_fill_alternate() { return _color_fill_alternate; }

		widgets::table_view_specs& widgets::table_view_specs::color_fill_alternate(const color& color_fill_alternate) {
			_color_fill_alternate = color_fill_alternate;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_row_hot() { return _color_row_hot; }

		widgets::table_view_specs& widgets::table_view_specs::color_row_hot(const color& color_row_hot) {
			_color_row_hot = color_row_hot;
			return *this;
		}

		lecui::color& widgets::table_view_specs::color_row_selected() { return _color_row_selected; }

		widgets::table_view_specs& widgets::table_view_specs::color_row_selected(const color& color_row_selected) {
			_color_row_selected = color_row_selected;
			return *this;
		}

		std::vector<lecui::table_column>& widgets::table_view_specs::columns() { return _columns; }

		widgets::table_view_specs& widgets::table_view_specs::columns(const std::vector<table_column>& columns) {
			_columns = columns;
			return *this;
		}

		std::vector<table_row>& widgets::table_view_specs::data() { return _data; }

		widgets::table_view_specs& widgets::table_view_specs::data(const std::vector<table_row>& data) {
			_data = data;
			return *this;
		}

		std::vector<long>& widgets::table_view_specs::selected() { return _selected; }

		widgets::table_view_specs& widgets::table_view_specs::selected(const std::vector<long>& selected) {
			_selected = selected;
			return *this;
		}

		bool& widgets::table_view_specs::user_sort() { return _user_sort; }

		widgets::table_view_specs& widgets::table_view_specs::user_sort(const bool& user_sort) {
			_user_sort = user_sort;
			return *this;
		}

		bool& widgets::table_view_specs::fixed_number_column() { return _fixed_number_column; }

		widgets::table_view_specs& widgets::table_view_specs::fixed_number_column(const bool& fixed_number_column) {
			_fixed_number_column = fixed_number_column;
			return *this;
		}

		std::string& widgets::table_view_specs::fixed_number_column_name() { return _fixed_number_column_name; }

		widgets::table_view_specs& widgets::table_view_specs::fixed_number_column_name(const std::string& fixed_number_column_name) {
			_fixed_number_column_name = fixed_number_column_name;
			return *this;
		}

		class widgets::table_view_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_table_view(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_text_selected(defaults::color(_page._d_page._fm._d._theme, item::table_view_text_selected))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::table_view))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::table_view_border))
					.color_fill_alternate(defaults::color(_page._d_page._fm._d._theme, item::table_view_alternate))
					.color_row_hot(defaults::color(_page._d_page._fm._d._theme, item::table_view_row_hover))
					.color_row_selected(defaults::color(_page._d_page._fm._d._theme, item::table_view_row_selected))
					.color_text_header(defaults::color(_page._d_page._fm._d._theme, item::table_view_text_header))
					.color_text_header_hot(defaults::color(_page._d_page._fm._d._theme, item::table_view_text_header_hot))
					.color_fill_header(defaults::color(_page._d_page._fm._d._theme, item::table_view_header))
					.color_grid(defaults::color(_page._d_page._fm._d._theme, item::table_view_grid));
			}
			containers::page& _page;
			table_view_specs& _specs;
		};

		widgets::table_view_builder::table_view_builder(containers::page& page) :
			table_view_builder(page, "") {}

		widgets::table_view_builder::table_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::table_view_builder::~table_view_builder() { delete& _d; }

		widgets::table_view_specs&
			widgets::table_view_builder::specs() {
			return _d._specs;
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
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
