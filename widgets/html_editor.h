/*
** html_editor.h - html editor widget interface
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
			/// <summary>Textbox widget.</summary>
			class lecui_api html_editor {
			public:
				/// <summary>HTML editor widget specifications.</summary>
				class html_editor_specs : public specs {
				public:
					html_editor_specs() {
						cursor = cursor_type::caret;
						rect.size({ 200.f, 25.f });
					}
					float border = .5f;
					float corner_radius_x = 3.f;
					float corner_radius_y = 3.f;
					color color_border;
					color color_caret;

					bool operator==(const html_editor_specs&);
					bool operator!=(const html_editor_specs&);
				};

				/// <summary>HTML editor constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				html_editor(containers::page& page, const std::string& alias);
				virtual ~html_editor();

				/// <summary>Get the html editor specifications.</summary>
				/// <returns>A reference to the html_editor specifications.</returns>
				[[nodiscard]]
				html_editor_specs& specs();

				/// <summary>Get the html editor specifications.</summary>
				/// <returns>A reference to the html_editor specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				html_editor_specs& operator()();

				/// <summary>Get the specifications of a html editor.</summary>
				/// <param name="fm">The form containing the html editor.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the html editor specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static html_editor_specs& specs(form& fm, const std::string& path);

			protected:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				html_editor();
				html_editor(const html_editor&);
				html_editor& operator=(const html_editor&);
			};
		}
	}
}
