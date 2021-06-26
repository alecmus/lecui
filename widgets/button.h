//
// button.h - button widget interface
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
			/// <summary>Button widget.</summary>
			class lecui_api button {
			public:
				/// <summary>Button widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 80x25px.</remarks>
				class button_specs : public specs {
					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 5.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 5.f;

				public:
					button_specs() {
						rect_.size({ 80.f, 25.f });
					}

					bool operator==(const button_specs&);
					bool operator!=(const button_specs&);

					// generic specs

					std::string& text() override { return text_; }
					button_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					button_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					button_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					button_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					button_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					button_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					button_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					button_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					button_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					button_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					button_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					button_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					float& border() { return border_; }
					button_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					color& color_border() { return color_border_; }
					button_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					button_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					button_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}
				};

				/// <summary>Button constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				button(containers::page& page);

				/// <summary>Button constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "next_button".</param>
				button(containers::page& page, const std::string& alias);
				~button();

				/// <summary>Get the button specifications.</summary>
				/// <returns>A reference to the button specifications.</returns>
				[[nodiscard]]
				button_specs& specs();

				/// <summary>Get the button specifications.</summary>
				/// <returns>A reference to the button specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				button_specs& operator()();

				/// <summary>Get the specifications of a button.</summary>
				/// <param name="fm">The form containing the button.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/next_button".</param>
				/// <returns>A reference to the button specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static button_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				button() = delete;
				button(const button&) = delete;
				button& operator=(const button&) = delete;
			};
		}
	}
}
