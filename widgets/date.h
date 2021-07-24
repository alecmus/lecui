//
// date.h - date widget interface
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
			/// <summary>Date widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 110x60px.</remarks>
			class lecui_api date_specs : public specs {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 2.f;
				float _corner_radius_y = 2.f;
				date _date_value;
				void* _p_special_pane_specs = nullptr;

			public:
				date_specs() {
					_rect.size({ 90.f, 40.f });
				};

				/// <summary>Events specific to this widget.</summary>
				struct date_events : basic_events {
					/// <summary>Called when the date is changed.</summary>
					/// <remarks>The parameter contains the new date.</remarks>
					std::function<void(const date&)> change = nullptr;
				};

				date_events& events() {
					return _date_events;
				}

				bool operator==(const date_specs&);
				bool operator!=(const date_specs&);

				// generic specs

				std::string& text() override;
				date_specs& text(const std::string& text);

				std::string& tooltip() override;
				date_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				date_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				date_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				date_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				date_specs& font(const std::string& font);

				float& font_size() override;
				date_specs& font_size(const float& font_size);

				color& color_text() override;
				date_specs& color_text(const color& color_text);

				color& color_fill() override;
				date_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				date_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				date_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				date_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				date_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				date_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the date.</summary>
				/// <returns>A reference to the date property.</returns>
				date& date_value();

				/// <summary>Set the date.</summary>
				/// <param name="date_value">The date, as defined in <see cref="date"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				date_specs& date_value(const date& date_value);

			private:
				date_events _date_events;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};

			/// <summary>Date widget builder.</summary>
			class lecui_api date_builder {
			public:
				/// <summary>Date builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				date_builder(containers::page& page);

				/// <summary>Date builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "last_saved".</param>
				date_builder(containers::page& page, const std::string& alias);
				~date_builder();

				/// <summary>Get the date specifications.</summary>
				/// <returns>A reference to the date specifications.</returns>
				[[nodiscard]]
				date_specs& specs();

				/// <summary>Get the date specifications.</summary>
				/// <returns>A reference to the date specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				date_specs& operator()();

				/// <summary>Get the specifications of a date.</summary>
				/// <param name="fm">The form containing the date.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/last_saved".</param>
				/// <returns>A reference to the date specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static date_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				date_builder() = delete;
				date_builder(const date&) = delete;
				date_builder& operator=(const date&) = delete;
			};
		}
	}
}
