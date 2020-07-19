/*
** table_view.h - table view widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

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
				class table_view_specs : public specs {
				public:
					table_view_specs() {}
					color color_border;
					color color_dropdown_hot = { 255, 255, 255, 255 };
					color color_menu = { 255, 255, 255, 255 };
					color color_grid;
					color color_text_header;
					color color_fill_header;
					color color_fill_alternate;
					color color_row_hot;
					color color_row_selected;
					float border = .5f;
					float corner_radius_x = 2.f;
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

					/// <summary>The rows to select by default.</summary>
					std::vector<long> selected;

					struct table_view_events : basic_events {
						/// <summary>This handler is called when the selection changes. The selected
						/// rows will be in the given 'rows' parameter.</summary>
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
				table_view();
				table_view(const table_view&);
				table_view& operator=(const table_view&);
			};
		}
	}
}
