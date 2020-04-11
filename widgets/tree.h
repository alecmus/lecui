/*
** tree.h - tree interface
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

		namespace widgets_implementation {
			class tree;
		}

		namespace widgets {
			namespace specs {
				class tree : public widget {
				public:
					tree() {
						color_text = { 0, 0, 0, 255 };
						color_fill = { 255, 255, 255, 255 };
						color_hot = { 225, 242, 255, 255 };
						color_selected = { 0, 120, 170, 100 };
					}
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					liblec::lecui::color color_border = { 150, 150, 150, 255 };

					class node {
					public:
						std::string name;
						bool expand = false;
						std::map<std::string, node> children;
						node(std::string name) :
							name(name) {}

					private:
						bool selected = false;
						lecui::rect rc = { 0.f, 0.f, 0.f, 0.f };
						lecui::rect rc_expand = { 0.f, 0.f, 0.f, 0.f };

						friend widgets_implementation::tree;
					};

					static node& insert_node(std::map<std::string, node>& parent, std::string name) {
						parent.insert(std::make_pair(name, node(name)));
						return parent.at(name);
					}

					std::map<std::string, node> root;

					std::function<void(node& n)> on_selection;

					bool operator==(const tree&);
					bool operator!=(const tree&);
				};
			}

			class lecui_api tree {
			public:
				tree(liblec::lecui::containers::page& page);
				~tree();

				liblec::lecui::widgets::specs::tree&
					add(const std::string& name);
				static liblec::lecui::widgets::specs::tree&
					specs(form& fm, const std::string& name);

			private:
				class tree_impl;
				tree_impl& d_;

				tree();
				tree(const tree&);
				tree& operator=(const tree&);
			};
		}
	}
}
