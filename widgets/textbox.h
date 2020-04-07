/*
** textbox.h - textbox interface
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
				/// <summary>Textbox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class textbox : public widget {
				public:
					textbox() {
						color_text = { 0, 0, 0, 255 };
						color_fill = { 255, 255, 255, 255 };
						color_selected = { 0, 120, 170, 100 };
						color_disabled = { 248, 248, 248, 255 };
					}
					std::string prompt = "Enter text here";
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					liblec::lecui::color color_border = { 150, 150, 150, 255 };
					liblec::lecui::color color_prompt = { 180, 180, 180, 255 };
					liblec::lecui::color color_caret = { 50, 50, 50, 255 };

					bool operator==(const textbox&);
					bool operator!=(const textbox&);
				};
			}

			class lecui_api textbox {
			public:
				textbox(liblec::lecui::containers::page& page);
				~textbox();

				liblec::lecui::widgets::specs::textbox&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::textbox&
					specs(form& fm, const std::string& name);

			private:
				class textbox_impl;
				textbox_impl& d_;

				textbox();
				textbox(const textbox&);
				textbox& operator=(const textbox&);
			};
		}
	}
}
