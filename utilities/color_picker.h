/*
** color_picker.h - color picker interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
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
		/// <summary>Color picker.</summary>
		class lecui_api color_picker {
		public:
			color_picker(form& fm);
			~color_picker();

			/// <summary>Pick a color through a color picker dialog.</summary>
			/// <param name="picked_color">The picked color will be written back here.</param>
			/// <returns>Returns true if a color was picked, else false.</returns>
			[[nodiscard]]
			bool pick(color& picked_color);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			color_picker();
			color_picker(const color_picker&);
			color_picker& operator=(const color_picker&);
		};
	}
}
