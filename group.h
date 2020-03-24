/*
** group.h - group interface
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
				class group : public widget {
				public:
					group() { color_fill.alpha = 25; }

					bool is_filled = true;
					liblec::lecui::color color_border = { 20, 80, 140, 100 };
					float border = 0.5f;
					long margin = 5;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					std::vector<std::string> widgets;
				};
			}

			class lecui_api group {
			public:
				group(liblec::lecui::containers::page& page);
				~group();

				liblec::lecui::widgets::specs::group& add(const std::string& name);

			private:
				class group_impl;
				group_impl& d_;

				group();
				group(const group&);
				group& operator=(const group&);
			};
		}
	}
}
