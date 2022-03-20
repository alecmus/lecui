//
// tree_view.h - tree view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Tree view widget. To create an instance use the <see cref="add"></see> static method.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api tree_view : public widget {
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

					/// <summary>Node constructor.</summary>
					/// <param name="name">The name of the node.</param>
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
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;
				std::map<std::string, node> _root;
				void* _p_special_pane_specs = nullptr;

			public:
				/// <summary>Constructor.</summary>
				tree_view() {
					_rect.size({ 200.f, 200.f });
				}

				/// <summary>Helper for inserting a node.</summary>
				/// <param name="root">A reference to the root.</param>
				/// <param name="name">The name of the node being added.</param>
				/// <returns>A reference to the node that's been added.</returns>
				/// <remarks>Throws on failure.</remarks>
				static node& insert_node(std::map<std::string, node>& root,
					std::string name) {
					root.insert(std::make_pair(name, node(name)));
					return root.at(name);
				}

				/// <summary>Tree view widget events.</summary>
				struct tree_view_events : basic_events {
					/// <summary>Selection event. Happens when the selection changes. The
					/// parameter will contain a reference to the newly selected node.</summary>
					std::function<void(node& n)> selection;
				};

				/// <summary>Get or set tree view widget events.</summary>
				/// <returns>A reference to the tree view widget events.</returns>
				tree_view_events& events() {
					return _tree_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const tree_view& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const tree_view& param);

				/// <summary>Copy tree view specs. Only essential properties are copied.</summary>
				/// <param name="param">The table view specs to copy.</param>
				/// <returns>Returns a reference to the copy.</returns>
				tree_view& operator=(const tree_view& param);

				/// <summary>Copy constructor.</summary>
				/// <param name="param">The table view specs to copy.</param>
				tree_view(const tree_view& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tree_view& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tree_view& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the root of the tree.</summary>
				/// <returns>A reference to the root.</returns>
				/// <remarks>The entire tree is built recursively on this root.</remarks>
				std::map<std::string, node>& root();

				/// <summary>Set the root of the tree.</summary>
				/// <param name="root">The root.</param>
				/// <returns>A reference to the modified object.</returns>
				tree_view& root(const std::map<std::string, node>& root);

			public:
				/// <summary>Add a tree view to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static tree_view& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a tree view.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. admin_page/system_pane/database_browser.</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static tree_view& get(form& fm, const std::string& path);

			private:
				/// <summary>Tree view widget events.</summary>
				tree_view_events _tree_events;

#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting tree view widget. 'Get' method documentation applies.
#define get_tree_view(path) liblec::lecui::widgets::tree_view::get(*this, path)
#endif
