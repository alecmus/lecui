//
// button.h - button widget interface
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
			/// <summary>Button widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 80x25px.</remarks>
			class lecui_api button : public widget, public badge_widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;

			public:
				button() {
					_rect.size({ 80.f, 25.f });
				}

				bool operator==(const button&);
				bool operator!=(const button&);

				// generic widget

				std::string& text() override;
				button& text(const std::string& text);

				std::string& tooltip() override;
				button& tooltip(const std::string& tooltip);

				badge_specs& badge() override;
				button& badge(const badge_specs& badge);

				lecui::rect& rect() override;
				button& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				button& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				button& cursor(const cursor_type cursor);

				std::string& font() override;
				button& font(const std::string& font);

				float& font_size() override;
				button& font_size(const float& font_size);

				color& color_text() override;
				button& color_text(const color& color_text);

				color& color_fill() override;
				button& color_fill(const color& color_fill);

				color& color_hot() override;
				button& color_hot(const color& color_hot);

				color& color_selected() override;
				button& color_selected(const color& color_selected);

				color& color_disabled() override;
				button& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				button& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button& corner_radius_y(const float& corner_radius_y);

			public:
				/// <summary>Add a button to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "next_button".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static button& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a button.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_pane/next_button".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static button& get(form& fm, const std::string& path);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting button widget. 'Get' method documentation applies.
#define get_button(path) liblec::lecui::widgets::button::get(*this, path)
#endif
