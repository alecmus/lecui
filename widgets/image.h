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
		namespace widgets {
			/// <summary>Image widget.</summary>
			class lecui_api image {
			public:
				/// <summary>Image widget specifications.</summary>
				class image_specs : public specs {
				public:
					image_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 200.f });
					}
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					color color_border;
					std::string file;
					int png_resource = 0;

					bool operator==(const image_specs&);
					bool operator!=(const image_specs&);
				};

				/// <summary>Image constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				image(containers::page& page);

				/// <summary>Image constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				image(containers::page& page, const std::string& alias);
				~image();

				/// <summary>Get the specifications of the image.</summary>
				/// <returns>A reference to the image specifications.</returns>
				[[nodiscard]]
				image_specs& specs();

				/// <summary>Get the specifications of the image.</summary>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				image_specs& operator()();

				/// <summary>Get the specifications of an image.</summary>
				/// <param name="fm">The form containing the image.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static image_specs& specs(form& fm, const std::string& path);

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
