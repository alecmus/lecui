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
			/// <summary>Button widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 80x25px.</remarks>
			class lecui_api button_specs : public specs {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;

			public:
				button_specs() {
					_rect.size({ 80.f, 25.f });
				}

				bool operator==(const button_specs&);
				bool operator!=(const button_specs&);

				// generic specs

				std::string& text() override;
				button_specs& text(const std::string& text);

				std::string& tooltip() override;
				button_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				button_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				button_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				button_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				button_specs& font(const std::string& font);

				float& font_size() override;
				button_specs& font_size(const float& font_size);

				color& color_text() override;
				button_specs& color_text(const color& color_text);

				color& color_fill() override;
				button_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				button_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				button_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				button_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				button_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				button_specs& corner_radius_y(const float& corner_radius_y);
			};

			/// <summary>Button widget builder.</summary>
			class lecui_api button_builder {
			public:
				/// <summary>Button builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				button_builder(containers::page& page);

				/// <summary>Button builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias of the button, e.g. "next_button".</param>
				button_builder(containers::page& page, const std::string& alias);
				~button_builder();

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
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static button_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				button_builder() = delete;
				button_builder(const button_builder&) = delete;
				button_builder& operator=(const button_builder&) = delete;
			};
		}
	}
}

/// Helper for getting button specs. Builder documentation applies.
#define get_button_specs(path) liblec::lecui::widgets::button_builder::specs(*this, path)
