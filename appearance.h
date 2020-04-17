/*
** appearance.h - appearance interface
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
	#include "form.h"
#else
	#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Manipulate the form's appearance.</summary>
		class lecui_api appearance {
		public:
			appearance(form& fm);
			~appearance();

			/// <summary>Whether to add a shadow to the form.</summary>
			/// <param name="enable">Set to false to disable the shadow.</param>
			/// <remarks>The shadow is enabled by default if this method is never called.</remarks>
			void shadow(const bool& enable);

			/// <summary>Whether to enable the dark theme.</summary>
			/// <param name="enable">Set to true to enable the dark theme.</param>
			/// <remarks>This option has to be set early in on_layout(), or in the
			/// constructor. Changing this later is not yet supported.</remarks>
			void dark_theme(const bool& enable);

			/// <summary>Set the theme color.</summary>
			/// <param name="color">The color of controls and the form border.</param>
			/// <param name="color_hot">The color on-hovering over a control.</param>
			/// <param name="color_disabled">The color when a control is disabled.</param>
			/// <param name="color_text">The text color.</param>
			void theme(const lecui::color& color,
				const lecui::color& color_hot,
				const lecui::color& color_disabled,
				const lecui::color& color_text);

			/// <summary>Set the form's background colors.</summary>
			/// <param name="color">The background color of the entire form.</param>
			/// <param name="color_titlebar">The background color of the titlebar.</param>
			void background(const lecui::color& color,
				const lecui::color& color_titlebar);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			appearance();
			appearance(const appearance&);
			appearance& operator=(const appearance&);
		};
	}
}
