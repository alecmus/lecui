/*
** image.h - image widget interface
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
				/// <summary>Image widget specifications.</summary>
				class image : public widget {
				public:
					image() {
						color_fill = { 222, 222, 222, 0 };
						color_hot = { 0, 120, 170, 100 };
						color_selected = { 0, 120, 170, 100 }; }
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					color color_border = { 150, 150, 150, 0 };
					std::string file;
					int png_resource = 0;

					bool operator==(const image&);
					bool operator!=(const image&);
				};
			}

			/// <summary>Image widget.</summary>
			class lecui_api image {
			public:
				image(containers::page& page);
				~image();

				/// <summary>Add an image widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::image&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing image.</summary>
				/// <param name="fm">The form containing the image.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::image&
					specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				image();
				image(const image&);
				image& operator=(const image&);
			};
		}
	}
}
