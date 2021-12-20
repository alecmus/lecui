//
// date.h - date widget interface
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
			/// <summary>Date widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 110x60px.</remarks>
			class lecui_api date : public widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 2.f;
				float _corner_radius_y = 2.f;
				lecui::date _date_value;
				void* _p_special_pane_specs = nullptr;

			public:
				date() {
					_rect.size({ 90.f, 40.f });
				};

				/// <summary>Events specific to this widget.</summary>
				struct date_events : basic_events {
					/// <summary>Called when the date is changed.</summary>
					/// <remarks>The parameter contains the new date.</remarks>
					std::function<void(const lecui::date&)> change = nullptr;
				};

				date_events& events() {
					return _date_events;
				}

				bool operator==(const date&);
				bool operator!=(const date&);

				// generic widget

				std::string& text() override;
				date& text(const std::string& text);

				std::string& tooltip() override;
				date& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				date& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				date& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				date& cursor(const cursor_type cursor);

				std::string& font() override;
				date& font(const std::string& font);

				float& font_size() override;
				date& font_size(const float& font_size);

				color& color_text() override;
				date& color_text(const color& color_text);

				color& color_fill() override;
				date& color_fill(const color& color_fill);

				color& color_hot() override;
				date& color_hot(const color& color_hot);

				color& color_selected() override;
				date& color_selected(const color& color_selected);

				color& color_disabled() override;
				date& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				date& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the date.</summary>
				/// <returns>A reference to the date property.</returns>
				lecui::date& date_value();

				/// <summary>Set the date.</summary>
				/// <param name="date_value">The date, as defined in <see cref="date"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				widgets::date& date_value(const lecui::date& date_value);

			public:
				/// <summary>Add a date widget to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "last_saved".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static date& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a date.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_pane/last_saved".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static date& get(form& fm, const std::string& path);

			private:
				date_events _date_events;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};
		}
	}
}

/// Helper for getting date widget. Builder documentation applies.
#define get_date(path) liblec::lecui::widgets::date::get(*this, path)
