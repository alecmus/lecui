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
				float _border = .5f;
				color _color_border;
				color _color_border_hot;
				float _corner_radius_x = .0f;
				float _corner_radius_y = .0f;

			public:
				rectangle_specs() {
					_rect.size({ 200.f, 200.f });
				}

				bool operator==(const rectangle_specs&);
				bool operator!=(const rectangle_specs&);

				// generic specs

				std::string& text() override;
				rectangle_specs& text(const std::string& text);

				std::string& tooltip() override;
				rectangle_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				rectangle_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				rectangle_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				rectangle_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				rectangle_specs& font(const std::string& font);

				float& font_size() override;
				rectangle_specs& font_size(const float& font_size);

				color& color_text() override;
				rectangle_specs& color_text(const color& color_text);

				color& color_fill() override;
				rectangle_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				rectangle_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				rectangle_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				rectangle_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& color_border(const color& color_border);

				/// <summary>Get or set the color of the border when the mouse is over the widget.</summary>
				/// <returns>A reference to the color, as defined in <see cref="color"></see>.</returns>
				color& color_border_hot();

				/// <summary>Set the color of the border when the mouse is over the widget.</summary>
				/// <param name="color_border">The color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& color_border_hot(const color& color_border_hot);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				rectangle_specs& corner_radius_y(const float& corner_radius_y);
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
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static rectangle_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				rectangle_builder() = delete;
				rectangle_builder(const rectangle_builder&) = delete;
				rectangle_builder& operator=(const rectangle_builder&) = delete;
			};
		}
	}
}

/// Helper for getting rectangle specs. Builder documentation applies.
#define get_rectangle_specs(path) liblec::lecui::widgets::rectangle_builder::specs(*this, path)
