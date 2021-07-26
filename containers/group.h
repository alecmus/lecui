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
			/// <summary>Group container specifications.</summary>
			/// <remarks>A group does not change the widget path, e.g. A widget located at
			/// "home_page/tab_pane/tab three/pane_two/label" keeps that location even after
			/// being added to a group. Never add a group name to a path. Groups are strictly for
			/// aesthetics and they don't "carry" the widgets that belong to them like other
			/// containers.</remarks>
			class lecui_api group_specs : public widgets::specs {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;
				float _margin = 5.f;
				std::vector<std::string> _widgets;

			public:
				group_specs() {
					// rect is ignored. The group dynamically surrounds the widgets associated
					// with it as they change location and size.
					_rect = { 0.f, 0.f, 0.f, 0.f };
				}

				bool operator==(const group_specs&);
				bool operator!=(const group_specs&);

				// generic specs

				std::string& text() override;
				group_specs& text(const std::string& text);

				std::string& tooltip() override;
				group_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				group_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				group_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				group_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				group_specs& font(const std::string& font);

				float& font_size() override;
				group_specs& font_size(const float& font_size);

				color& color_text() override;
				group_specs& color_text(const color& color_text);

				color& color_fill() override;
				group_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				group_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				group_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				group_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the group's margin. Defines how far we extend outside the collective group rectangles.</summary>
				/// <returns>A reference to the margin, in pixels.</returns>
				float& margin();

				/// <summary>Set the group's margin. Defines how far we extend outside the collective group rectangles.</summary>
				/// <param name="margin">The margin, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& margin(const float& margin);

				/// <summary>Get or set the list of the aliases for the widgets to be added to the group. e.g.
				/// { "username_text_field", "password_text_field", "login_button" }.</summary>
				/// <returns>A reference to the list of aliases.</returns>
				std::vector<std::string>& widgets();

				/// <summary>Set the list of the aliases for the widgets to be added to the group. e.g.
				/// { "username_text_field", "password_text_field", "login_button" }.</summary>
				/// <param name="widgets">The list of aliases.</param>
				/// <returns>A reference to the modified object.</returns>
				group_specs& widgets(const std::vector<std::string>& widgets);
			};

			/// <summary>Group container builder.</summary>
			class lecui_api group_builder {
			public:
				/// <summary>Group builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				group_builder(containers::page& page);

				/// <summary>Group builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				group_builder(containers::page& page, const std::string& alias);
				~group_builder();

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
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static group_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				group_builder() = delete;
				group_builder(const group_builder&) = delete;
				group_builder& operator=(const group_builder&) = delete;
			};
		}
	}
}

/// Helper for getting group specs. Builder documentation applies.
#define get_group_specs(path) liblec::lecui::containers::group_builder::specs(*this, path)
