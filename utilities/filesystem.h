/*
** filesystem.h - filesystem interface
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
		/// <summary>Interact with the OS file system.</summary>
		class lecui_api filesystem {
		public:
			filesystem(form& fm);
			~filesystem();

			/// <summary>Display a 'select folder' modal dialog.</summary>
			/// <param name="title">The title of the dialog.</param>
			/// <returns>The full path to the selected folder. Empty if none is selected.</returns>
			[[nodiscard]] std::string select_folder(const std::string& title);

			/// <summary>Display an 'open file' modal dialog.</summary>
			/// <param name="params">The parameters of the dialog.</param>
			/// <returns>The full path to the selected file, including it's extension.</returns>
			[[nodiscard]] std::string open_file(const open_file_params& params);

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
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			filesystem();
			filesystem(const filesystem&);
			filesystem& operator=(const filesystem&);
		};
	}
}