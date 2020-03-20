/*
** tab_control.h - tab control interface
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
	#include "widget.h"
#else
	#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			class page;
			namespace specs {
				class tab_control : public widget {
				public:
					tab_control() { color_fill = { 0, 0, 0, 15 }; }

					bool is_filled = true;
					liblec::lecui::color color_tabs = { 20, 80, 140, 15 };
					liblec::lecui::color color_border = { 20, 80, 140, 100 };
					liblec::lecui::color color_selected = { 20, 80, 140, 255 };
					float border = 0.5f;
					long margin = 5;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
				};
			}

			class tab;

			class lecui_api tab_control {
			public:
				tab_control(liblec::lecui::widgets::page& page, const std::string& name);
				~tab_control();

				liblec::lecui::widgets::specs::tab_control& specs();
				void select(const std::string& name);

			private:
				class tab_control_impl;
				tab_control_impl& d_;

				tab_control();
				tab_control(const tab_control&);
				tab_control& operator=(const tab_control&);

				friend tab;
			};

			class lecui_api tab {
			public:
				tab(tab_control& tc);
				~tab();

				liblec::lecui::widgets::page& add(const std::string& name);

			private:
				class tab_impl;
				tab_impl& d_;

				tab();
				tab(const tab&);
				tab& operator=(const tab&);
			};
		}
	}
}
