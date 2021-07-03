//
// rectangle.h - rectangle widget interface
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
			/// <summary>Rectangle widget specifications.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api rectangle_specs : public specs {
				float border_ = .5f;
				color color_border_;
				color color_border_hot_;
				float corner_radius_x_ = .0f;
				float corner_radius_y_ = .0f;

			public:
				rectangle_specs() {
					rect_.size({ 200.f, 200.f });
				}

				bool operator==(const rectangle_specs&);
				bool operator!=(const rectangle_specs&);

				// generic specs

				std::string& text() override { return text_; }
				rectangle_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				rectangle_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				rectangle_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				rectangle_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				rectangle_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				rectangle_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				rectangle_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				rectangle_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				rectangle_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				rectangle_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				rectangle_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				rectangle_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border() { return border_; }

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the color of the border when the mouse is over the widget.</summary>
				/// <returns>A reference to the color, as defined in <see cref="color"></see>.</returns>
				color& color_border_hot() { return color_border_hot_; }

				/// <summary>Set the color of the border when the mouse is over the widget.</summary>
				/// <param name="color_border">The color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& color_border_hot(const color& color_border_hot) {
					color_border_hot_ = color_border_hot;
					return *this;
				}

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x() { return corner_radius_x_; }

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& corner_radius_x(const float& corner_radius_x) {
					corner_radius_x_ = corner_radius_x;
					return *this;
				}

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y() { return corner_radius_y_; }

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& corner_radius_y(const float& corner_radius_y) {
					corner_radius_y_ = corner_radius_y;
					return *this;
				}
			};

			/// <summary>Rectangle widget builder.</summary>
			class lecui_api rectangle_builder {
			public:
				/// <summary>Rectangle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				rectangle_builder(containers::page& page);

				/// <summary>Rectangle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_rect".</param>
				rectangle_builder(containers::page& page, const std::string& alias);
				~rectangle_builder();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				[[nodiscard]]
				rectangle_specs& specs();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				rectangle_specs& operator()();

				/// <summary>Get the specifications of a rectangle.</summary>
				/// <param name="fm">The form containing the rectangle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_rect".</param>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static rectangle_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				rectangle_builder() = delete;
				rectangle_builder(const rectangle_builder&) = delete;
				rectangle_builder& operator=(const rectangle_builder&) = delete;
			};
		}
	}
}
