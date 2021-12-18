//
// timer.h - timer interface
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
		/// <summary>Timer manager class.</summary>
		class lecui_api timer_manager {
		public:
			/// <summary>Timer manager constructor.</summary>
			/// <param name="fm">The form.</param>
			timer_manager(form& fm);
			~timer_manager();

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
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			timer_manager() = delete;
			timer_manager(const timer_manager&) = delete;
			timer_manager& operator=(const timer_manager&) = delete;
		};
	}
}
