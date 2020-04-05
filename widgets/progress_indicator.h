/*
** progress_indicator.h - progress_indicator interface
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
				class progress_indicator : public widget {
				public:
					progress_indicator() {
						color_text = { 20, 80, 140, 255 };
					}
					liblec::lecui::color color_empty = { 180, 180, 180, 255 };
					float line_thickness_empty = .5f;
					float line_thickness_fill = 1.f;
					float percentage = 25.f;
					int precision = 0;

					bool operator==(const progress_indicator&);
					bool operator!=(const progress_indicator&);
				};
			}

			class lecui_api progress_indicator {
			public:
				progress_indicator(liblec::lecui::containers::page& page);
				~progress_indicator();

				liblec::lecui::widgets::specs::progress_indicator&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::progress_indicator&
					specs(form& fm, const std::string& name);

			private:
				class progress_indicator_impl;
				progress_indicator_impl& d_;

				progress_indicator();
				progress_indicator(const progress_indicator&);
				progress_indicator& operator=(const progress_indicator&);
			};
		}
	}
}
