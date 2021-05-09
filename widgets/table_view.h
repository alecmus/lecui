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
			/// <summary>Table view widget.</summary>
			class lecui_api table_view {
			public:
				/// <summary>Table view widget specifications.</summary>
				/// <remarks>Default size is 200x200px.</remarks>
				class table_view_specs : public specs {
				public:
					table_view_specs() {
						rect.size({ 200.f, 200.f });
					}

					/// <summary>The color of the text in a selected row.</summary>
					color color_text_selected;

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The color of the grid lines.</summary>
					color color_grid;

					/// <summary>The color of the header text.</summary>
					color color_text_header;

					/// <summary>The table header's fill color.</summary>
					color color_fill_header;

					/// <summary>The table's alternate row color (used for the background of
					/// even rows).</summary>
					color color_fill_alternate;
					
					/// <summary>The background color of a row when the mouse is over it.</summary>
					color color_row_hot;

					/// <summary>The background color of a selected row.</summary>
					color color_row_selected;

					/// <summary>The thickness of the border.</summary>
					float border = .5f;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = 2.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = 2.f;

					/// <summary>The table view columns.</summary>
					/// <example>
					/// Two columns can be defined as follows:
					/// columns = { { "Name", 130 }, { "Surname", 210 } };
					/// </example>
					std::vector<table_column> columns;

					/// <summary>
					/// The table data. Each entry of the vector (a map) is a row and should
					/// mirror the column structure. The key is the column name and the value is
					/// the contents of the cell. Therefore the map size should be equivalent to
					/// the number of columns, while the size of the vector will determine the
					/// number of rows.
					/// </summary>
					/// <example>
					/// A table with two columns 'Name' and 'Surname' can be populated as follows
					/// to a total of three rows:
					/// data = {
					/// { {"Name", "Emily Yolanda"}, { "Surname", "Moyo" } },
					/// { {"Name", "Tanaka"}, {"Surname", "Sibanda"} },
					/// { {"Name", "Precious"}, {"Surname", "Ncube"} }
					/// </example>
					std::vector<std::map<std::string, std::string>> data;

					/// <summary>The rows to select by default, numbered from 0.</summary>
					std::vector<long> selected;

					/// <summary>Events specific to this widget.</summary>
					struct table_view_events : basic_events {
						/// <summary>This handler is called when the selection changes.</summary>
						/// <remarks>The parameter contains the selected rows.</remarks>
						std::function<void(
							const std::vector<std::map<std::string, std::string>>& rows)>
							selection = nullptr;
					};

					table_view_events& events() {
						return table_events_;
					}

					bool operator==(const table_view_specs&);
					bool operator!=(const table_view_specs&);

				private:
					table_view_events table_events_;
				};

				/// <summary>Table constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				table_view(containers::page& page);

				/// <summary>Table constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "debtors".</param>
				table_view(containers::page& page, const std::string& alias);
				~table_view();

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
				table_view() = delete;
				table_view(const table_view&) = delete;
				table_view& operator=(const table_view&) = delete;
			};
		}
	}
}
