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

			/// <summary>Label widget.</summary>
			class lecui_api label {
			public:
				/// <summary>Label widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 200x20px.</remarks>
				class label_specs : public specs {
					/// <summary>The color when the label is pressed and the mouse is over it.
					/// </summary>
					color color_hot_pressed_;

					/// <summary>Whether to allow multiple lines.</summary>
					bool multiline_ = false;

					/// <summary>Whether to center the text horizontally.</summary>
					bool center_h_ = false;

					/// <summary>Whether to center the text vertically.</summary>
					bool center_v_ = false;

				public:
					label_specs() {
						rect_.size({ 200.f, 20.f });
					};

					bool operator==(const label_specs&);
					bool operator!=(const label_specs&);

					// generic specs

					std::string& text() override { return text_; }
					label_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					label_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					label_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					label_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					label_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					label_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					label_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					label_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					label_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					label_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					label_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					label_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					color& color_hot_pressed() { return color_hot_pressed_; }
					label_specs& color_hot_pressed(const color& color_hot_pressed) {
						color_hot_pressed_ = color_hot_pressed;
						return *this;
					}

					bool& multiline() { return multiline_; }
					label_specs& multiline(const bool& multiline) {
						multiline_ = multiline;
						return *this;
					}

					bool& center_h() { return center_h_; }
					label_specs& center_h(const bool& center_h) {
						center_h_ = center_h;
						return *this;
					}

					bool& center_v() { return center_v_; }
					label_specs& center_v(const bool& center_v) {
						center_v_ = center_v;
						return *this;
					}
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
