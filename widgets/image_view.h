/*
** image_view.h - image view widget interface
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
			/// <summary>Image view widget.</summary>
			class lecui_api image_view {
			public:
				/// <summary>Image view widget specifications.</summary>
				class image_view_specs : public specs {
				public:
					image_view_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 200.f });
					}
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					color color_border;
					std::string file;
					int png_resource = 0;
					image_quality quality = image_quality::medium;
					bool enlarge_if_smaller = false;
					bool keep_aspect_ratio = true;

					bool operator==(const image_view_specs&);
					bool operator!=(const image_view_specs&);
				};

				/// <summary>Image view constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				image_view(containers::page& page);

				/// <summary>Image view constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				image_view(containers::page& page, const std::string& alias);
				~image_view();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				[[nodiscard]]
				image_view_specs& specs();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				image_view_specs& operator()();

				/// <summary>Get the specifications of an image.</summary>
				/// <param name="fm">The form containing the image.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static image_view_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				image_view();
				image_view(const image_view&);
				image_view& operator=(const image_view&);
			};
		}
	}
}
