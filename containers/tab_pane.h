/*
** tab_pane.h - tab pane interface
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
			class tab;

			/// <summary>Tab pane container.</summary>
			/// <remarks>Only tab containers can be added to this container.</remarks>
			class lecui_api tab_pane {
			public:
				/// <summary>Tab pane specifications.</summary>
				class tab_pane_specs : public widgets::specs {
				public:
					tab_pane_specs() { color_fill = { 100, 100, 100, 15 }; }

					bool is_filled = true;
					color color_tabs = { 0, 120, 170, 15 };
					color color_border = { 0, 120, 170, 50 };
					color color_selected = { 0, 120, 170, 255 };
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
				};

				/// <summary>Tab pane constructor.</summary>
				/// <param name="page">A reference to the container to place the
				/// control in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_pane".
				/// </param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab pane leads to undefined behavior.</remarks>
				tab_pane(containers::page& page, const std::string& alias);
				~tab_pane();

				/// <summary>Get the tab pane specifications.</summary>
				/// <returns>A reference to the control specifications.</returns>
				[[nodiscard]]
				tab_pane_specs& specs();

				/// <summary>Get the specifications of a tab pane.</summary>
				/// <param name="fm">The form the control is in.</param>
				/// <param name="path">The full path to the control, e.g.
				/// "sample_page/settings_tab_pane".</param>
				/// <returns>A reference to the control specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static tab_pane_specs& specs(form& fm, const std::string& path);

				/// <summary>Select the visible tab in the tab pane.</summary>
				/// <param name="tab_name">The in-control unique name of the tab, e.g. "Options".
				/// </param>
				/// <remarks>By default none of the tabs is selected if this method is
				/// never called and the tab pane will load empty. The first selection will
				/// happen when the user clicks a tab, at which point the contents of that tab
				/// will be displayed.</remarks>
				void select(const std::string& tab_name);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tab_pane();
				tab_pane(const tab_pane&);
				tab_pane& operator=(const tab_pane&);

				friend class tab;
			};

			/// <summary>Tab container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing tab panes within tabs that are themselves in another
			/// tab pane, to virtually any depth level that the memory of the computer the app
			/// is running on can permit.</remarks>
			class lecui_api tab {
			public:
				/// <summary>Tab constructor.</summary>
				/// <param name="tp">The tab pane to place it in.</param>
				/// <param name="tab_name">The in-tab_pane unique name of the tab, 
				/// e.g. "Options".</param>
				tab(tab_pane& tp, const std::string& tab_name);
				~tab();

				/// <summary>Get the tab container page.</summary>
				/// <returns>A reference to the tab container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the tab, just
				/// like a regular page.</remarks>
				[[nodiscard]]
				containers::page& get();

				/// <summary>Get the tab container page of an existing tab.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the tab, e.g.
				/// "sample_page/settings_tab_pane/Options".
				/// </param>
				/// <returns>A reference to the tab container page.</returns>
				[[nodiscard]]
				static containers::page& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tab();
				tab(const tab&);
				tab& operator=(const tab&);
			};
		}
	}
}
