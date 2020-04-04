/*
** combo.h - combo interface
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
		namespace containers {
			class page;
		}

		namespace widgets {
			namespace specs {
				class combo : public widget {
				public:
					combo() {
						color_fill = { 255, 255, 255, 0 };
						color_hot = { 200, 200, 200, 50 }; }
					liblec::lecui::color color_border = { 20, 80, 140, 255 };
					liblec::lecui::color color_dropdown_hot = { 255, 255, 255, 255 };
					liblec::lecui::color color_menu = { 255, 255, 255, 255 };
					liblec::lecui::color color_menu_hot = { 200, 230, 255, 255 };
					liblec::lecui::color color_menu_selected = { 20, 80, 140, 255 };
					float border = .5f;
					float corner_radius_x = 2.f;
					float corner_radius_y = 2.f;
					std::vector<std::string> items;
					std::string selected;
					std::function<void(const std::string&)> on_selection = nullptr;

					bool operator==(const combo&);
					bool operator!=(const combo&);
				};
			}

			class lecui_api combo {
			public:
				combo(liblec::lecui::containers::page& page);
				~combo();

				liblec::lecui::widgets::specs::combo&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::combo&
					specs(form& fm, const std::string& name);

			private:
				class combo_impl;
				combo_impl& d_;

				combo();
				combo(const combo&);
				combo& operator=(const combo&);
			};
		}
	}
}
