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
			/// <summary>Line widget.</summary>
			class lecui_api line {
			public:
				/// <summary>Line widget specifications.</summary>
				class line_specs : public specs {
					/// <summary>The thickness of the line.</summary>
					float thickness_ = 1.f;

					/// <summary>The points, in order, for the line line to go through. These
					/// are relative to the top left corner of the line's bounding rectangle.
					/// </summary>
					std::vector<point> points_;

				public:
					line_specs() {}

					bool operator==(const line_specs&);
					bool operator!=(const line_specs&);

					// generic specs

					std::string& text() override { return text_; }
					line_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					line_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					line_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					line_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					line_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					line_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					line_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					line_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					line_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					line_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					line_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					line_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					float& thickness() { return thickness_; }
					line_specs& thickness(const float& thickness) {
						thickness_ = thickness;
						return *this;
					}

					std::vector<point>& points() { return points_; }
					line_specs& points(const std::vector<point>& points) {
						points_ = points;
						return *this;
					}
				};

				/// <summary>Line constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				line(containers::page& page);

				/// <summary>Line constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_line".</param>
				line(containers::page& page, const std::string& alias);
				~line();

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
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				line() = delete;
				line(const line&) = delete;
				line& operator=(const line&) = delete;
			};
		}
	}
}
