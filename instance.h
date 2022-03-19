//
// instance.h - instance interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Manage application instances.</summary>
		class lecui_api instance_manager {
		public:
			/// <summary>Class constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			/// <param name="guid">
			/// Unique guid used to prevent multiple instances of the app,
			/// e.g. "Global\\{7DBACCA2-B96E-4D0E-B0F7-933EA9A3C064}".
			/// This same guid is also used to allow a new instance to
			/// (1) Cause a new instance to find and open the existing instance before exiting
			/// (2) Pass it's command line arguments as data to the existing instance. The latter
			/// is received in the existing instance through the form's on_receive_data handler.
			/// </param>
			/// <remarks>It is highly recommended that top level forms within the same app use
			/// the same guid. This way, when a new instance is attempted, the latest form to be
			/// created will be brought to the foreground. GUIDs only apply to top level forms.
			/// </remarks>
			instance_manager(form& fm, const std::string& guid);

			/// <summary>Class destructor.</summary>
			~instance_manager();

			/// <summary>Send data to another application instance.</summary>
			/// <param name="guid">The unique guid of the target application.</param>
			/// <param name="data">The data to send.</param>
			/// <param name="timeout_milliseconds">The timeout of the send operation, in
			/// milliseconds.</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if the operation is successful, else false.</returns>
			bool send_data(const std::string& guid, const std::string& data,
				const long& timeout_milliseconds, std::string& error);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			instance_manager() = delete;
			instance_manager(const instance_manager&) = delete;
			instance_manager& operator=(const instance_manager&) = delete;
		};
	}
}
