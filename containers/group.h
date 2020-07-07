/*
** group.h - group container interface
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
			/// <summary>Group container.</summary>
			class lecui_api group {
			public:
				/// <summary>Group container specifications.</summary>
				/// <remarks>A group does not change the widget path, e.g. A widget located at
				/// "home_page/tab_pane/tab three/pane_two/label" keeps that location even after
				/// being added to a group. Never add a group name to a path. Groups are strictly for
				/// aesthetics and they don't "carry" the widgets that belong to them like other
				/// containers.</remarks>
				class group_specs : public widgets::specs {
				public:
					group_specs() {
						// rect is ignored. The group dynamically surrounds the widgets associated
						// with it as they change location and size.
						rect = { 0.f, 0.f, 0.f, 0.f };
					}

					color color_border;
					float border = .5f;
					float margin = 5.f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;

					/// <summary>A list of the aliases for the widgets to be added to the group. e.g.
					/// { "username_textbox", "password_textbox", "login_button" }.</summary>
					std::vector<std::string> widgets;

					bool operator==(const group_specs&);
					bool operator!=(const group_specs&);
				};

				/// <summary>Group constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				group(containers::page& page, const std::string& alias);
				~group();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				[[nodiscard]]
				group_specs& specs();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				group_specs& operator()();

				/// <summary>Get the specifications of a group.</summary>
				/// <param name="fm">The form containing the group.</param>
				/// <param name="path">The full path to the container, e.g.
				/// "sample_page/left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static group_specs& specs(form& fm, const std::string& path);

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
