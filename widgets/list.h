/*
** list.h - list widget interface
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
		namespace containers {
			class page;
		}

		namespace widgets {
			namespace specs {
				/// <summary>List widget specifications.</summary>
				class list : public widget {
				public:
					list() {
						color_fill = { 255, 255, 255, 255 };
						color_hot = { 200, 200, 200, 50 }; }
					color color_border = { 150, 150, 150, 255 };
					color color_dropdown_hot = { 255, 255, 255, 255 };
					color color_menu = { 255, 255, 255, 255 };
					color color_grid = { 215, 215, 215, 255 };
					color color_text_header = { 0, 0, 0, 255 };
					color color_fill_header = { 222, 222, 222, 255 };
					color color_fill_alternate = { 0, 120, 170, 15 };
					color color_row_hot = { 225, 242, 255, 255 };
					color color_row_selected = { 40, 160, 200, 255 };
					float border = .5f;
					float corner_radius_x = 2.f;
					float corner_radius_y = 2.f;

					/// <summary>The table columns.</summary>
					/// <example>
					/// Two columns can be defined as follows:
					/// columns = { { "Name", 130 }, { "Surname", 210 } };
					/// </example>
					std::vector<list_column> columns;

					/// <summary>
					/// The table. Each entry of the vector (a map) is a row and should
					/// mirror the column structure. The key is the column name and the value is
					/// the contents of the cell. Therefore the map size should be equivalent to
					/// the number of columns, while the size of the vector will determine the
					/// number of rows.
					/// </summary>
					/// <example>
					/// A table with two columns 'Name' and 'Surname' can be populated as follows
					/// to a total of three rows:
					/// table = {
					/// { {"Name", "Emily Yolanda"}, { "Surname", "Moyo" } },
					/// { {"Name", "Tanaka"}, {"Surname", "Sibanda"} },
					/// { {"Name", "Precious"}, {"Surname", "Ncube"} }
					/// </example>
					std::vector<std::map<std::string, std::string>> table;

					/// <summary>The rows to select by default.</summary>
					std::vector<long> selected;

					/// <summary>This handler is called when the selection changes. The selected
					/// rows will be in the given 'rows' parameter.</summary>
					std::function<void(
						const std::vector<std::map<std::string, std::string>>& rows)>
						on_selection = nullptr;

					bool operator==(const list&);
					bool operator!=(const list&);
				};
			}

			/// <summary>List widget.</summary>
			class lecui_api list {
			public:
				list(containers::page& page);
				~list();

				/// <summary>Add a list widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "debtors".</param>
				/// <returns>A reference to the list specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::list&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing list.</summary>
				/// <param name="fm">The form containing the list.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/debtors".</param>
				/// <returns>A reference to the list specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::list&
					specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				list();
				list(const list&);
				list& operator=(const list&);
			};
		}
	}
}
