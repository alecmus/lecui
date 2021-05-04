/*
** label.h - label widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
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

			/// <summary>Label widget.</summary>
			class lecui_api label {
			public:
				/// <summary>Label widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 200x20px.</remarks>
				class label_specs : public specs {
				public:
					label_specs() {
						rect.size({ 200.f, 20.f });
					};

					/// <summary>The color when the label is pressed and the mouse is over it.
					/// </summary>
					color color_hot_pressed;

					/// <summary>Whether to allow multiple lines.</summary>
					bool multiline = false;

					/// <summary>Whether to center the text horizontally.</summary>
					bool center_h = false;

					/// <summary>Whether to center the text vertically.</summary>
					bool center_v = false;

					bool operator==(const label_specs&);
					bool operator!=(const label_specs&);
				};

				/// <summary>Label constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				label(containers::page& page);

				/// <summary>Label constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
				label(containers::page& page, const std::string& alias);
				~label();

				/// <summary>Get the label specifications.</summary>
				/// <returns>A reference to the label specifications.</returns>
				[[nodiscard]]
				label_specs& specs();

				/// <summary>Get the label specifications.</summary>
				/// <returns>A reference to the label specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				label_specs& operator()();

				/// <summary>Get the specifications of a label.</summary>
				/// <param name="fm">The form containing the label.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the label specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static label_specs& specs(form& fm,const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				label() = delete;
				label(const label&) = delete;
				label& operator=(const label&) = delete;
			};
		}
	}
}
