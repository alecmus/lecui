/*
** checkbox.h - checkbox interface
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
				/// <summary>Checkbox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 20px</remarks>
				class checkbox : public widget {
				public:
					checkbox() {
						color_fill = { 229, 229, 229, 255 };
						color_hot = { 240, 240, 240, 255 };
						color_selected = { 0, 120, 170, 100 };
					}
					std::string text_unchecked;
					std::string text_indeterminate;
					float border = .5f;
					liblec::lecui::color color_check = { 0, 120, 170, 255 };
					liblec::lecui::color color_border = { 150, 150, 150, 255 };

					enum class checkbox_status {
						checked,
						unchecked,
						indeterminate,
					} status = checkbox_status::indeterminate;

					std::function<void(checkbox_status checked)> on_check = nullptr;

					bool operator==(const checkbox&);
					bool operator!=(const checkbox&);
				};
			}

			class lecui_api checkbox {
			public:
				checkbox(liblec::lecui::containers::page& page);
				~checkbox();

				liblec::lecui::widgets::specs::checkbox&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::checkbox&
					specs(form& fm, const std::string& name);

			private:
				class checkbox_impl;
				checkbox_impl& d_;

				checkbox();
				checkbox(const checkbox&);
				checkbox& operator=(const checkbox&);
			};
		}
	}
}
