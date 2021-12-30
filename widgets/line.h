//
// line.h - line widget interface
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
			/// <summary>Line widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			class lecui_api line : public widget {
				float _thickness = 1.f;
				std::vector<point> _points;

			public:
				line() {}

				bool operator==(const line&);
				bool operator!=(const line&);

				// generic widget

				std::string& text() override;
				line& text(const std::string& text);

				std::string& tooltip() override;
				line& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				line& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				line& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				line& cursor(const cursor_type cursor);

				std::string& font() override;
				line& font(const std::string& font);

				float& font_size() override;
				line& font_size(const float& font_size);

				color& color_text() override;
				line& color_text(const color& color_text);

				color& color_fill() override;
				line& color_fill(const color& color_fill);

				color& color_hot() override;
				line& color_hot(const color& color_hot);

				color& color_selected() override;
				line& color_selected(const color& color_selected);

				color& color_disabled() override;
				line& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the line's thickness.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& thickness();

				/// <summary>Set the line's thickness.</summary>
				/// <param name="thickness">The thickness of the line, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				line& thickness(const float& thickness);

				/// <summary>Get or set the points, in order, for the line to go through.
				/// These are relative to the top left corner of the line's bounding rectangle.
				/// </summary>
				/// <returns>A reference to the list of points.</returns>
				std::vector<point>& points();

				/// <summary>Set the points, in order, for the line line to go through.
				/// These are relative to the top left corner of the line's bounding rectangle.</summary>
				/// <param name="points">The list of points.</param>
				/// <returns>A reference to the modified object.</returns>
				line& points(const std::vector<point>& points);

			public:
				/// <summary>Add a line widget to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_line".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static line& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a line.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_three/border_line".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static line& get(form& fm, const std::string& path);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting line widget. 'Get' method documentation applies.
#define get_line(path) liblec::lecui::widgets::line::get(*this, path)
#endif
