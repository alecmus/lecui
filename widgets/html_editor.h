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
			/// <summary>HTML editor widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is a minimum of 320x215px.</remarks>
			class lecui_api html_editor : public widget {
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

				html_editor() {
					_cursor = cursor_type::caret;
					_rect.size({ 320.f, 215.f });
				}

				bool operator==(const html_editor&);
				bool operator!=(const html_editor&);

				// generic widget

				std::string& text() override;
				html_editor& text(const std::string& text);

				std::string& tooltip() override;
				html_editor& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				html_editor& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				html_editor& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				html_editor& cursor(const cursor_type cursor);

				std::string& font() override;
				html_editor& font(const std::string& font);

				float& font_size() override;
				html_editor& font_size(const float& font_size);

				color& color_text() override;
				html_editor& color_text(const color& color_text);

				color& color_fill() override;
				html_editor& color_fill(const color& color_fill);

				color& color_hot() override;
				html_editor& color_hot(const color& color_hot);

				color& color_selected() override;
				html_editor& color_selected(const color& color_selected);

				color& color_disabled() override;
				html_editor& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& border(const float& border);

				/// <summary>Get or set the thickness of the control pane border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& control_border();

				/// <summary>Set the thickness of the control border.</summary>
				/// <param name="control_border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& control_border(const float& control_border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the caret color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& color_caret(const color& color_caret);

				/// <summary>Get or set the color of the html control pane border.</summary>
				/// <returns>A reference to the property.</returns>
				color& color_control_border();

				/// <summary>Set the color of the html control pane border.</summary>
				/// <param name="color_control_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& color_control_border(const color& color_control_border);

				/// <summary>Get or set the fill color of the control.</summary>
				/// <returns>A reference to the property.</returns>
				color& color_control_fill();

				/// <summary>Set the fill color of the control.</summary>
				/// <param name="color_control_fill">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				html_editor& color_control_fill(const color& color_control_fill);

			public:
				/// <summary>Add an html editor to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "email_text".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static html_editor& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of an html editor.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "login_page/credentials_pane/email_text".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static html_editor& get(form& fm, const std::string& path);

			private:
#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};
		}
	}
}

/// Helper for getting html editor widget. Builder documentation applies.
#define get_html_editor(path) liblec::lecui::widgets::html_editor::get(*this, path)
