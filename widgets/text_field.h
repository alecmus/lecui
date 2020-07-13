/*
** text_field.h - text_field widget interface
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
			/// <summary>Text field widget.</summary>
			class lecui_api text_field {
			public:
				/// <summary>Text field widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class text_field_specs : public specs {
				public:
					text_field_specs() {
						cursor = cursor_type::caret;
						rect.size({ 200.f, 25.f });
					}
					std::string prompt = "Enter text here";
					float border = .5f;
					float corner_radius_x = 3.f;
					float corner_radius_y = 3.f;
					color color_border;
					color color_prompt;
					color color_caret;

					bool operator==(const text_field_specs&);
					bool operator!=(const text_field_specs&);

				private:
					/// <summary>The character to use for masking the text.</summary>
					char mask = '\0';

#if defined(LECUI_EXPORTS)
					friend class password_field;
					friend class text_field_impl;
#endif
				};

				/// <summary>Text field constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				text_field(containers::page& page, const std::string& alias);
				virtual ~text_field();

				/// <summary>Get the text_field specifications.</summary>
				/// <returns>A reference to the text_field specifications.</returns>
				[[nodiscard]]
				text_field_specs& specs();

				/// <summary>Get the text_field specifications.</summary>
				/// <returns>A reference to the text_field specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				text_field_specs& operator()();

				/// <summary>Get the specifications of a text_field.</summary>
				/// <param name="fm">The form containing the text_field.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the text_field specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static text_field_specs& specs(form& fm, const std::string& path);

			protected:
				class impl;
				impl& d_;

				class password_field;
				friend class password_field;

				// Default constructor and copying an object of this class are not allowed
				text_field();
				text_field(const text_field&);
				text_field& operator=(const text_field&);
			};
		}
	}
}
