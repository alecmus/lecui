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
	#include "../form.h"
#else
	#include <liblec/lecui/form.h>
#endif

#include <functional>

namespace liblec {
	namespace lecui {
		namespace widgets {
			namespace specs {
				struct on_resize {
					float perc_x = 0.f;
					float perc_y = 0.f;
					float perc_width = 0.f;
					float perc_height = 0.f;

					float min_x = 0.f;
					float min_y = 0.f;
					float max_x = 0.f;
					float max_y = 0.f;

					float min_width = 0.f;
					float min_height = 0.f;
					float max_width = 0.f;
					float max_height = 0.f;

					bool operator==(const on_resize&);
					bool operator!=(const on_resize&);
				};

				/// <summary>Base class for all widget specifications.</summary>
				class widget {
				public:
					std::string text;
					std::string font = "Segoe UI";
					float font_size = 9.0f;
					rect rect;
					widgets::specs::on_resize resize;
					color color_text = { 0, 0, 0, 255 };
					color color_fill = { 0, 120, 170, 255 };
					color color_hot = { 0, 120, 170, 255 };
					color color_selected = { 0, 120, 170, 30 };
					color color_disabled = { 180, 180, 180, 255 };
					std::function<void()> on_click = nullptr;

					bool operator==(const widget&);
					bool operator!=(const widget&);
				};
			}
		}
	}
}
