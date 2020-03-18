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

#ifdef LECUI_EXPORTS
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			class form_api timer {
			public:
				timer(form& fm);
				~timer();

				void add(const std::string& name,
					const unsigned long& milliseconds,
					std::function<void()> on_timer);
				bool running(const std::string& name);
				void stop(const std::string& name);

			private:
				class timer_impl;
				timer_impl& d_;

				timer();
				timer(const timer&);
				timer& operator=(const timer&);
			};
		}
	}
}
