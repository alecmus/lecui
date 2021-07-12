//
// text_field.h - text_field widget interface
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

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Text field widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 200x25px</remarks>
			class lecui_api text_field_specs : public specs {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 3.f;
				float _corner_radius_y = 3.f;
				std::string _prompt = "Enter text here";
				color _color_prompt;
				color _color_caret;

			public:
				text_field_specs() {
					_cursor = cursor_type::caret;
					_rect.size({ 200.f, 25.f });
				}

				bool operator==(const text_field_specs&);
				bool operator!=(const text_field_specs&);

				// generic specs

				std::string& text() override;
				text_field_specs& text(const std::string& text);

				std::string& tooltip() override;
				text_field_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				text_field_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				text_field_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				text_field_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				text_field_specs& font(const std::string& font);

				float& font_size() override;
				text_field_specs& font_size(const float& font_size);

				color& color_text() override;
				text_field_specs& color_text(const color& color_text);

				color& color_fill() override;
				text_field_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				text_field_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				text_field_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				text_field_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the text to be displayed when the widget is empty.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& prompt();

				/// <summary>Set the text to be displayed when the widget is empty.</summary>
				/// <param name="prompt">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& prompt(const std::string& prompt);

				/// <summary>Get or set the color of the prompt text.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_prompt();

				/// <summary>Set the color of the prompt text.</summary>
				/// <param name="color_prompt">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& color_prompt(const color& color_prompt);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of to use for the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field_specs& color_caret(const color& color_caret);

			private:
				/// <summary>The character to use for masking the text.</summary>
				char mask = '\0';

#if defined(LECUI_EXPORTS)
				friend class password_field_builder;
				friend class text_field_impl;
#endif
			};

			/// <summary>Text field widget builder.</summary>
			class lecui_api text_field_builder {
			public:
				/// <summary>Text field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				text_field_builder(containers::page& page);

				/// <summary>Text field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				text_field_builder(containers::page& page, const std::string& alias);
				virtual ~text_field_builder();

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
				impl& _d;

				class password_field_builder;
				friend class password_field_builder;

				// Default constructor and copying an object of this class are not allowed
				text_field_builder() = delete;
				text_field_builder(const text_field_builder&) = delete;
				text_field_builder& operator=(const text_field_builder&) = delete;
			};
		}
	}
}
