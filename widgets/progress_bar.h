/*
** progress_bar.h - progress_bar interface
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
				/// <summary>Progress indicator widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 50x50px</remarks>
				class progress_bar : public widget {
				public:
					liblec::lecui::color color_border = { 200, 200, 200, 255 };
					float border = 1.f;
					float percentage = 0.f;

					bool operator==(const progress_bar&);
					bool operator!=(const progress_bar&);
				};
			}

			class lecui_api progress_bar {
			public:
				progress_bar(liblec::lecui::containers::page& page);
				~progress_bar();

				liblec::lecui::widgets::specs::progress_bar&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::progress_bar&
					specs(form& fm, const std::string& name);

			private:
				class progress_indicator_impl;
				progress_indicator_impl& d_;

				progress_bar();
				progress_bar(const progress_bar&);
				progress_bar& operator=(const progress_bar&);
			};
		}
	}
}
