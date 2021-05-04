/*
** time.h - time widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Time widget.</summary>
			class lecui_api time {
			public:
				/// <summary>Time widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 90x40px.</remarks>
				class time_specs : public specs {
				public:
					time_specs() {
						rect.size({ 90.f, 40.f });
					};

					/// <summary>The thickness of the border.</summary>
					float border = .5f;
					
					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = 2.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = 2.f;

					/// <summary>The time to display.</summary>
					lecui::time time_value{};

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

				private:
					time_events time_events_;
				};

				/// <summary>Time constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				time(containers::page& page);

				/// <summary>Time constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "end_time".</param>
				time(containers::page& page, const std::string& alias);
				~time();

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
				time() = delete;
				time(const time&) = delete;
				time& operator=(const time&) = delete;
			};
		}
	}
}
