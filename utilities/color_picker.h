//
// color_picker.h - color picker interface
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
		/// <summary>Color picker.</summary>
		class lecui_api color_picker {
		public:
			/// <summary>Constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			color_picker(form& fm);

			/// <summary>Destructor.</summary>
			~color_picker();

			/// <summary>Pick a color through a color picker dialog.</summary>
			/// <param name="picked_color">The picked color will be written back here.</param>
			/// <returns>Returns true if a color was picked, else false.</returns>
			[[nodiscard]]
			bool pick(color& picked_color);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			color_picker() = delete;
			color_picker(const color_picker&) = delete;
			color_picker& operator=(const color_picker&) = delete;
		};
	}
}
