//
// text_field.h - text_field widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

#include <set>

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Text field widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 200x25px</remarks>
			class lecui_api text_field : public widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 3.f;
				float _corner_radius_y = 3.f;
				std::string _prompt = "Enter text here";
				color _color_prompt;
				color _color_caret;
				color _color_input_error;
				std::set<char> _allowed_characters;
				std::set<char> _forbidden_characters;
				int _maximum_length = 0;

			public:
				text_field() {
					_cursor = cursor_type::caret;
					_rect.size({ 200.f, 25.f });
				}

				bool operator==(const text_field&);
				bool operator!=(const text_field&);

				// generic widget

				std::string& text() override;
				text_field& text(const std::string& text);

				std::string& tooltip() override;
				text_field& tooltip(const std::string& tooltip);

				badge_specs& badge() override;
				text_field& badge(const badge_specs& badge);

				lecui::rect& rect() override;
				text_field& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				text_field& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				text_field& cursor(const cursor_type cursor);

				std::string& font() override;
				text_field& font(const std::string& font);

				float& font_size() override;
				text_field& font_size(const float& font_size);

				color& color_text() override;
				text_field& color_text(const color& color_text);

				color& color_fill() override;
				text_field& color_fill(const color& color_fill);

				color& color_hot() override;
				text_field& color_hot(const color& color_hot);

				color& color_selected() override;
				text_field& color_selected(const color& color_selected);

				color& color_disabled() override;
				text_field& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the text to be displayed when the widget is empty.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& prompt();

				/// <summary>Set the text to be displayed when the widget is empty.</summary>
				/// <param name="prompt">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& prompt(const std::string& prompt);

				/// <summary>Get or set the color of the prompt text.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_prompt();

				/// <summary>Set the color of the prompt text.</summary>
				/// <param name="color_prompt">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& color_prompt(const color& color_prompt);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of to use for the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& color_caret(const color& color_caret);

				/// <summary>Get or set the color to show that there has been an input error.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_input_error();

				/// <summary>Set the color to show that there has been an input error.</summary>
				/// <param name="color_input_error">The color to show that there has been an input error.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& color_input_error(const color& color_input_error);

				/// <summary>Get or set the character set to limit input to.</summary>
				/// <returns>A reference to the character set.</returns>
				/// <remarks>An empty character set means there is no limit.</remarks>
				std::set<char>& allowed_characters();

				/// <summary>Set the character set to limit input to.</summary>
				/// <param name="allowed_characters">The character set. An empty character set means there is no limit.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& allowed_characters(const std::set<char>& allowed_characters);

				/// <summary>Get or set the set of characters to prevent from being typed in.</summary>
				/// <returns>A reference to the character set.</returns>
				/// <remarks>An empty character set means all characters are permitted. Takes precedence over the allowed character set.</remarks>
				std::set<char>& forbidden_characters();

				/// <summary>Set the set of characters to prevent from being typed in.</summary>
				/// <param name="forbidden_characters">The character set. An empty character set means all characters are permitted.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Takes precedence over the allowed character set.</remarks>
				text_field& forbidden_characters(const std::set<char>& forbidden_characters);

				/// <summary>Get or set the maximum length of the input string. 0 means unlimited length.</summary>
				/// <returns>A reference to the maximum length.</returns>
				int& maximum_length();

				/// <summary>Set the maximum length of the input string.</summary>
				/// <param name="maximum_length">The maximum length of the input string. 0 means unlimited length.</param>
				/// <returns>A reference to the modified object.</returns>
				text_field& maximum_length(const int& maximum_length);

			public:
				/// <summary>Add a text field to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static text_field& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a text field.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static text_field& get(form& fm, const std::string& path);

			private:
				/// <summary>The character to use for masking the text.</summary>
				char mask = '\0';

#if defined(LECUI_EXPORTS)
				friend class password_field_builder;
				friend class text_field_impl;
#endif
			};
		}
	}
}

/// Helper for getting text field widget. Builder documentation applies.
#define get_text_field(path) liblec::lecui::widgets::text_field::get(*this, path)
