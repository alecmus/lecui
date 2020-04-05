/*
** rectangle.h - rectangle interface
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
				class rectangle : public widget {
				public:
					rectangle() {
						color_fill = { 222, 222, 222, 255 };
						color_hot = { 0, 120, 170, 100 };
						color_selected = { 0, 120, 170, 100 }; }
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					liblec::lecui::color color_border = { 150, 150, 150, 255 };

					bool operator==(const rectangle&);
					bool operator!=(const rectangle&);
				};
			}

			class lecui_api rectangle {
			public:
				rectangle(liblec::lecui::containers::page& page);
				~rectangle();

				liblec::lecui::widgets::specs::rectangle&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::rectangle&
					specs(form& fm, const std::string& name);

			private:
				class rectangle_impl;
				rectangle_impl& d_;

				rectangle();
				rectangle(const rectangle&);
				rectangle& operator=(const rectangle&);
			};
		}
	}
}
