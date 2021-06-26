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
			/// <summary>Text field widget.</summary>
			class lecui_api text_field {
			public:
				/// <summary>Text field widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 200x25px</remarks>
				class text_field_specs : public specs {
					/// <summary>The prompt text, displayed when the widget is empty.</summary>
					std::string prompt_ = "Enter text here";

					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 3.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 3.f;

					/// <summary>The color of the prompt text.</summary>
					color color_prompt_;

					/// <summary>The color of the caret.</summary>
					color color_caret_;

				public:
					text_field_specs() {
						cursor_ = cursor_type::caret;
						rect_.size({ 200.f, 25.f });
					}

					bool operator==(const text_field_specs&);
					bool operator!=(const text_field_specs&);

					// generic specs

					std::string& text() override { return text_; }
					text_field_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					text_field_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					text_field_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					text_field_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					text_field_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					text_field_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					text_field_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					text_field_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					text_field_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					text_field_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					text_field_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					text_field_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					std::string& prompt() { return prompt_; }
					text_field_specs& prompt(const std::string& prompt) {
						prompt_ = prompt;
						return *this;
					}

					float& border() { return border_; }
					text_field_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					color& color_border() { return color_border_; }
					text_field_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					text_field_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					text_field_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}

					color& color_prompt() { return color_prompt_; }
					text_field_specs& color_prompt(const color& color_prompt) {
						color_prompt_ = color_prompt;
						return *this;
					}

					color& color_caret() { return color_caret_; }
					text_field_specs& color_caret(const color& color_caret) {
						color_caret_ = color_caret;
						return *this;
					}

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
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				text_field(containers::page& page);

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
				text_field() = delete;
				text_field(const text_field&) = delete;
				text_field& operator=(const text_field&) = delete;
			};
		}
	}
}
