/*
** timer.h - timer interface
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
		/// <summary>Timer management.</summary>
		class lecui_api timer_management {
		public:
			timer_management(form& fm);
			~timer_management();

			/// <summary>Add a timer.</summary>
			/// <param name="alias">The in-form unique alias, e.g. "activation_timer".</param>
			/// <param name="milliseconds">The cycle duration.</param>
			/// <param name="on_timer">The handler that's called after the duration.</param>
			/// <remarks>The timer will keep running until stopped manually, with the handler
			/// getting called repeatedly in a cycle.</remarks>
			void add(const std::string& alias,
				const unsigned long& milliseconds,
				std::function<void()> on_timer);

			/// <summary>Check if a specific timer is running.</summary>
			/// <param name="alias">The timer's unique alias.</param>
			/// <returns>True if the timer is running, else false.</returns>
			bool running(const std::string& alias);

			/// <summary>Stop a specific timer.</summary>
			/// <param name="alias">The timer's unique alias.</param>
			void stop(const std::string& alias);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			timer_management();
			timer_management(const timer_management&);
			timer_management& operator=(const timer_management&);
		};
	}
}
