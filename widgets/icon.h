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

					/// <summary>Text position in icon.</summary>
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

					/// <summary>The thickness of the border.</summary>
					float border = .5f;

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = 3.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = 3.f;

					/// <summary>ID of PNG resource containing the image.</summary>
					/// <remarks>PNG resource takes precedence over image file.</remarks>
					int png_resource = 0;

					/// <summary>Full path to image file.</summary>
					/// <remarks>PNG resource takes precedence over image file.</remarks>
					std::string file;

					/// <summary>The quality of the image in the icon.</summary>
					image_quality quality = image_quality::high;

					/// <summary>Descriptive text placed under the icon text.</summary>
					std::string description;

					/// <summary>The position of the text.</summary>
					icon_text_position text_position = icon_text_position::right_top;

					bool operator==(const icon_specs&);
					bool operator!=(const icon_specs&);
				};

				/// <summary>Icon constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				icon(containers::page& page);

				/// <summary>Icon constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "users".</param>
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
				/// "sample_page/sample_pane/users".</param>
				/// <returns>A reference to the icon specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static icon_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				icon() = delete;
				icon(const icon&) = delete;
				icon& operator=(const icon&) = delete;
			};
		}
	}
}
