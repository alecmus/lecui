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
#if defined(LECUI_EXPORTS)
		namespace widgets_impl {
			class textbox;
		}
#endif

		namespace widgets {
			/// <summary>Textbox widget.</summary>
			class lecui_api textbox {
			public:
				/// <summary>Textbox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class textbox_specs : public specs {
				public:
					textbox_specs() {
						cursor = cursor_type::caret;
						rect.size({ 200.f, 25.f });
						color_text = { 0, 0, 0, 255 };
						color_fill = { 255, 255, 255, 255 };
						color_selected = { 0, 120, 170, 100 };
						color_disabled = { 248, 248, 248, 255 };
					}
					std::string prompt = "Enter text here";
					float border = .5f;
					float corner_radius_x = 3.f;
					float corner_radius_y = 3.f;
					color color_border = { 150, 150, 150, 255 };
					color color_prompt = { 180, 180, 180, 255 };
					color color_caret = { 50, 50, 50, 255 };

					bool operator==(const textbox_specs&);
					bool operator!=(const textbox_specs&);

				private:
					/// <summary>The character to use for masking the text.</summary>
					char mask = '\0';

#if defined(LECUI_EXPORTS)
					friend class passwordbox;
					friend class lecui::widgets_impl::textbox;
#endif
				};

				/// <summary>Textbox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				textbox(containers::page& page, const std::string& alias);
				virtual ~textbox();

				/// <summary>Get the textbox specifications.</summary>
				/// <returns>A reference to the textbox specifications.</returns>
				[[nodiscard]]
				textbox_specs& specs();

				/// <summary>Get the textbox specifications.</summary>
				/// <returns>A reference to the textbox specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				textbox_specs& operator()();

				/// <summary>Get the specifications of a textbox.</summary>
				/// <param name="fm">The form containing the textbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the textbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static textbox_specs& specs(form& fm, const std::string& path);

			protected:
				class impl;
				impl& d_;

				class passwordbox;
				friend class passwordbox;

				// Default constructor and copying an object of this class are not allowed
				textbox();
				textbox(const textbox&);
				textbox& operator=(const textbox&);
			};
		}
	}
}
