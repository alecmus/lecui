//
// table_view.cpp - table view widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../table_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::table_view::operator==(const table_view& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_grid == param._color_grid) &&
				(_color_text_header == param._color_text_header) &&
				(_color_text_header_hot == param._color_text_header_hot) &&
				(_color_fill_header == param._color_fill_header) &&
				(_color_fill_alternate == param._color_fill_alternate) &&
				(_color_row_hot == param._color_row_hot) &&
				(_color_row_selected == param._color_row_selected);
		}

		bool widgets::table_view::operator!=(const table_view& param) {
			return !operator==(param);
		}

		widgets::table_view& widgets::table_view::operator=(const table_view& right) {
			// generic widget
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

			// widget specific widget
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

		widgets::table_view::table_view(const table_view& right) {
			*this = right;
		}

		std::string& widgets::table_view::text() { return _text; }

		widgets::table_view& widgets::table_view::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::table_view::tooltip() { return _tooltip; }

		widgets::table_view& widgets::table_view::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::table_view::rect() {
			if (_p_special_pane)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->rect();
			else
				return _rect;
		}

		widgets::table_view& widgets::table_view::rect(const lecui::rect& rect) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		resize_params& widgets::table_view::on_resize() {
			if (_p_special_pane)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->on_resize();
			else
				return _on_resize;
		}

		widgets::table_view& widgets::table_view::on_resize(const resize_params& on_resize) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::widget::cursor_type& widgets::table_view::cursor() { return _cursor; }

		widgets::table_view& widgets::table_view::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::table_view::font() { return _font; }

		widgets::table_view& widgets::table_view::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::table_view::font_size() { return _font_size; }

		widgets::table_view& widgets::table_view::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::table_view::color_text() { return _color_text; }

		widgets::table_view& widgets::table_view::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::table_view::color_fill() {
			if (_p_special_pane)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->color_fill();
			else
				return _color_fill;
		}

		widgets::table_view& widgets::table_view::color_fill(const color& color_fill) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->color_fill(color_fill);
			else
				_color_fill = color_fill;

			return *this;
		}

		color& widgets::table_view::color_hot() { return _color_hot; }

		widgets::table_view& widgets::table_view::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::table_view::color_selected() { return _color_selected; }

		widgets::table_view& widgets::table_view::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::table_view::color_disabled() { return _color_disabled; }

		widgets::table_view& widgets::table_view::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::table_view::border() {
			if (_p_special_pane)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->border();
			else
				return _border;
		}

		widgets::table_view& widgets::table_view::border(const float& border) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->border(border);
			else
				_border = border;

			return *this;
		}

		float& widgets::table_view::grid_line() { return _grid_line; }

		widgets::table_view& widgets::table_view::grid_line(const float& grid_line) {
			_grid_line = grid_line;
			return *this;
		}

		lecui::color& widgets::table_view::color_border() {
			if (_p_special_pane)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->color_border();
			else
				return _color_border;
		}

		widgets::table_view& widgets::table_view::color_border(const color& color_border) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->color_border(color_border);
			else
				_color_border = color_border;

			return *this;
		}

		float& widgets::table_view::corner_radius_x() { return _corner_radius_x; }

		widgets::table_view& widgets::table_view::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::table_view::corner_radius_y() { return _corner_radius_y; }

		widgets::table_view& widgets::table_view::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		lecui::color& widgets::table_view::color_text_selected() { return _color_text_selected; }

		widgets::table_view& widgets::table_view::color_text_selected(const color& color_text_selected) {
			_color_text_selected = color_text_selected;
			return *this;
		}

		color& widgets::table_view::color_grid() { return _color_grid; }

		widgets::table_view& widgets::table_view::color_grid(const color& color_grid) {
			_color_grid = color_grid;
			return *this;
		}

		lecui::color& widgets::table_view::color_text_header() { return _color_text_header; }

		widgets::table_view& widgets::table_view::color_text_header(const color& color_text_header) {
			_color_text_header = color_text_header;
			return *this;
		}

		lecui::color& widgets::table_view::color_text_header_hot() { return _color_text_header_hot; }

		widgets::table_view& widgets::table_view::color_text_header_hot(const color& color_text_header_hot) {
			_color_text_header_hot = color_text_header_hot;
			return *this;
		}

		color& widgets::table_view::color_fill_header() { return _color_fill_header; }

		widgets::table_view& widgets::table_view::color_fill_header(const color& color_fill_header) {
			_color_fill_header = color_fill_header;
			return *this;
		}

		lecui::color& widgets::table_view::color_fill_alternate() { return _color_fill_alternate; }

		widgets::table_view& widgets::table_view::color_fill_alternate(const color& color_fill_alternate) {
			_color_fill_alternate = color_fill_alternate;
			return *this;
		}

		lecui::color& widgets::table_view::color_row_hot() { return _color_row_hot; }

		widgets::table_view& widgets::table_view::color_row_hot(const color& color_row_hot) {
			_color_row_hot = color_row_hot;
			return *this;
		}

		lecui::color& widgets::table_view::color_row_selected() { return _color_row_selected; }

		widgets::table_view& widgets::table_view::color_row_selected(const color& color_row_selected) {
			_color_row_selected = color_row_selected;
			return *this;
		}

		std::vector<lecui::table_column>& widgets::table_view::columns() { return _columns; }

		widgets::table_view& widgets::table_view::columns(const std::vector<table_column>& columns) {
			_columns = columns;
			return *this;
		}

		std::vector<table_row>& widgets::table_view::data() { return _data; }

		widgets::table_view& widgets::table_view::data(const std::vector<table_row>& data) {
			_data = data;
			return *this;
		}

		std::vector<long>& widgets::table_view::selected() { return _selected; }

		widgets::table_view& widgets::table_view::selected(const std::vector<long>& selected) {
			_selected = selected;
			return *this;
		}

		bool& widgets::table_view::user_sort() { return _user_sort; }

		widgets::table_view& widgets::table_view::user_sort(const bool& user_sort) {
			_user_sort = user_sort;
			return *this;
		}

		bool& widgets::table_view::fixed_number_column() { return _fixed_number_column; }

		widgets::table_view& widgets::table_view::fixed_number_column(const bool& fixed_number_column) {
			_fixed_number_column = fixed_number_column;
			return *this;
		}

		std::string& widgets::table_view::fixed_number_column_name() { return _fixed_number_column_name; }

		widgets::table_view& widgets::table_view::fixed_number_column_name(const std::string& fixed_number_column_name) {
			_fixed_number_column_name = fixed_number_column_name;
			return *this;
		}

		namespace widgets {
			/// <summary>Table view widget builder.</summary>
			class table_view_builder {
			public:
				/// <summary>Table view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				table_view_builder(containers::page& page);

				/// <summary>Table view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "debtors".</param>
				table_view_builder(containers::page& page, const std::string& alias);
				~table_view_builder();

				/// <summary>Get the table view specifications.</summary>
				/// <returns>A reference to the table view specifications.</returns>
				[[nodiscard]]
				table_view& specs();

				/// <summary>Get the table view specifications.</summary>
				/// <returns>A reference to the table view specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				table_view& operator()();

				/// <summary>Get the specifications of a table view.</summary>
				/// <param name="fm">The form containing the table view.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/debtors".</param>
				/// <returns>A reference to the table view specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static table_view& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				table_view_builder() = delete;
				table_view_builder(const table_view_builder&) = delete;
				table_view_builder& operator=(const table_view_builder&) = delete;
			};
		}

		class widgets::table_view_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_table_view(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, element::label))
					.color_text_selected(defaults::color(_page._d_page._fm._d._theme, element::table_view_text_selected))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, element::table_view))
					.color_border(defaults::color(_page._d_page._fm._d._theme, element::table_view_border))
					.color_fill_alternate(defaults::color(_page._d_page._fm._d._theme, element::table_view_alternate))
					.color_row_hot(defaults::color(_page._d_page._fm._d._theme, element::table_view_row_hover))
					.color_row_selected(defaults::color(_page._d_page._fm._d._theme, element::table_view_row_selected))
					.color_text_header(defaults::color(_page._d_page._fm._d._theme, element::table_view_text_header))
					.color_text_header_hot(defaults::color(_page._d_page._fm._d._theme, element::table_view_text_header_hot))
					.color_fill_header(defaults::color(_page._d_page._fm._d._theme, element::table_view_header))
					.color_grid(defaults::color(_page._d_page._fm._d._theme, element::table_view_grid));
			}
			containers::page& _page;
			table_view& _specs;
		};

		widgets::table_view_builder::table_view_builder(containers::page& page) :
			table_view_builder(page, "") {}

		widgets::table_view_builder::table_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::table_view_builder::~table_view_builder() { delete& _d; }

		widgets::table_view&
			widgets::table_view_builder::specs() {
			return _d._specs;
		}

		widgets::table_view&
			widgets::table_view_builder::operator()() {
			return specs();
		}

		widgets::table_view&
			widgets::table_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_table_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_table_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::table_view& liblec::lecui::widgets::table_view::add(containers::page& page, const std::string& alias) {
			return table_view_builder(page, alias).specs();
		}

		widgets::table_view& liblec::lecui::widgets::table_view::get(form& fm, const std::string& path) {
			return table_view_builder::specs(fm, path);
		}

		void widgets::table_view::scroll_vertically(const float& amount) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->scroll_vertically(amount);
		}

		void widgets::table_view::scroll_horizontally(const float& amount) {
			if (_p_special_pane)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane*>(_p_special_pane)->scroll_horizontally(amount);
		}
	}
}
