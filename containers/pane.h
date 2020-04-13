/*
** pane.h - pane container interface
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
	#include "../widgets/widget.h"
#else
	#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace containers {
			class page;
		}

		namespace containers {
			namespace specs {
				/// <summary>Pane container specifications.</summary>
				class pane : public widgets::specs::widget {
				public:
					pane() { color_fill = { 100, 100, 100, 15 }; }

					bool is_filled = true;
					color color_border = { 0, 120, 170, 50 };
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
				};
			}

			/// <summary>Pane container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing panes within panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit.</remarks>
			class lecui_api pane {
			public:
				/// <summary>Pane constructor.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_pane".
				/// </param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a pane leads to undefined behavior.</remarks>
				pane(containers::page& page, const std::string& alias);
				~pane();

				/// <summary>Get the pane specifications.</summary>
				/// <returns>A reference to the pane specifications.</returns>
				[[nodiscard]] containers::specs::pane&
					specs();

				/// <summary>Get the specifications of an existing pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the pane, e.g.
				/// "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane specifications.</returns>
				[[nodiscard]] static containers::specs::pane&
					specs(form& fm, const std::string& path);

				/// <summary>Get the pane container page.</summary>
				/// <returns>A reference to the pane container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the pane, just
				/// like a regular page.</remarks>
				[[nodiscard]] containers::page&
					get();

				/// <summary>Get the pane container page of an existing pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the pane, e.g.
				/// "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane container page.</returns>
				[[nodiscard]] static containers::page&
					get(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				pane();
				pane(const pane&);
				pane& operator=(const pane&);
			};
		}
	}
}
