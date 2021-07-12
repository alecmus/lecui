//
// html_editor.h - html editor widget interface
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
			/// <summary>HTML editor widget specifications.</summary>
			/// <remarks>Recommended size with defaults is a minimum of 320x215px.</remarks>
			class lecui_api html_editor_specs : public specs {
				float _border = .5f;
				float _control_border = .5f;
				color _color_border;
				float _corner_radius_x = 3.f;
				float _corner_radius_y = 3.f;
				color _color_caret;
				color _color_control_border;

				/// <summary>The color of the html control pane background.</summary>
				color _color_control_fill;

				void* _p_controls_pane_specs = nullptr;
				void* _p_html_pane_specs = nullptr;
				lecui::rect _rect_widget_overall = { 0, 0, 0, 0 };
				resize_params _on_resize_overall = { 0.f, 0.f, 0.f, 0.f };

			public:
				html_editor_specs() {
					_cursor = cursor_type::caret;
					_rect.size({ 320.f, 215.f });
				}

				bool operator==(const html_editor_specs&);
				bool operator!=(const html_editor_specs&);

				// generic specs

				std::string& text() override;
				html_editor_specs& text(const std::string& text);

				std::string& tooltip() override;
				html_editor_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				html_editor_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				html_editor_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				html_editor_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				html_editor_specs& font(const std::string& font);

				float& font_size() override;
				html_editor_specs& font_size(const float& font_size);

				color& color_text() override;
				html_editor_specs& color_text(const color& color_text);

				color& color_fill() override;
				html_editor_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				html_editor_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				html_editor_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				html_editor_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& border(const float& border);

				/// <summary>Get or set the thickness of the control pane border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& control_border();

				/// <summary>Set the thickness of the control border.</summary>
				/// <param name="control_border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& control_border(const float& control_border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the caret color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& color_caret(const color& color_caret);

				/// <summary>Get or set the color of the html control pane border.</summary>
				/// <returns>A reference to the property.</returns>
				color& color_control_border();

				/// <summary>Set the color of the html control pane border.</summary>
				/// <param name="color_control_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& color_control_border(const color& color_control_border);

				/// <summary>Get or set the fill color of the control.</summary>
				/// <returns>A reference to the property.</returns>
				color& color_control_fill();

				/// <summary>Set the fill color of the control.</summary>
				/// <param name="color_control_fill">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor_specs& color_control_fill(const color& color_control_fill);

			private:
#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};

			/// <summary>HTML editor widget builder.</summary>
			class lecui_api html_editor_builder {
			public:
				/// <summary>HTML editor builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				html_editor_builder(containers::page& page);

				/// <summary>HTML editor builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "email_text".</param>
				html_editor_builder(containers::page& page, const std::string& alias);
				virtual ~html_editor_builder();

				/// <summary>Get the html editor specifications.</summary>
				/// <returns>A reference to the html_editor specifications.</returns>
				[[nodiscard]]
				html_editor_specs& specs();

				/// <summary>Get the html editor specifications.</summary>
				/// <returns>A reference to the html_editor specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				html_editor_specs& operator()();

				/// <summary>Get the specifications of a html editor.</summary>
				/// <param name="fm">The form containing the html editor.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/email_text".</param>
				/// <returns>A reference to the html editor specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static html_editor_specs& specs(form& fm, const std::string& path);

			protected:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				html_editor_builder() = delete;
				html_editor_builder(const html_editor_builder&) = delete;
				html_editor_builder& operator=(const html_editor_builder&) = delete;
			};
		}
	}
}
