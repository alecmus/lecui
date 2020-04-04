/*
** button.h - button interface
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
				class button : public widget {
				public:
					button() {
						color_text = { 0, 0, 0, 255 };
						color_fill = { 229, 229, 229, 255 };
						color_hot = { 240, 240, 240, 255 };
						color_selected = { 20, 80, 140, 100 };
					}
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					liblec::lecui::color color_border = { 150, 150, 150, 255 };

					bool operator==(const button&);
					bool operator!=(const button&);
				};
			}

			class lecui_api button {
			public:
				button(liblec::lecui::containers::page& page);
				~button();

				liblec::lecui::widgets::specs::button&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::button&
					specs(form& fm, const std::string& name);

			private:
				class button_impl;
				button_impl& d_;

				button();
				button(const button&);
				button& operator=(const button&);
			};
		}
	}
}
