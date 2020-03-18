/*
** toggle.h - toggle interface
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
			class page;
			namespace specs {
				class toggle : public widget {
				public:
					toggle() { text = "On"; color_fill = { 255, 255, 255, 200 }; }
					std::string text_off = "Off";
					liblec::lecui::color color_on = { 20, 80, 140, 255 };
					liblec::lecui::color color_off = { 150, 150, 150, 255 };
					bool on = false;
					std::function<void(bool)> on_toggle = nullptr;
				};
			}

			class form_api toggle {
			public:
				toggle(liblec::lecui::widgets::page& page);
				~toggle();

				liblec::lecui::widgets::specs::toggle& add(const std::string& name);

			private:
				class toggle_impl;
				toggle_impl& d_;

				toggle();
				toggle(const toggle&);
				toggle& operator=(const toggle&);
			};
		}
	}
}
