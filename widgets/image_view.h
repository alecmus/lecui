//
// image_view.h - image view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

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
				/// <remarks>Default size is 200x200px.</remarks>
				class image_view_specs : public specs {
				public:
					image_view_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 200.f });
					}

					/// <summary>The thickness of the border.</summary>
					float border = .5f;

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = .0f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = .0f;

					/// <summary>ID of PNG resource containing the image.</summary>
					/// <remarks>PNG resource takes precedence over image file.</remarks>
					int png_resource = 0;

					/// <summary>Full path to image file.</summary>
					/// <remarks>PNG resource takes precedence over image file.</remarks>
					std::string file;

					/// <summary>The quality of the image.</summary>
					/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
					image_quality quality = image_quality::medium;

					/// <summary>Whether to enlarge the image to fill up the rect if the image
					/// is smaller than the rectangle.</summary>
					bool enlarge_if_smaller = false;

					/// <summary>Whether to keep the image aspect ratio.</summary>
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
				image_view() = delete;
				image_view(const image_view&) = delete;
				image_view& operator=(const image_view&) = delete;
			};
		}
	}
}
