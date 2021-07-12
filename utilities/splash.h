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
			/// <param name="dpi_aware">Whether the splash screen scales the image automatically based on the DPI scale.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false (in which case error information
			/// if written to the "error" parameter.</returns>
			/// <remarks>To use the same image and have it appear the same size on screens with different DPI settings
			/// set the <see cref="dpi_aware"></see> flag to true. However, if you would like to have only a sharp,
			/// pixel-perfect splash screen it is recommended to set the flag to false and (optionally) load a different
			/// resource based on the dpi scale reported through the <see cref="form::get_dpi_scale()"></see> method.
			/// For example, you may have three different images: one for 1.0 up to 1.5 scale, another for 1.75 to 2.25
			/// and then another for 2.50 and beyond.
			/// </remarks>
			bool display(int png_resource, bool dpi_aware, std::string& error);

			/// <summary>Remove splash screen image.</summary>
			void remove();

		private:
			class impl;
			impl& _d;

			// Copying an object of this class are not allowed
			splash(const splash&);
			splash& operator=(const splash&);
		};
	}
}
