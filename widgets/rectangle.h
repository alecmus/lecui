//
// rectangle.h - rectangle widget interface
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
			/// <summary>Rectangle widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api rectangle : public widget {
				float _border = .5f;
				color _color_border;
				color _color_border_hot;
				float _corner_radius_x = .0f;
				float _corner_radius_y = .0f;

			public:
				rectangle() {
					_rect.size({ 200.f, 200.f });
				}

				bool operator==(const rectangle&);
				bool operator!=(const rectangle&);

				// generic widget

				std::string& text() override;
				rectangle& text(const std::string& text);

				std::string& tooltip() override;
				rectangle& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				rectangle& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				rectangle& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				rectangle& cursor(const cursor_type cursor);

				std::string& font() override;
				rectangle& font(const std::string& font);

				float& font_size() override;
				rectangle& font_size(const float& font_size);

				color& color_text() override;
				rectangle& color_text(const color& color_text);

				color& color_fill() override;
				rectangle& color_fill(const color& color_fill);

				color& color_hot() override;
				rectangle& color_hot(const color& color_hot);

				color& color_selected() override;
				rectangle& color_selected(const color& color_selected);

				color& color_disabled() override;
				rectangle& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle& color_border(const color& color_border);

				/// <summary>Get or set the color of the border when the mouse is over the widget.</summary>
				/// <returns>A reference to the color, as defined in <see cref="color"></see>.</returns>
				color& color_border_hot();

				/// <summary>Set the color of the border when the mouse is over the widget.</summary>
				/// <param name="color_border_hot">The color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle& color_border_hot(const color& color_border_hot);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle& corner_radius_y(const float& corner_radius_y);

			public:
				/// <summary>Add a rectangle to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_rect".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static rectangle& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a rectangle.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_three/border_rect".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static rectangle& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting rectangle widget. Builder documentation applies.
#define get_rectangle(path) liblec::lecui::widgets::rectangle::get(*this, path)
