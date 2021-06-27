//
// time.h - time widget interface
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
			/// <summary>Time widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 90x40px.</remarks>
			class time_specs : public specs {
				float border_ = .5f;
				color color_border_;
				float corner_radius_x_ = 2.f;
				float corner_radius_y_ = 2.f;
				lecui::time time_value_{};

			public:
				time_specs() {
					rect_.size({ 90.f, 40.f });
				};

				/// <summary>Events specific to this widget.</summary>
				struct time_events : basic_events {
					/// <summary>Called when the time is changed.</summary>
					/// <remarks>The parameter contains the new time.</remarks>
					std::function<void(lecui::time)> change = nullptr;
				};

				time_events& events() {
					return time_events_;
				}

				bool operator==(const time_specs&);
				bool operator!=(const time_specs&);

				// generic specs

				std::string& text() override { return text_; }
				time_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				time_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				time_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				time_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				time_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				time_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				time_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				time_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				time_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				time_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				time_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				time_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border() { return border_; }

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				time_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x() { return corner_radius_x_; }

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time_specs& corner_radius_x(const float& corner_radius_x) {
					corner_radius_x_ = corner_radius_x;
					return *this;
				}

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y() { return corner_radius_y_; }

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time_specs& corner_radius_y(const float& corner_radius_y) {
					corner_radius_y_ = corner_radius_y;
					return *this;
				}

				/// <summary>Get or set the time.</summary>
				/// <returns>A reference to the time value.</returns>
				lecui::time& time_value() { return time_value_; }

				/// <summary>Set the time.</summary>
				/// <param name="time_value">The time to set the widget to.</param>
				/// <returns>A reference to the modified object.</returns>
				time_specs& time_value(const lecui::time& time_value) {
					time_value_ = time_value;
					return *this;
				}

			private:
				time_events time_events_;
			};

			/// <summary>Time widget builder.</summary>
			class lecui_api time_builder {
			public:
				/// <summary>Time builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				time_builder(containers::page& page);

				/// <summary>Time builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "end_time".</param>
				time_builder(containers::page& page, const std::string& alias);
				~time_builder();

				/// <summary>Get the time specifications.</summary>
				/// <returns>A reference to the time specifications.</returns>
				[[nodiscard]]
				time_specs& specs();

				/// <summary>Get the time specifications.</summary>
				/// <returns>A reference to the time specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				time_specs& operator()();

				/// <summary>Get the specifications of a time.</summary>
				/// <param name="fm">The form containing the time.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/end_time".</param>
				/// <returns>A reference to the time specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static time_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				time_builder() = delete;
				time_builder(const time_builder&) = delete;
				time_builder& operator=(const time_builder&) = delete;
			};
		}
	}
}
