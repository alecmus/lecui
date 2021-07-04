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
			/// <summary>Tree view widget specifications.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api tree_view_specs : public specs {
			public:
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

			private:
				float border_ = .5f;
				color color_border_;
				float corner_radius_x_ = 5.f;
				float corner_radius_y_ = 5.f;
				std::map<std::string, node> root_;
				void* p_special_pane_specs_ = nullptr;

			public:
				tree_view_specs() {
					rect_.size({ 200.f, 200.f });
				}

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
				tree_view_specs& operator=(const tree_view_specs&);
				tree_view_specs(const tree_view_specs&);

				// generic specs

				std::string& text() override;
				tree_view_specs& text(const std::string& text);

				std::string& tooltip() override;
				tree_view_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				tree_view_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				tree_view_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				tree_view_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				tree_view_specs& font(const std::string& font);

				float& font_size() override;
				tree_view_specs& font_size(const float& font_size);

				color& color_text() override;
				tree_view_specs& color_text(const color& color_text);

				color& color_fill() override;
				tree_view_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				tree_view_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				tree_view_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				tree_view_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the root of the tree.</summary>
				/// <returns>A reference to the root.</returns>
				/// <remarks>The entire tree is built recursively on this root.</remarks>
				std::map<std::string, node>& root();

				/// <summary>Set the root of the tree.</summary>
				/// <param name="root">The root.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view_specs& root(const std::map<std::string, node>& root);

			private:
				tree_view_events tree_events_;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};

			/// <summary>Tree view widget builder.</summary>
			class lecui_api tree_view_builder {
			public:
				/// <summary>Tree view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				tree_view_builder(containers::page& page);

				/// <summary>Tree view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				tree_view_builder(containers::page& page, const std::string& alias);
				~tree_view_builder();

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
				tree_view_builder() = delete;
				tree_view_builder(const tree_view_builder&) = delete;
				tree_view_builder& operator=(const tree_view_builder&) = delete;
			};
		}
	}
}
