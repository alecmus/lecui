/*
** rectangle.h - rectangle widget interface
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
				/// <summary>Rectangle widget specifications.</summary>
				class rectangle : public widget {
				public:
					rectangle() {
						color_fill = { 222, 222, 222, 255 };
						color_hot = { 0, 120, 170, 100 };
						color_selected = { 0, 120, 170, 100 }; }
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					color color_border = { 150, 150, 150, 255 };

					bool operator==(const rectangle&);
					bool operator!=(const rectangle&);
				};
			}

			/// <summary>Rectangle widget.</summary>
			class lecui_api rectangle {
			public:
				rectangle(containers::page& page);
				~rectangle();

				/// <summary>Add a rectangle widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "border_rect".</param>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::rectangle&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing rectangle.</summary>
				/// <param name="fm">The form containing the rectangle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_control/tab_three/border_rect".</param>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::rectangle&
					specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				rectangle();
				rectangle(const rectangle&);
				rectangle& operator=(const rectangle&);
			};
		}
	}
}
