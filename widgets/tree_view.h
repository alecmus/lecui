//
// tree_view.h - tree view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

#include <map>

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Tree view widget.</summary>
			class lecui_api tree_view {
			public:
				/// <summary>Tree view widget specifications.</summary>
				/// <remarks>Default size is 200x200px.</remarks>
				class tree_view_specs : public specs {
				public:
					tree_view_specs() {
						rect.size({ 200.f, 200.f });
					}

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
						friend class widgets::tree_view_impl;
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

					struct tree_view_events : basic_events {
						/// <summary>The handler to be called when the selection changes. The
						/// parameter will contain a reference to the newly selected node.</summary>
						std::function<void(node& n)> selection;
					};

					tree_view_events& events() {
						return tree_events_;
					}

					bool operator==(const tree_view_specs&);
					bool operator!=(const tree_view_specs&);

				private:
					tree_view_events tree_events_;
				};

				/// <summary>Tree view constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				tree_view(containers::page& page);

				/// <summary>Tree view constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				tree_view(containers::page& page, const std::string& alias);
				~tree_view();

				/// <summary>Get the tree view specifications.</summary>
				/// <returns>A reference to the tree view specifications.</returns>
				[[nodiscard]]
				tree_view_specs& specs();

				/// <summary>Get the tree view specifications.</summary>
				/// <returns>A reference to the tree view specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				tree_view_specs& operator()();

				/// <summary>Get the specifications of a tree view.</summary>
				/// <param name="fm">The form containing the tree view.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "admin_page/system_pane/database_browser".</param>
				/// <returns>A reference to the tree specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static tree_view_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tree_view() = delete;
				tree_view(const tree_view&) = delete;
				tree_view& operator=(const tree_view&) = delete;
			};
		}
	}
}
