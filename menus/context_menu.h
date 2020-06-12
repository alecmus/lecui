/*
** context_menu.h - context menu interface
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
#include "../form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Menu items.</summary>
        struct menu_item {
			/// <summary>The label text of the menu item.</summary>
            std::string label;

			/// <summary>The image to use for the menu item (placed on the right).</summary>
            std::string image_file;

			/// <summary>The children of the menu item (used to make a tree).</summary>
            std::vector<menu_item> children;
        };

		class lecui_api context_menu {
		public:
			/// <summary>Pin type. Used to determine the position of the context menu with
			/// respect to the pin rectangle.</summary>
			enum class pin_type {
				/// <summary>If there is enough room below the pin rectangle, or if the area below
				/// is larger than the area above, the menu will be pinned at the bottom, otherwise
				/// it will be pinned at the top. The width of the menu will be equal to the width
				/// of the pin rectangle.</summary>
				bottom,

				/// <summary>If there is enough room on the right of the pin rectangle, or if the
				/// area on the right is larger than the area on the left, the menu will be pinned
				/// to the right, otherwise it will be pinned to the left.</summary>
				right,
			};

			/// <summary>The context menu specifications. By default the menu is pinned to the
			/// top left corner of the mouse cursor.</summary>
			struct specs {
				/// <summary>The menu items.</summary>
				std::vector<menu_item> items;

				/// <summary>The rectangle to pin the menu to. Leave with a size of zero to rather
				/// pin the menu to the mouse cursor (default behavior).</summary>
				lecui::rect pin = { 0.f, 0.f, 0.f, 0.f };

				/// <summary>The pin type to use, if the pin rectangle is used (i.e. if it's
				/// non-zero size).</summary>
				pin_type type = pin_type::bottom;
			};

			/// <summary>Context menu constructor.</summary>
			context_menu();

			/// <summary>Display a context menu.</summary>
			/// <param name="fm">The form to which the menu belongs.</param>
			/// <param name="menu_specs">The menu specifications.</param>
			/// <returns>The selected menu item, if any. If the menu item is a child, the full
			/// path to the item is returned, e.g. "fruit/citrus/orange".</returns>
			std::string operator() (form& fm, const specs& menu_specs);

		private:
			class impl;

			// Copying an object of this class are not allowed
			context_menu(const context_menu&);
			context_menu& operator=(const context_menu&);
		};
	}
}
