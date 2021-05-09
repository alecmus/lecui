//
// rectangle.h - rectangle widget interface
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
			/// <summary>Rectangle widget.</summary>
			class lecui_api rectangle {
			public:
				/// <summary>Rectangle widget specifications.</summary>
				/// <remarks>Default size is 200x200px.</remarks>
				class rectangle_specs : public specs {
				public:
					rectangle_specs() {
						rect.size({ 200.f, 200.f });
					}

					/// <summary>The thickness of the border.</summary>
					float border = .5f;

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The color of the border when the mouse is over the widget.</summary>
					color color_border_hot;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = .0f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = .0f;

					bool operator==(const rectangle_specs&);
					bool operator!=(const rectangle_specs&);
				};

				/// <summary>Rectangle constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				rectangle(containers::page& page);

				/// <summary>Rectangle constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_rect".</param>
				rectangle(containers::page& page, const std::string& alias);
				~rectangle();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				[[nodiscard]]
				rectangle_specs& specs();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				rectangle_specs& operator()();

				/// <summary>Get the specifications of a rectangle.</summary>
				/// <param name="fm">The form containing the rectangle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_rect".</param>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static rectangle_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				rectangle() = delete;
				rectangle(const rectangle&) = delete;
				rectangle& operator=(const rectangle&) = delete;
			};
		}
	}
}
