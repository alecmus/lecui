/*
** time.h - time widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
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

					lecui::time time_value{};

					struct time_events : basic_events {
						/// <summary>Called when the time is changed. The parameter contains the
						/// new time.</summary>
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
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
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
				/// "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the time specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static time_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				time();
				time(const time&);
				time& operator=(const time&);
			};
		}
	}
}
