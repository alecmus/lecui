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
			/// <summary>Base class for all widget specifications.</summary>
			class specs {
			public:
				/// <summary>Resize parameters.</summary>
				/// <remarks>Affects how the widget behaves when the immediate container's
				/// dimensions change.</remarks>
				struct resize_params {
					/// <summary>The percentage rate for following the container's right border.
					/// 0 = doesn't move horizontally, 100 = moves same number of pixels
					/// horizontally that the containers's right border has moved.
					/// </summary>
					float perc_x = 0.f;

					/// <summary>The percentage rate for following the container's bottom border.
					/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
					/// that the container's bottom border has moved.</summary>
					float perc_y = 0.f;

					/// <summary>The percentage rate for following the container's change in width.
					/// 0 = doesn't follow change in container's width, 100 = width changes at 
					/// same rate as that of container.</summary>
					float perc_width = 0.f;

					/// <summary>The percentage rate for following the container's change in 
					/// height. 0 = doesn't follow change in container's height, 100 = height 
					/// changes at same rate as that of container.</summary>
					float perc_height = 0.f;

					/// Minimum coordinates permitted as widget moves
					float min_x = 0.f;
					float min_y = 0.f;

					/// Maximum coordinates permitted as widget moves
					float max_x = 0.f;
					float max_y = 0.f;

					/// Minimum dimensions permitted as widget shrinks
					float min_width = 0.f;
					float min_height = 0.f;

					/// Maximum dimensions permitted as widget grows
					float max_width = 0.f;
					float max_height = 0.f;

					bool operator==(const resize_params&);
					bool operator!=(const resize_params&);
				};

				/// Content
				std::string text;

				/// Dimensions
				lecui::rect rect;
				resize_params on_resize;

				/// Appearance
				std::string font = "Segoe UI";
				float font_size = 9.0f;
				color color_text = { 0, 0, 0, 255 };
				color color_fill = { 0, 120, 170, 255 };
				color color_hot = { 0, 120, 170, 255 };
				color color_selected = { 0, 120, 170, 30 };
				color color_disabled = { 180, 180, 180, 255 };

				/// Handlers
				std::function<void()> on_click = nullptr;

				bool operator==(const specs&);
				bool operator!=(const specs&);
			};
		}
	}
}
