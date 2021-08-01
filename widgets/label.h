//
// label.h - label widget interface
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
			/// This widget supports two kinds of formatting:
			/// 
			/// 1. HTML formatting with CSS style tags
			/// 
			/// e.g. "<html>This is <span style='font-size: 16.5pt; font-style: italic; color: 0x0000FFFF;'>good</span>.</html>";
			/// 
			/// The supported tags are as follows:
			/// 
			/// strong			Bold text
			/// em				Italic text
			/// u				Underline text
			/// s				Strikethrough text
			/// span			Use CSS style tags
			/// 
			/// The span tag supports the style attribute and the following properties:
			/// 
			/// font-family		The font name
			/// font-size		The font size, either in pt or in px
			/// font-weight		The font weight, either 'none' or 'bold'
			/// font-style		The font style, either 'none' or 'italic'
			/// text-decoration	The text decoration, either 'none' or 'underline' or 'line-through'
			/// color			The text color, either in the form rgb(255, 0, 0) or 0xFF0000FF or #FF0000FF
			/// 
			/// Note: the strong, em and u html tags are automatically converted to CSS style tags internally.
			/// 
			/// 2. Custom formatting
			/// 
			/// Start and end of XML formatting is marked by the 'text' tag: <text></text>
			/// 
			/// The supported attributes of the text are:
			/// 
			/// font			The font name
			/// size			The font size, in points
			/// bold			'true' or 'false'
			/// italic			'true' or 'false'
			/// underline		'true' or 'false'
			/// strikethrough	'true' or 'false'
			/// color			0xAABBCCDD where r = AA, g = BB, b=CC, a=DD
			/// 
			/// e.g. "This is <text size='16.5' italic='true' color='0x0000FFFF'>good</text>.";
			/// 
			/// Both of the above two methods of formatting will display a label with the sentence:
			/// 
			/// This is good.
			/// 
			/// The word 'good' will be of size 16.5, italic and blue, while the rest
			/// of the text will use the widget specifications.
			/// 
			
			/// <summary>Label widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 200x20px.</remarks>
			class lecui_api label : public widget {
				color _color_hot_pressed;
				bool _multiline = false;
				bool _center_h = false;
				bool _center_v = false;

			public:
				label() {
					_rect.size({ 200.f, 20.f });
				};

				bool operator==(const label&);
				bool operator!=(const label&);

				// generic widget

				std::string& text() override;
				label& text(const std::string& text);

				std::string& tooltip() override;
				label& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				label& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				label& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				label& cursor(const cursor_type cursor);

				std::string& font() override;
				label& font(const std::string& font);

				float& font_size() override;
				label& font_size(const float& font_size);

				color& color_text() override;
				label& color_text(const color& color_text);

				color& color_fill() override;
				label& color_fill(const color& color_fill);

				color& color_hot() override;
				label& color_hot(const color& color_hot);

				color& color_selected() override;
				label& color_selected(const color& color_selected);

				color& color_disabled() override;
				label& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the color used when the label is pressed and the mouse is over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_hot_pressed();

				/// <summary>Set the color used when the label is pressed and the mouse is over it.</summary>
				/// <param name="color_hot_pressed">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				label& color_hot_pressed(const color& color_hot_pressed);

				/// <summary>Get or set the property for whether to allow multiple lines.</summary>
				/// <returns>A reference to the property.</returns>
				bool& multiline();

				/// <summary>Set the property for whether to allow multiple lines.</summary>
				/// <param name="multiline">Whether to allow multiple lines.</param>
				/// <returns>A reference to the modified object.</returns>
				label& multiline(const bool& multiline);

				/// <summary>Get or set the property for whether to center the text horizontally.</summary>
				/// <returns>A reference to the property.</returns>
				bool& center_h();

				/// <summary>Set the property for whether to center the text horizontally.</summary>
				/// <param name="center_h">Whether to center the text horizontally.</param>
				/// <returns>A reference to the modified object.</returns>
				label& center_h(const bool& center_h);

				/// <summary>Get or set the property for whether to center the text vertically.</summary>
				/// <returns>A reference to the property.</returns>
				bool& center_v();

				/// <summary>Set the property for whether to center the text vertically.</summary>
				/// <param name="center_h">Whether to center the text vertically.</param>
				/// <returns>A reference to the modified object.</returns>
				label& center_v(const bool& center_v);

			public:
				/// <summary>Add a label to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static label& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a label.</summary>
				/// <param name="fm"></param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static label& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting label widget. Builder documentation applies.
#define get_label(path) liblec::lecui::widgets::label::get(*this, path)
