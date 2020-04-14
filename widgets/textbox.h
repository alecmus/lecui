/*
** textbox.h - textbox widget interface
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
		namespace containers {
			class page;
		}

		namespace widgets {
			/// <summary>Textbox widget specifications.</summary>
			/// <remarks>Recommended height with defaults is 25px</remarks>
			class textbox_specs : public specs {
			public:
				textbox_specs() {
					color_text = { 0, 0, 0, 255 };
					color_fill = { 255, 255, 255, 255 };
					color_selected = { 0, 120, 170, 100 };
					color_disabled = { 248, 248, 248, 255 };
				}
				std::string prompt = "Enter text here";
				float border = .5f;
				float corner_radius_x = 5.f;
				float corner_radius_y = 5.f;
				color color_border = { 150, 150, 150, 255 };
				color color_prompt = { 180, 180, 180, 255 };
				color color_caret = { 50, 50, 50, 255 };

				bool operator==(const textbox_specs&);
				bool operator!=(const textbox_specs&);
			};

			/// <summary>Textbox widget.</summary>
			class lecui_api textbox {
			public:
				textbox(containers::page& page);
				~textbox();

				/// <summary>Add a textbox widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				/// <returns>A reference to the textbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				textbox_specs& add(const std::string& alias);

				/// <summary>Get the specifications of an existing textbox.</summary>
				/// <param name="fm">The form containing the textbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the textbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static textbox_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				textbox();
				textbox(const textbox&);
				textbox& operator=(const textbox&);
			};
		}
	}
}
