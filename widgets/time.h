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
			/// <summary>Time widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 90x40px.</remarks>
			class lecui_api time : public widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 2.f;
				float _corner_radius_y = 2.f;
				lecui::time _time_value{};
				void* _p_special_pane_specs = nullptr;

			public:
				time() {
					_rect.size({ 70.f, 20.f });
				};

				/// <summary>Events specific to this widget.</summary>
				struct time_events : basic_events {
					/// <summary>Called when the time is changed.</summary>
					/// <remarks>The parameter contains the new time.</remarks>
					std::function<void(lecui::time)> change = nullptr;
				};

				time_events& events() {
					return _time_events;
				}

				bool operator==(const time&);
				bool operator!=(const time&);

				// generic widget

				std::string& text() override;
				time& text(const std::string& text);

				std::string& tooltip() override;
				time& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				time& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				time& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				time& cursor(const cursor_type cursor);

				std::string& font() override;
				time& font(const std::string& font);

				float& font_size() override;
				time& font_size(const float& font_size);

				color& color_text() override;
				time& color_text(const color& color_text);

				color& color_fill() override;
				time& color_fill(const color& color_fill);

				color& color_hot() override;
				time& color_hot(const color& color_hot);

				color& color_selected() override;
				time& color_selected(const color& color_selected);

				color& color_disabled() override;
				time& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				time& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				time& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the time.</summary>
				/// <returns>A reference to the time value.</returns>
				lecui::time& time_value();

				/// <summary>Set the time.</summary>
				/// <param name="time_value">The time to set the widget to.</param>
				/// <returns>A reference to the modified object.</returns>
				time& time_value(const lecui::time& time_value);

			public:
				/// <summary>Add a time widget to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "end_time".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static time& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a time widget.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_pane/end_time".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static time& get(form& fm, const std::string& path);

			private:
				time_events _time_events;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};
		}
	}
}

/// Helper for getting time widget. Builder documentation applies.
#define get_time(path) liblec::lecui::widgets::time::get(*this, path)
