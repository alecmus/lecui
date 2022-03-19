//
// filesystem.h - filesystem interface
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
		/// <summary>Interact with the OS file system.</summary>
		class lecui_api filesystem {
		public:
			/// <summary>Constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			filesystem(form& fm);

			/// <summary>Destructor.</summary>
			~filesystem();

			/// <summary>Display a 'select folder' modal dialog.</summary>
			/// <param name="title">The title of the dialog.</param>
			/// <returns>The full path to the selected folder. Empty if none is selected.</returns>
			[[nodiscard]] std::string select_folder(const std::string& title);

			/// <summary>Display an 'open file' modal dialog.</summary>
			/// <param name="params">The parameters of the dialog.</param>
			/// <returns>A list of the full paths to the selected file(s), including the extensions.</returns>
			[[nodiscard]] std::vector<std::string> open_file(const open_file_params& params);

			/// <summary>Display a 'save file' modal dialog so the user can select its properties
			/// and location.</summary>
			/// <param name="file">The suggested name of the file.</param>
			/// <param name="params">The parameters of the dialog.</param>
			/// <returns>The full path the user desires, including the filename and extension.
			/// </returns>
			[[nodiscard]] std::string save_file(const std::string& file,
				const save_file_params& params);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			filesystem() = delete;
			filesystem(const filesystem&) = delete;
			filesystem& operator=(const filesystem&) = delete;
		};
	}
}
