//
// pane.h - pane container interface
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
			/// <summary>Pane container specifications.</summary>
			class lecui_api pane_specs : public widgets::widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;

			public:
				pane_specs() {}

				// generic widget

				std::string& text() override;
				pane_specs& text(const std::string& text);

				std::string& tooltip() override;
				pane_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				pane_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				pane_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				pane_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				pane_specs& font(const std::string& font);

				float& font_size() override;
				pane_specs& font_size(const float& font_size);

				color& color_text() override;
				pane_specs& color_text(const color& color_text);

				color& color_fill() override;
				pane_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				pane_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				pane_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				pane_specs& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& corner_radius_y(const float& corner_radius_y);
			};

			/// <summary>Pane container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing panes within panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit.</remarks>
			class lecui_api pane :
				public pane_specs, public containers::page {
			public:
				/// <summary>Pane constructor.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="alias">The in-page unique alias of the pane, e.g. "settings_pane".</param>
				pane(form& fm, const std::string& alias);

				/// <summary>Get the size of the pane's page.</summary>
				/// <returns>The size, in pixels.</returns>
				/// <remarks>The size is automatically determined by the library. It is important
				/// to note that any widgets added to a page, and any other container for that
				/// matter, only see the dimensions and coordinates of that container not those
				/// of the form or another container higher up the hierarchy. Dimensions and
				/// coordinates are local to a container.</remarks>
				[[nodiscard]] const lecui::size size() override;

				/// <summary>Make a pane.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_pane".</param>
				/// <returns>A reference to the pane specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static pane& add(containers::page& page, const std::string& alias = std::string(), const float& content_margin = 10.f);

				/// <summary>Get the specifications of a pane.</summary>
				/// <param name="fm"></param>
				/// <param name="path">The full path to the pane, e.g. "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static pane& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting pane widget. Builder documentation applies.
#define get_pane(path) liblec::lecui::containers::pane::get(*this, path)
