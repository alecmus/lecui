//
// line.h - line widget interface
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
			/// <summary>Line widget specifications.</summary>
			class lecui_api line_specs : public specs {
				float _thickness = 1.f;
				std::vector<point> _points;

			public:
				line_specs() {}

				bool operator==(const line_specs&);
				bool operator!=(const line_specs&);

				// generic specs

				std::string& text() override;
				line_specs& text(const std::string& text);

				std::string& tooltip() override;
				line_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				line_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				line_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				line_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				line_specs& font(const std::string& font);

				float& font_size() override;
				line_specs& font_size(const float& font_size);

				color& color_text() override;
				line_specs& color_text(const color& color_text);

				color& color_fill() override;
				line_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				line_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				line_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				line_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the line's thickness.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& thickness();

				/// <summary>Set the line's thickness.</summary>
				/// <param name="thickness">The thickness of the line, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				line_specs& thickness(const float& thickness);

				/// <summary>Get or set the points, in order, for the line line to go through.
				/// These are relative to the top left corner of the line's bounding rectangle.
				/// </summary>
				/// <returns>A reference to the list of points.</returns>
				std::vector<point>& points();

				/// <summary>Set the points, in order, for the line line to go through.
				/// These are relative to the top left corner of the line's bounding rectangle.</summary>
				/// <param name="points">The list of points.</param>
				/// <returns>A reference to the modified object.</returns>
				line_specs& points(const std::vector<point>& points);
			};

			/// <summary>Line widget builder.</summary>
			class lecui_api line_builder {
			public:
				/// <summary>Line builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				line_builder(containers::page& page);

				/// <summary>Line builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_line".</param>
				line_builder(containers::page& page, const std::string& alias);
				~line_builder();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				[[nodiscard]]
				line_specs& specs();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				line_specs& operator()();

				/// <summary>Get the specifications of a line.</summary>
				/// <param name="fm">The form containing the line.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_line".</param>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static line_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				line_builder() = delete;
				line_builder(const line_builder&) = delete;
				line_builder& operator=(const line_builder&) = delete;
			};
		}
	}
}
