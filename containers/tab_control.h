/*
** tab_control.h - tab control interface
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
			namespace specs {
				/// <summary>Tab control specifications.</summary>
				class tab_control : public widgets::specs::widget {
				public:
					tab_control() { color_fill = { 100, 100, 100, 15 }; }

					bool is_filled = true;
					color color_tabs = { 0, 120, 170, 15 };
					color color_border = { 0, 120, 170, 50 };
					color color_selected = { 0, 120, 170, 255 };
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
				};
			}

			class tab;

			/// <summary>Tab control container.</summary>
			/// <remarks>Only tab containers can be added to this container.</remarks>
			class lecui_api tab_control {
			public:
				/// <summary>Tab control constructor.</summary>
				/// <param name="page">A reference to the container to place the
				/// control in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_control".
				/// </param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab control leads to undefined behavior.</remarks>
				tab_control(containers::page& page, const std::string& alias);
				~tab_control();

				/// <summary>Get the tab control specifications.</summary>
				/// <returns>A reference to the control specifications.</returns>
				[[nodiscard]] containers::specs::tab_control&
					specs();

				/// <summary>Get the specifications of an existing tab control.</summary>
				/// <param name="fm">The form the control is in.</param>
				/// <param name="path">The full path to the control, e.g.
				/// "sample_page/settings_tab_control".</param>
				/// <returns>A reference to the control specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static containers::specs::tab_control&
					specs(form& fm, const std::string& path);

				/// <summary>Select the visible tab in the tab control.</summary>
				/// <param name="tab_name">The in-control unique name of the tab, e.g. "Options".
				/// </param>
				/// <remarks>By default none of the tabs is selected if this method is
				/// never called and the tab control will load empty. The first selection will
				/// happen when the user clicks a tab, at which point the contents of that tab
				/// will be displayed.</remarks>
				void select(const std::string& tab_name);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tab_control();
				tab_control(const tab_control&);
				tab_control& operator=(const tab_control&);

				friend tab;
			};

			/// <summary>Tab container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing tab controls within tabs that are themselves in another
			/// tab control, to virtually any depth level that the memory of the computer the app
			/// is running on can permit.</remarks>
			class lecui_api tab {
			public:
				tab(tab_control& tc);
				~tab();

				/// <summary>Add a tab container to a tab control.</summary>
				/// <param name="tab_name">The in-control unique name of the tab, e.g. "Options".
				/// </param>
				/// <returns>A reference to the tab container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the tab, just
				/// like a regular page.</remarks>
				[[nodiscard]] containers::page&
					add(const std::string& tab_name);

				/// <summary>Get the tab container page of an existing tab.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the tab, e.g.
				/// "sample_page/settings_tab_control/Options".
				/// </param>
				/// <returns>A reference to the tab container page.</returns>
				[[nodiscard]] static containers::page&
					get(form& fm, const std::string& path);

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
