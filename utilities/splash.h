//
// splash.h - splash screen interface
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
		/// <summary>Splash screen class. Useful for displaying graphics while
		/// app is loading.</summary>
		class lecui_api splash {
		public:
			splash() = delete;

			splash(form& fm);
			~splash();

			/// <summary>Display splash screen image.</summary>
			/// <param name="png_resource">The ID of the PNG resource containing the image.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false (in which case error information
			/// if written to the "error" parameter.</returns>
			bool display(int png_resource, std::string& error);

			/// <summary>Remove splash screen image.</summary>
			void remove();

		private:
			class impl;
			impl& d_;

			// Copying an object of this class are not allowed
			splash(const splash&);
			splash& operator=(const splash&);
		};
	}
}
