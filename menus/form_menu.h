//
// form_menu.h - form menu interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Menu items.</summary>
		struct form_menu_item {
			/// <summary>The label text of the menu item.</summary>
			std::string label;

			/// <summary>The action handler.</summary>
			std::function<void()> action = nullptr;

			/// <summary>The children of the menu item (used to make a tree).</summary>
			std::vector<form_menu_item> children;

			/// <summary>The image resource to use for the menu item (placed on the left).</summary>
			/// <remarks>Takes precedence over <see cref='image_file'></see>.</remarks>
			int image_png_resource = 0;

			/// <summary>The image to use for the menu item (placed on the left).</summary>
			/// <remarks>Used only if <see cref='image_png_resource'></see> is zero.</remarks>
			std::string image_file;

			/// <summary>The font to use.</summary>
			std::string font = "Segoe UI";

			/// <summary>The font size.</summary>
			float font_size = 9.f;
		};

		/// <summary>Form menu.</summary>
		/// <remarks>Menu is added in the title bar to the right of the form caption.</remarks>
		class lecui_api form_menu {
		public:
			form_menu(form& fm);
			~form_menu();

			/// <summary>Add form menu label.</summary>
			/// <param name="text">The label text, e.g. "File".</param>
			/// <param name="tooltip">The (optional) tooltip text.</param>
			/// <param name="items">The items to be displayed when the
			/// label is selected.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			bool add(const std::string& text,
				const std::string& tooltip,
				const std::vector<form_menu_item>& items,
				std::string& error);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			form_menu() = delete;
			form_menu(const form_menu&) = delete;
			form_menu& operator=(const form_menu&) = delete;
		};
	}
}
