/*
** widget.h - widget interface
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
	#include "form.h"
#else
	#include <liblec/lecui/form.h>
#endif

#include <functional>

namespace liblec {
	namespace lecui {
		namespace widgets {
			namespace specs {
				struct on_resize {
					long perc_x = 0;
					long perc_y = 0;
					long perc_width = 0;
					long perc_height = 0;

					long min_x = 0;
					long min_y = 0;
					long max_x = 0;
					long max_y = 0;

					long min_width = 0;
					long min_height = 0;
					long max_width = 0;
					long max_height = 0;
				};

				class widget {
				public:
					std::string text;
					std::string font = "Segoe UI";
					float font_size = 9.0f;
					liblec::lecui::rect rect;
					liblec::lecui::widgets::specs::on_resize resize;
					liblec::lecui::color color_text = { 0, 0, 0, 255 };
					liblec::lecui::color color_fill = { 20, 80, 140, 255 };
					liblec::lecui::color color_hot = { 20, 80, 140, 255 };
					liblec::lecui::color color_selected = { 20, 80, 140, 30 };
					liblec::lecui::color color_disabled = { 225, 225, 225, 255 };
					std::function<void()> on_click = nullptr;
				};
			}
		}
	}
}
