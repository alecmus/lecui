//
// table_view.h - table view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

#include <map>

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Table view widget specifications.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api table_view_specs : public specs {
				float border_ = .5f;
				color color_border_;
				float corner_radius_x_ = 2.f;
				float corner_radius_y_ = 2.f;
				color color_text_selected_;
				color color_grid_;
				color color_text_header_;
				color color_text_header_hot_;
				color color_fill_header_;
				color color_fill_alternate_;
				color color_row_hot_;
				color color_row_selected_;
				std::vector<table_column> columns_;
				std::vector<std::map<std::string, std::string>> data_;
				std::vector<long> selected_;
				bool user_sort_ = false;
				void* p_special_pane_specs_ = nullptr;

			public:
				table_view_specs() {
					rect_.size({ 200.f, 200.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct table_view_events : basic_events {
					/// <summary>This handler is called when the selection changes.</summary>
					/// <remarks>The parameter contains the selected rows.</remarks>
					std::function<void(
						const std::vector<std::map<std::string, std::string>>& rows)>
						selection = nullptr;

					/// <summary>This handler is called when the table view is right clicked.</summary>
					/// <remarks>The parameter contained the selected rows.</remarks>
					std::function<void(
						const std::vector<std::map<std::string, std::string>>& rows)>
						context_menu = nullptr;
				};

				table_view_events& events() {
					return table_events_;
				}

				bool operator==(const table_view_specs&);
				bool operator!=(const table_view_specs&);
				table_view_specs& operator=(const table_view_specs&);
				table_view_specs(const table_view_specs&);

				// generic specs

				std::string& text() override;
				table_view_specs& text(const std::string& text);

				std::string& tooltip() override;
				table_view_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				table_view_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				table_view_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				table_view_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				table_view_specs& font(const std::string& font);

				float& font_size() override;
				table_view_specs& font_size(const float& font_size);

				color& color_text() override;
				table_view_specs& color_text(const color& color_text);

				color& color_fill() override;
				table_view_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				table_view_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				table_view_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				table_view_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the color of the text in a selected row.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_selected();

				/// <summary>Set the color of the text in a selected row.</summary>
				/// <param name="color_text_selected">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_text_selected(const color& color_text_selected);

				/// <summary>Get or set the color of the grid lines.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_grid();

				/// <summary>Set the color of the grid lines.</summary>
				/// <param name="color_grid">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_grid(const color& color_grid);

				/// <summary>Get or set the color of the header text.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_header();

				/// <summary>Set the color of the header text.</summary>
				/// <param name="color_text_header">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_text_header(const color& color_text_header);

				/// <summary>Get or set the color of the header text when the mouse is over it (if user sorting is allowed).</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_header_hot();

				/// <summary>Set the color of the header text when the mouse is over it (if user sorting is allowed).</summary>
				/// <param name="color_text_header_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_text_header_hot(const color& color_text_header_hot);

				/// <summary>Get or set the table header's fill color.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_header();

				/// <summary>Set the table header's fill color.</summary>
				/// <param name="color_fill_header">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_fill_header(const color& color_fill_header);

				/// <summary>Get or set the table's alternate row color (used for the
				/// background of even rows).</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_alternate();

				/// <summary>Set the table's alternate row color (used for the background of
				/// even rows).</summary>
				/// <param name="color_fill_alternate"></param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_fill_alternate(const color& color_fill_alternate);

				/// <summary>Get or set the background color of a row when the mouse is over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_row_hot();

				/// <summary>Set the background color of a row when the mouse is over it.</summary>
				/// <param name="color_row_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_row_hot(const color& color_row_hot);

				/// <summary>Get or set the background color of a selected row.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_row_selected();

				/// <summary>Set the background color of a selected row.</summary>
				/// <param name="color_row_selected">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& color_row_selected(const color& color_row_selected);

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
				table_view_specs& columns(const std::vector<table_column>& columns);

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
				std::vector<std::map<std::string, std::string>>& data();

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
				table_view_specs& data(const std::vector<std::map<std::string, std::string>>& data);

				/// <summary>Get or set the rows to select by default, numbered from 0.</summary>
				/// <returns>A reference to the selected rows.</returns>
				std::vector<long>& selected();

				/// <summary>Set the rows to select by default, numbered from 0.</summary>
				/// <param name="selected">The selected rows.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& selected(const std::vector<long>& selected);

				/// <summary>Get or set whether to allow sorting by clicking columns.</summary>
				/// <returns>A reference to the property.</returns>
				bool& user_sort();

				/// <summary>Set whether to allow sorting by clicking columns.</summary>
				/// <param name="user_sort">The property.</param>
				/// <returns>A reference to the modified object.</returns>
				table_view_specs& user_sort(const bool& user_sort);

			private:
				table_view_events table_events_;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};

			/// <summary>Table view widget builder.</summary>
			class lecui_api table_view_builder {
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
				table_view_specs& specs();

				/// <summary>Get the table view specifications.</summary>
				/// <returns>A reference to the table view specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				table_view_specs& operator()();

				/// <summary>Get the specifications of a table view.</summary>
				/// <param name="fm">The form containing the table view.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/debtors".</param>
				/// <returns>A reference to the table view specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static table_view_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				table_view_builder() = delete;
				table_view_builder(const table_view_builder&) = delete;
				table_view_builder& operator=(const table_view_builder&) = delete;
			};
		}
	}
}
