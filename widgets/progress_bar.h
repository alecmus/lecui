/*
** progress_bar.h - progress bar widget interface
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
		namespace containers {
			class page;
		}

		namespace widgets {
			/// <summary>Progress bar widget specifications.</summary>
			/// <remarks>Recommended height with defaults is 12px</remarks>
			class progress_bar_specs : public specs {
			public:
				color color_border = { 200, 200, 200, 255 };
				float border = 1.f;
				float percentage = 0.f;

				bool operator==(const progress_bar_specs&);
				bool operator!=(const progress_bar_specs&);
			};

			/// <summary>Progress bar widget.</summary>
			class lecui_api progress_bar {
			public:
				progress_bar(containers::page& page);
				~progress_bar();

				/// <summary>Add a progress bar widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "encryption_status".</param>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				progress_bar_specs& add(const std::string& alias);

				/// <summary>Get the specifications of an existing progress bar.</summary>
				/// <param name="fm">The form containing the progress bar.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/encryption_pane/encryption_status".</param>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static progress_bar_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				progress_bar();
				progress_bar(const progress_bar&);
				progress_bar& operator=(const progress_bar&);
			};
		}
	}
}
