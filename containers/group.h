//
// group.h - group container interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			/// <summary>Group container. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>A group does not change the widget path, e.g. A widget located at
			/// "home_page/tab_pane/tab three/pane_two/label" keeps that location even after
			/// being added to a group. Never add a group name to a path. Groups are strictly for
			/// aesthetics and they don't "carry" the widgets that belong to them like other
			/// containers.</remarks>
			class lecui_api group : public widgets::widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;
				float _margin = 5.f;
				std::vector<std::string> _widgets;

			public:

				group() {
					// rect is ignored. The group dynamically surrounds the widgets associated
					// with it as they change location and size.
					_rect = { 0.f, 0.f, 0.f, 0.f };
				}

				bool operator==(const group&);
				bool operator!=(const group&);

				// generic widget

				std::string& text() override;
				group& text(const std::string& text);

				std::string& tooltip() override;
				group& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				group& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				group& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				group& cursor(const cursor_type cursor);

				std::string& font() override;
				group& font(const std::string& font);

				float& font_size() override;
				group& font_size(const float& font_size);

				color& color_text() override;
				group& color_text(const color& color_text);

				color& color_fill() override;
				group& color_fill(const color& color_fill);

				color& color_hot() override;
				group& color_hot(const color& color_hot);

				color& color_selected() override;
				group& color_selected(const color& color_selected);

				color& color_disabled() override;
				group& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				group& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the group's margin. Defines how far we extend outside the collective group rectangles.</summary>
				/// <returns>A reference to the margin, in pixels.</returns>
				float& margin();

				/// <summary>Set the group's margin. Defines how far we extend outside the collective group rectangles.</summary>
				/// <param name="margin">The margin, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group& margin(const float& margin);

				/// <summary>Get or set the list of the aliases for the widgets to be added to the group. e.g.
				/// { "username_text_field", "password_text_field", "login_button" }.</summary>
				/// <returns>A reference to the list of aliases.</returns>
				std::vector<std::string>& widgets();

				/// <summary>Set the list of the aliases for the widgets to be added to the group. e.g.
				/// { "username_text_field", "password_text_field", "login_button" }.</summary>
				/// <param name="widgets">The list of aliases.</param>
				/// <returns>A reference to the modified object.</returns>
				group& widgets(const std::vector<std::string>& widgets);

			public:
				/// <summary>Create a group.</summary>
				/// <param name="page">The container to make the group in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static group& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a group.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the group, e.g. "sample_page/left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static group& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting group widget. Builder documentation applies.
#define get_group(path) liblec::lecui::containers::group::get(*this, path)
