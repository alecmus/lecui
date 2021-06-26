//
// group.h - group container interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../widgets/widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace containers {
			/// <summary>Group container.</summary>
			class lecui_api group {
			public:
				/// <summary>Group container specifications.</summary>
				/// <remarks>A group does not change the widget path, e.g. A widget located at
				/// "home_page/tab_pane/tab three/pane_two/label" keeps that location even after
				/// being added to a group. Never add a group name to a path. Groups are strictly for
				/// aesthetics and they don't "carry" the widgets that belong to them like other
				/// containers.</remarks>
				class group_specs : public widgets::specs {
					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 5.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 5.f;

					/// <summary>The group's margin. Defines how far we extend outside the collective group rectangles.</summary>
					float margin_ = 5.f;

					/// <summary>A list of the aliases for the widgets to be added to the group. e.g.
					/// { "username_text_field", "password_text_field", "login_button" }.</summary>
					std::vector<std::string> widgets_;

				public:
					group_specs() {
						// rect is ignored. The group dynamically surrounds the widgets associated
						// with it as they change location and size.
						rect_ = { 0.f, 0.f, 0.f, 0.f };
					}

					bool operator==(const group_specs&);
					bool operator!=(const group_specs&);

					// generic specs

					std::string& text() override { return text_; }
					group_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					group_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					group_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					group_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					group_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					group_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					group_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					group_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					group_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					group_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					group_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					group_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					float& border() { return border_; }
					group_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					color& color_border() { return color_border_; }
					group_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					group_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					group_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}

					float& margin() { return margin_; }
					group_specs& margin(const float& margin) {
						margin_ = margin;
						return *this;
					}

					std::vector<std::string>& widgets() { return widgets_; }
					group_specs& widgets(const std::vector<std::string>& widgets) {
						widgets_ = widgets;
						return *this;
					}
				};

				/// <summary>Group constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				group(containers::page& page);

				/// <summary>Group constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				group(containers::page& page, const std::string& alias);
				~group();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				[[nodiscard]]
				group_specs& specs();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				group_specs& operator()();

				/// <summary>Get the specifications of a group.</summary>
				/// <param name="fm">The form containing the group.</param>
				/// <param name="path">The full path to the container, e.g.
				/// "sample_page/left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static group_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				group() = delete;
				group(const group&) = delete;
				group& operator=(const group&) = delete;
			};
		}
	}
}
