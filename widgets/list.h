/*
** list.h - list interface
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

#include <map>

namespace liblec {
	namespace lecui {
		namespace containers {
			class page;
		}

		namespace widgets {
			namespace specs {
				class list : public widget {
				public:
					list() {
						color_fill = { 255, 255, 255, 255 };
						color_hot = { 200, 200, 200, 50 }; }
					liblec::lecui::color color_border = { 150, 150, 150, 255 };
					liblec::lecui::color color_dropdown_hot = { 255, 255, 255, 255 };
					liblec::lecui::color color_menu = { 255, 255, 255, 255 };
					liblec::lecui::color color_grid = { 215, 215, 215, 255 };
					liblec::lecui::color color_text_header = { 0, 0, 0, 255 };
					liblec::lecui::color color_fill_header = { 222, 222, 222, 255 };
					liblec::lecui::color color_fill_alternate = { 0, 120, 170, 15 };
					liblec::lecui::color color_row_hot = { 225, 242, 255, 255 };
					liblec::lecui::color color_row_selected = { 40, 160, 200, 255 };
					float border = .5f;
					float corner_radius_x = 2.f;
					float corner_radius_y = 2.f;
					std::vector<liblec::lecui::list_column> columns;
					std::vector<std::map<std::string, std::string>> table;
					std::vector<long> selected;
					std::function<void(const std::vector<std::map<std::string, std::string>>&)> on_selection = nullptr;

					bool operator==(const list&);
					bool operator!=(const list&);
				};
			}

			class lecui_api list {
			public:
				list(liblec::lecui::containers::page& page);
				~list();

				liblec::lecui::widgets::specs::list&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::list&
					specs(form& fm, const std::string& name);

			private:
				class list_impl;
				list_impl& d_;

				list();
				list(const list&);
				list& operator=(const list&);
			};
		}
	}
}
