/*
** pane.h - pane interface
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
				class pane : public widget {
				public:
					pane() { color_fill = { 100, 100, 100, 15 }; }

					bool is_filled = true;
					liblec::lecui::color color_border = { 20, 80, 140, 50 };
					float border = 0.5f;
					long margin = 5;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
				};
			}

			class lecui_api pane {
			public:
				pane(liblec::lecui::widgets::page& page, const std::string& name);
				~pane();

				liblec::lecui::widgets::specs::pane& specs();
				liblec::lecui::widgets::page& get();

			private:
				class pane_impl;
				pane_impl& d_;

				pane();
				pane(const pane&);
				pane& operator=(const pane&);
			};
		}
	}
}
