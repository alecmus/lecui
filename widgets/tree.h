/*
** tree.h - tree widget interface
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

		namespace widgets_impl {
			class tree;
		}

		namespace widgets {
			/// <summary>Tree widget specifications.</summary>
			class tree_specs : public specs {
			public:
				tree_specs() {
					color_text = { 0, 0, 0, 255 };
					color_fill = { 255, 255, 255, 255 };
					color_hot = { 225, 242, 255, 255 };
					color_selected = { 0, 120, 170, 100 };
				}
				float border = .5f;
				float corner_radius_x = 5.f;
				float corner_radius_y = 5.f;
				color color_border = { 150, 150, 150, 255 };

				/// <summary>Tree node.</summary>
				class node {
				public:
					/// <summary>The name of the node.</summary>
					std::string name;

					/// <summary>Whether to show the node's children, if any.</summary>
					bool expand = false;

					/// <summary>The node's children.</summary>
					std::map<std::string, node> children;

					node(std::string name) :
						name(name) {}

				private:
					// Default constructor not allowed
					node() {}

					bool selected = false;
					lecui::rect rc = { 0.f, 0.f, 0.f, 0.f };
					lecui::rect rc_expand = { 0.f, 0.f, 0.f, 0.f };

					friend widgets_impl::tree;
				};

				/// <summary>Helper for inserting a node.</summary>
				/// <param name="parent">A reference to the root.</param>
				/// <param name="name">The name of the node being added.</param>
				/// <returns>A reference to the node that's been added.</returns>
				/// <remarks>Throws on failure.</remarks>
				static node& insert_node(std::map<std::string, node>& root,
					std::string name) {
					root.insert(std::make_pair(name, node(name)));
					return root.at(name);
				}

				/// <summary>The entire tree is built recursively on this root.</summary>
				std::map<std::string, node> root;

				/// <summary>The handler to be called when the selection changes. The
				/// parameter will contain a reference to the newly selected node.</summary>
				std::function<void(node& n)> on_selection;

				bool operator==(const tree_specs&);
				bool operator!=(const tree_specs&);
			};

			/// <summary>Tree widget.</summary>
			class lecui_api tree {
			public:
				tree(containers::page& page);
				~tree();

				/// <summary>Add a tree widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				/// <returns>A reference to the tree specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				tree_specs& add(const std::string& alias);

				/// <summary>Get the specifications of an existing tree.</summary>
				/// <param name="fm">The form containing the tree.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "admin_page/system_pane/database_browser".</param>
				/// <returns>A reference to the tree specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static tree_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tree();
				tree(const tree&);
				tree& operator=(const tree&);
			};
		}
	}
}
