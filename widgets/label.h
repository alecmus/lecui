//
// label.h - label widget interface
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

namespace liblec {
	namespace lecui {
		namespace widgets {
			// This widget supports two kinds of formatting:
			// 
			// 1. HTML formatting with CSS style tags
			// 
			// e.g. "<html>This is <span style='font-size: 16.5pt; font-style: italic; color: 0x0000FFFF;'>good</span>.</html>";
			// 
			// The supported tags are as follows:
			// 
			// strong			Bold text
			// em				Italic text
			// u				Underline text
			// s				Strikethrough text
			// span			Use CSS style tags
			// 
			// The span tag supports the style attribute and the following properties:
			// 
			// font-family		The font name
			// font-size		The font size, either in pt or in px
			// font-weight		The font weight, either 'none' or 'bold'
			// font-style		The font style, either 'none' or 'italic'
			// text-decoration	The text decoration, either 'none' or 'underline' or 'line-through'
			// color			The text color, either in the form rgb(255, 0, 0) or 0xFF0000FF or #FF0000FF
			// 
			// Note: the strong, em and u html tags are automatically converted to CSS style tags internally.
			// 
			// 2. Custom formatting
			// 
			// Start and end of XML formatting is marked by the 'text' tag: <text></text>
			// 
			// The supported attributes of the text are:
			// 
			// font			The font name
			// size			The font size, in points
			// bold			'true' or 'false'
			// italic			'true' or 'false'
			// underline		'true' or 'false'
			// strikethrough	'true' or 'false'
			// color			0xAABBCCDD where r = AA, g = BB, b=CC, a=DD
			// 
			// e.g. "This is <text size='16.5' italic='true' color='0x0000FFFF'>good</text>.";
			// 
			// Both of the above two methods of formatting will display a label with the sentence:
			// 
			// This is good.
			// 
			// The word 'good' will be of size 16.5, italic and blue, while the rest
			// of the text will use the widget specifications.
			// 
			
			/// <summary>Label widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 200x20px.</remarks>
			class lecui_api label : public widget {
				color _color_hot_pressed;
				bool _multiline = false;
				text_alignment _alignment = text_alignment::left;
				paragraph_alignment _paragraph_alignment = paragraph_alignment::top;

			public:
				/// <summary>Constructor.</summary>
				label() {
					_rect.size({ 200.f, 20.f });
				};

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const label& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const label& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				label& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				label& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				label& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				label& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				label& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				label& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				label& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				label& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				label& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				label& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				label& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
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

				/// <summary>Get or set the text alignment.</summary>
				/// <returns>A reference to the property.</returns>
				text_alignment& alignment();

				/// <summary>Set the text alignment.</summary>
				/// <param name="alignment">The alignment of the text as defined in <see cref='text_alignment'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				label& alignment(const text_alignment& alignment);

				/// <summary>Get or set the paragraph alignment.</summary>
				/// <returns>A reference to the property.</returns>
				lecui::paragraph_alignment& paragraph_alignment();

				/// <summary>Set the paragraph alignment.</summary>
				/// <param name="paragraph_alignment">The paragraph alignment, as defined in <see cref='lecui::paragraph_alignment'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				label& paragraph_alignment(const lecui::paragraph_alignment& paragraph_alignment);

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
				/// <param name="fm">A reference to the form.</param>
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

#if defined(lecui_helper)
/// Helper for getting label widget. 'Get' method documentation applies.
#define get_label(path) liblec::lecui::widgets::label::get(*this, path)
#endif
