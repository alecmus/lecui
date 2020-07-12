/*
** date.h - date widget interface
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
			/// <summary>Date widget.</summary>
			class lecui_api date {
			public:
				/// <summary>Date widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 110x60px.</remarks>
				class date_specs : public specs {
				public:
					date_specs() {
						rect.size({ 110.f, 60.f });
					};

					lecui::date date_value;

					struct date_events : basic_events {
						/// <summary>Called when the date is changed. The parameter contains the
						/// new date.</summary>
						std::function<void(const lecui::date&)> change = nullptr;
					};

					date_events& events() {
						return date_events_;
					}

					bool operator==(const date_specs&);
					bool operator!=(const date_specs&);

				private:
					date_events date_events_;
				};

				/// <summary>Date constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
				date(containers::page& page, const std::string& alias);
				~date();

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
				/// "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the date specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static date_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				date();
				date(const date&);
				date& operator=(const date&);
			};
		}
	}
}