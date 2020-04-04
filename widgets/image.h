/*
** image.h - image interface
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
				class image : public widget {
				public:
					image() {
						color_fill = { 222, 222, 222, 0 };
						color_hot = { 20, 80, 140, 100 };
						color_selected = { 20, 80, 140, 100 }; }
					float border = .5f;
					float corner_radius_x = .0f;
					float corner_radius_y = .0f;
					liblec::lecui::color color_border = { 150, 150, 150, 0 };
					std::string file;
					int png_resource = 0;

					bool operator==(const image&);
					bool operator!=(const image&);
				};
			}

			class lecui_api image {
			public:
				image(liblec::lecui::containers::page& page);
				~image();

				liblec::lecui::widgets::specs::image&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::image&
					specs(form& fm, const std::string& name);

			private:
				class image_impl;
				image_impl& d_;

				image();
				image(const image&);
				image& operator=(const image&);
			};
		}
	}
}
