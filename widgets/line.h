/*
** line.h - line widget interface
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

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Line widget.</summary>
			class lecui_api line {
			public:
				/// <summary>Line widget specifications.</summary>
				class line_specs : public specs {
				public:
					line_specs() {}

					/// <summary>The thickness of the line.</summary>
					float thickness = 1.f;

					/// <summary>The points, in order, for the line line to go through. These
					/// are relative to the top left corner of the line's bounding rectangle.
					/// </summary>
					std::vector<point> points;

					bool operator==(const line_specs&);
					bool operator!=(const line_specs&);
				};

				/// <summary>Line constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				line(containers::page& page);

				/// <summary>Line constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_line".</param>
				line(containers::page& page, const std::string& alias);
				~line();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				[[nodiscard]]
				line_specs& specs();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				line_specs& operator()();

				/// <summary>Get the specifications of a line.</summary>
				/// <param name="fm">The form containing the line.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_line".</param>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static line_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				line() = delete;
				line(const line&) = delete;
				line& operator=(const line&) = delete;
			};
		}
	}
}
