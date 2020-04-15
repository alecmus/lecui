/*
** progress_indicator.h - progress indicator widget interface
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
			/// <summary>Progress indicator widget.</summary>
			class lecui_api progress_indicator {
			public:
				/// <summary>Progress indicator widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 50x50px</remarks>
				class progress_indicator_specs : public specs {
				public:
					progress_indicator_specs() {
						color_text = { 0, 120, 170, 255 };
					}
					color color_empty = { 200, 200, 200, 255 };
					float line_thickness_empty = .5f;
					float line_thickness_fill = 1.5f;
					float percentage = 0.f;
					int precision = 0;

					bool operator==(const progress_indicator_specs&);
					bool operator!=(const progress_indicator_specs&);
				};

				/// <summary>Progress indicator constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "file_transfer".</param>
				progress_indicator(containers::page& page, const std::string& alias);
				~progress_indicator();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				[[nodiscard]]
				progress_indicator_specs& specs();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				progress_indicator_specs& operator()();

				/// <summary>Get the specifications of a progress indicator.</summary>
				/// <param name="fm">The form containing the progress indicator.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/files/file_transfer".</param>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static progress_indicator_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				progress_indicator();
				progress_indicator(const progress_indicator&);
				progress_indicator& operator=(const progress_indicator&);
			};
		}
	}
}
