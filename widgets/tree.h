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
#if defined(LECUI_EXPORTS)
		namespace widgets_impl {
			class tree;
		}
#endif

		namespace widgets {
			/// <summary>Tree widget.</summary>
			class lecui_api tree {
			public:
				/// <summary>Tree widget specifications.</summary>
				class tree_specs : public specs {
				public:
					tree_specs() {}
					float border = .5f;
					float corner_radius_x = 5.f;
					float corner_radius_y = 5.f;
					color color_border;

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

#if defined(LECUI_EXPORTS)
						friend class widgets_impl::tree;
#endif
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

					struct tree_events : basic_events {
						/// <summary>The handler to be called when the selection changes. The
						/// parameter will contain a reference to the newly selected node.</summary>
						std::function<void(node& n)> selection;
					};

					tree_events& events() {
						return tree_events_;
					}

					bool operator==(const tree_specs&);
					bool operator!=(const tree_specs&);

				private:
					tree_events tree_events_;
				};

				/// <summary>Tree constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				tree(containers::page& page, const std::string& alias);
				~tree();

				/// <summary>Get the tree specifications.</summary>
				/// <returns>A reference to the tree specifications.</returns>
				[[nodiscard]]
				tree_specs& specs();

				/// <summary>Get the tree specifications.</summary>
				/// <returns>A reference to the tree specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				tree_specs& operator()();

				/// <summary>Get the specifications of a tree.</summary>
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
