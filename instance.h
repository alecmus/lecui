/*
** instance.h - instance interface
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
		/// <summary>Manage application instances.</summary>
		class lecui_api instance_management {
		public:
			/// <summary>Constructor</summary>
			/// <param name="fm">A reference to the form.</param>
			/// <param name="guid">
			/// Unique guid used to prevent multiple instances of the app,
			/// e.g. "Global\\{7DBACCA2-B96E-4D0E-B0F7-933EA9A3C064}".
			/// This same guid is also used to allow a new instance to
			/// (1) Cause a new instance to find and open the existing instance before exiting
			/// (2) Pass it's command line arguments as data to the existing instance. The latter
			/// is received in the existing instance through the form's on_receive_data handler.
			/// </param>
			instance_management(form& fm, const std::string& guid);
			~instance_management();

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			instance_management() = delete;
			instance_management(const instance_management&) = delete;
			instance_management& operator=(const instance_management&) = delete;
		};
	}
}
