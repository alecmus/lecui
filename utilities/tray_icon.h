//
// tray_icon.h - tray icon interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
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
		struct tray_menu_item {
			/// <summary>The label text of the menu item.</summary>
			std::string label;

			/// <summary>The action handler.</summary>
			std::function<void()> action = nullptr;

			/// <summary>The children of the menu item (used to make a tree).</summary>
			std::vector<tray_menu_item> children;

			/// <summary>The image to use for the menu item (placed on the right).</summary>
			std::string image_file;

			/// <summary>The font to use.</summary>
			std::string font = "Segoe UI";

			/// <summary>The font size.</summary>
			float font_size = 9.f;
		};

		/// <summary>Tray icon class.</summary>
		/// <remarks>Adds a tray icon near the clock on the Windows taskbar. Useful for applications
		/// that are designed to start automatically in the background when the user signs into their profile.</remarks>
		class lecui_api tray_icon {
		public:
			/// <summary>Constructor.</summary>
			/// <param name="fm">The form to which the tray icon belongs.</param>
			tray_icon(form& fm);
			~tray_icon();

			/// <summary>Add the system tray icon.</summary>
			/// <param name="png_resource">The PNG icon resource.</param>
			/// <param name="title">The title of the tray icon; displayed when the mouse is hovered over the tray icon.</param>
			/// <param name="items">The menu items displayed when the tray icon is right clicked, as defined in the
			/// <see cref="tray_menu_item"></see> type.</param>
			/// <param name="default_item">The label of the item whose action is to be invoked when the system tray icon
			/// is clicked (left click). If formatting is used for the actual label this does not have to be formatted
			/// as well because only the plain-text is compared.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if the icon is added successfully, else false.</returns>
			bool add(int png_resource,
				const std::string& title,
				const std::vector<tray_menu_item>& items,
				const std::string& default_item,
				std::string& error);

			/// <summary>Change the tray icon.</summary>
			/// <param name="png_resource">The new PNG icon resource to use for the icon.</param>
			/// <param name="title">The new title of the tray icon. Can be called any number of times after a tray icon is added
			/// through the <see cref="add"></see> method.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if the icon is changed successfully, else false.</returns>
			bool change(int png_resource,
				const std::string& title,
				std::string& error);

			/// <summary>Remove the tray icon.</summary>
			/// <remarks>If this method is never called explicitly it will be called automatically in the destructor. For this reason, the
			/// tray_icon object must persist for the lifetime of the app.</remarks>
			void remove();

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			tray_icon() = delete;
			tray_icon(const tray_icon&) = delete;
			tray_icon& operator=(const tray_icon&) = delete;
		};
	}
}
