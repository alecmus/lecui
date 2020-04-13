/*
** group.h - group widget interface
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
			namespace specs {
				/// <summary>Group widget specifications.</summary>
				class group : public widget {
				public:
					group() { color_fill.alpha = 25; }

					color color_border = { 0, 120, 170, 100 };
					float border = .5f;
					float margin = 5.f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					std::vector<std::string> widgets;

					bool operator==(const group&);
					bool operator!=(const group&);
				};
			}

			/// <summary>Group widget.</summary>
			class lecui_api group {
			public:
				group(containers::page& page);
				~group();

				/// <summary>Add a group widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::group&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing group.</summary>
				/// <param name="fm">The form containing the group.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::group&
					specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				group();
				group(const group&);
				group& operator=(const group&);
			};
		}
	}
}
