//
// table_view.h - table view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Table view widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api table_view : public widget {
				float _border = .5f;
				float _grid_line = .5f;
				color _color_border;
				float _corner_radius_x = 2.f;
				float _corner_radius_y = 2.f;
				color _color_text_selected;
				color _color_grid;
				color _color_text_header;
				color _color_text_header_hot;
				color _color_fill_header;
				color _color_fill_alternate;
				color _color_row_hot;
				color _color_row_selected;
				std::vector<table_column> _columns;
				std::vector<table_row> _data;
				std::vector<long> _selected;
				bool _user_sort = false;
				bool _fixed_number_column = false;
				std::string _fixed_number_column_name = "#";
				void* _p_special_pane = nullptr;

			public:
				/// <summary>Constructor.</summary>
				table_view() {
					_rect.size({ 200.f, 200.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct table_view_events : basic_events {
					/// <summary>Selection event. Happens when the selection changes.</summary>
					/// <remarks>The parameter contains the selected rows.</remarks>
					std::function<void(
						const std::vector<table_row>& rows)>
						selection = nullptr;

					/// <summary>Context menu event. Happens when the table view is right clicked.</summary>
					/// <remarks>The parameter contained the selected rows.</remarks>
					std::function<void(
						const std::vector<table_row>& rows)>
						context_menu = nullptr;
				};

				/// <summary>Get or set table view events.</summary>
				/// <returns>A reference to the table view events.</returns>
				table_view_events& events() {
					return _table_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const table_view& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const table_view& param);

				/// <summary>Copy table view specs. Only essential properties are copied.</summary>
				/// <param name="param">The table view specs to copy.</param>
				/// <returns>Returns a reference to the copy.</returns>
				table_view& operator=(const table_view& param);

				/// <summary>Copy constructor.</summary>
				/// <param name="param">The table view specs to copy.</param>
				table_view(const table_view& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				table_view& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				table_view& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& border(const float& border);

				/// <summary>Get or set the thickness of the grid-lines.</summary>
				/// <returns>A reference to the line thickness, in pixels.</returns>
				float& grid_line();

				/// <summary>Set the thickness of the grid-line.</summary>
				/// <param name="grid_line">The line thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& grid_line(const float& grid_line);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the color of the text in a selected row.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_selected();

				/// <summary>Set the color of the text in a selected row.</summary>
				/// <param name="color_text_selected">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_text_selected(const color& color_text_selected);

				/// <summary>Get or set the color of the grid lines.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_grid();

				/// <summary>Set the color of the grid lines.</summary>
				/// <param name="color_grid">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_grid(const color& color_grid);

				/// <summary>Get or set the color of the header text.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_header();

				/// <summary>Set the color of the header text.</summary>
				/// <param name="color_text_header">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_text_header(const color& color_text_header);

				/// <summary>Get or set the color of the header text when the mouse is over it (if user sorting is allowed).</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_header_hot();

				/// <summary>Set the color of the header text when the mouse is over it (if user sorting is allowed).</summary>
				/// <param name="color_text_header_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_text_header_hot(const color& color_text_header_hot);

				/// <summary>Get or set the table header's fill color.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_header();

				/// <summary>Set the table header's fill color.</summary>
				/// <param name="color_fill_header">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_fill_header(const color& color_fill_header);

				/// <summary>Get or set the table's alternate row color (used for the
				/// background of even rows).</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_alternate();

				/// <summary>Set the table's alternate row color (used for the background of
				/// even rows).</summary>
				/// <param name="color_fill_alternate"></param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_fill_alternate(const color& color_fill_alternate);

				/// <summary>Get or set the background color of a row when the mouse is over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_row_hot();

				/// <summary>Set the background color of a row when the mouse is over it.</summary>
				/// <param name="color_row_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_row_hot(const color& color_row_hot);

				/// <summary>Get or set the background color of a selected row.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_row_selected();

				/// <summary>Set the background color of a selected row.</summary>
				/// <param name="color_row_selected">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& color_row_selected(const color& color_row_selected);

				/// <summary>Get or set the table view columns.</summary>
				/// <returns>A reference to the list of the table view's columns.</returns>
				/// <example>
				/// Two columns can be defined as follows:
				/// columns = { { "Name", 130 }, { "Surname", 210 } };
				/// </example>
				std::vector<table_column>& columns();

				/// <summary>Set the table view columns.</summary>
				/// <param name="columns">The list of columns.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <example>
				/// Two columns can be defined as follows:
				/// columns = { { "Name", 130 }, { "Surname", 210 } };
				/// </example>
				table_view& columns(const std::vector<table_column>& columns);

				/// <summary>Get or set the table's data. Each entry of the vector (a map) is a
				/// row and should mirror the column structure. The key is the column name and
				/// the value is the contents of the cell. Therefore the map size should be
				/// equivalent to the number of columns, while the size of the vector will
				/// determine the number of rows.</summary>
				/// <returns>A reference to the table's data.</returns>
				/// <example>
				/// A table with two columns 'Name' and 'Surname' can be populated as follows
				/// to a total of three rows:
				/// data = {
				/// { {"Name", "Emily Yolanda"}, { "Surname", "Moyo" } },
				/// { {"Name", "Tanaka"}, {"Surname", "Sibanda"} },
				/// { {"Name", "Precious"}, {"Surname", "Ncube"} }
				/// </example>
				std::vector<table_row>& data();

				/// <summary>Set the table's data. Each entry of the vector (a map) is a
				/// row and should mirror the column structure. The key is the column name and
				/// the value is the contents of the cell. Therefore the map size should be
				/// equivalent to the number of columns, while the size of the vector will
				/// determine the number of rows.</summary>
				/// <param name="data">The table's data.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <example>
				/// A table with two columns 'Name' and 'Surname' can be populated as follows
				/// to a total of three rows:
				/// data = {
				/// { {"Name", "Emily Yolanda"}, { "Surname", "Moyo" } },
				/// { {"Name", "Tanaka"}, {"Surname", "Sibanda"} },
				/// { {"Name", "Precious"}, {"Surname", "Ncube"} }
				/// </example>
				table_view& data(const std::vector<table_row>& data);

				/// <summary>Get or set the rows to select by default, numbered from 0.</summary>
				/// <returns>A reference to the selected rows.</returns>
				std::vector<long>& selected();

				/// <summary>Set the rows to select by default, numbered from 0.</summary>
				/// <param name="selected">The selected rows.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& selected(const std::vector<long>& selected);

				/// <summary>Get or set whether to allow sorting by clicking columns.</summary>
				/// <returns>A reference to the property.</returns>
				bool& user_sort();

				/// <summary>Set whether to allow sorting by clicking columns.</summary>
				/// <param name="user_sort">The property.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& user_sort(const bool& user_sort);

				/// <summary>Get or set whether to add a fixed number column.</summary>
				/// <returns>A reference to the property.</returns>
				/// <remarks>Counts from 1 and is not sorted when the table's columns
				/// are user sorted.</remarks>
				bool& fixed_number_column();

				/// <summary>Set whether to add a fixed number column.</summary>
				/// <param name="fixed_number_column">The property.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Counts from 1 and is not sorted when the table's columns
				/// are user sorted.</remarks>
				table_view& fixed_number_column(const bool& fixed_number_column);

				/// <summary>Get or set the name of the fixed number column.</summary>
				/// <returns>A reference to the property.</returns>
				std::string& fixed_number_column_name();

				/// <summary>Set the name of the fixed number column.</summary>
				/// <param name="fixed_number_column_name">The property.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view& fixed_number_column_name(const std::string& fixed_number_column_name);

			public:
				/// <summary>Add a table_view to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "debtors".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static table_view& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a table_view.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_pane/debtors".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static table_view& get(form& fm, const std::string& path);

				/// <summary>Programmatically scroll the table vertically.</summary>
				/// <param name="amount">The amount of pixels to scroll the table by.</param>
				/// <remarks>A negative value moves the table upwards; it's equivalent to sliding
				/// the vertical scroll bar downwards, and vice-versa. If the table has no vertical scroll
				/// bar this method has no effect.</remarks>
				void scroll_vertically(const float& amount);

				/// <summary>Programmatically scroll the table horizontally.</summary>
				/// <param name="amount">The amount of pixels to scroll the table by.</param>
				/// <remarks>A negative value moves the table to the left; it's equivalent to sliding
				/// the horizontal scroll bar towards the right, and vice-versa. If the table has no
				/// horizontal scroll bar this method has no effect.</remarks>
				void scroll_horizontally(const float& amount);

			private:
				/// <summary>Table view events.</summary>
				table_view_events _table_events;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting table view widget. 'Get' method documentation applies.
#define get_table_view(path) liblec::lecui::widgets::table_view::get(*this, path)
#endif
