/*
** icon.h - icon widget interface
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
			/// <summary>Icon widget.</summary>
			class lecui_api icon {
			public:
				/// <summary>Icon widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 180x78px.</remarks>
				class icon_specs : public specs {
				public:
					icon_specs() {
						rect.size({ 180.f, 78.f });
					};

					enum class icon_text_position {
						bottom,
						top,
						right,
						right_top,
						right_bottom,
						left,
						left_top,
						left_bottom,
					};

					std::string file;
					int png_resource = 0;
					std::string description;
					icon_text_position text_position = icon_text_position::right_top;

					bool operator==(const icon_specs&);
					bool operator!=(const icon_specs&);
				};

				/// <summary>Icon constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
				icon(containers::page& page, const std::string& alias);
				~icon();

				/// <summary>Get the icon specifications.</summary>
				/// <returns>A reference to the icon specifications.</returns>
				[[nodiscard]]
				icon_specs& specs();

				/// <summary>Get the icon specifications.</summary>
				/// <returns>A reference to the icon specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				icon_specs& operator()();

				/// <summary>Get the specifications of a icon.</summary>
				/// <param name="fm">The form containing the icon.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the icon specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static icon_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				icon();
				icon(const icon&);
				icon& operator=(const icon&);
			};
		}
	}
}
