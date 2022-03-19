//
// tab_pane.h - tab pane interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../widgets/widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace containers {
#if defined(LECUI_EXPORTS)
			class tab_pane_builder;
			class tab_builder;
#endif

			/// <summary>Tab pane. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			class lecui_api tab_pane : public widgets::widget {
			public:
				/// <summary>The side to place the tabs on.</summary>
				enum class side {
					left,	///< Place the tabs on the left of the tab pane.
					right,	///< Place the tabs on the right of the tab pane.
					top,	///< Place the tabs at the top of the tab pane (default).
					bottom,	///< Place the tabs at the bottom of the tab pane.
				};

				/// <summary>Orientation of tab captions.</summary>
				enum class orientation {
					horizontal,	///< Horizontal orientation.
					vertical,	///< Vertical orientation.
				};

			private:
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;
				side _tab_side = side::top;
				std::vector<std::string> _caption_reserve;
				orientation _caption_orientation = orientation::horizontal;
				color _color_tabs;
				color _color_tabs_border;
				float _tabs_border = .5f;
				std::string _selected;
				containers::page* _p_page = nullptr;

#if defined(LECUI_EXPORTS)
				friend class tab_pane_builder;
				friend class tab_builder;
#endif
			public:
				/// <summary>Class constructor.</summary>
				tab_pane() {}
				
				/// <summary>Check whether tab pane specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const tab_pane& param);

				/// <summary>Check whether tab pane specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const tab_pane& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				/// <remarks>This is only used if the widget is non-static.</remarks>
				cursor_type& cursor() override;

				/// <summary>Se the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				/// <remarks>This property is not used.</remarks>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				tab_pane& color_selected(const color& color_selected);

				/// <summary>Ge tor set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the side on which to place the tab.</summary>
				/// <returns>A reference to the property, as defined in <see cref="side"></see>.</returns>
				side& tab_side();

				/// <summary>Set the side on which to place the tab.</summary>
				/// <param name="tab_side">The property, as defined in <see cref="side"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& tab_side(const side& tab_side);

				/// <summary>Get or set the caption reserve property.</summary>
				/// <returns>A reference to the list of tab captions.</returns>
				/// <remarks>Only affects tab captions whose orientation is perpendicular to
				/// the tab area. Make a reservation for the captions of all the tabs that are
				/// going to be added. If there is no way to know before-hand it is recommended
				/// to insert at least one dummy caption that estimates the size of the longest
				/// expected caption.
				/// This is not required if the tab caption orientation is parallel
				/// to the tab area. If the tab caption orientation is perpendicular, it is
				/// important to set this reservation before adding the first tab to the tab
				/// pane. Failure to do this will result in the thickness of the tab area being
				/// clipped to the size of the first tab's caption. This, however, won't be an
				/// issue if the rest of the tab captions are shorter than the first.</remarks>
				std::vector<std::string>& caption_reserve();

				/// <summary>Set the caption reserve property.</summary>
				/// <param name="caption_reserve">The list of tab captions.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Only affects tab captions whose orientation is perpendicular to
				/// the tab area. Make a reservation for the captions of all the tabs that are
				/// going to be added. If there is no way to know before-hand it is recommended
				/// to insert at least one dummy caption that estimates the size of the longest
				/// expected caption.
				/// This is not required if the tab caption orientation is parallel
				/// to the tab area. If the tab caption orientation is perpendicular, it is
				/// important to set this reservation before adding the first tab to the tab
				/// pane. Failure to do this will result in the thickness of the tab area being
				/// clipped to the size of the first tab's caption. This, however, won't be an
				/// issue if the rest of the tab captions are shorter than the first.</remarks>
				tab_pane& caption_reserve(const std::vector<std::string>& caption_reserve);

				/// <summary>Get or set the orientation of the caption text.</summary>
				/// <returns>A reference to the property, as defined in <see cref="orientation"></see>.</returns>
				orientation& caption_orientation();

				/// <summary>Set the orientation of the caption text.</summary>
				/// <param name="caption_orientation">The caption orientation, as defined in <see cref="orientation"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& caption_orientation(const orientation& caption_orientation);

				/// <summary>Get or set the fill color of the tabs.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tabs();

				/// <summary>Set the fill color of the tabs.</summary>
				/// <param name="color_tabs">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& color_tabs(const color& color_tabs);

				/// <summary>Get or set the color of the tab borders.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tabs_border();

				/// <summary>Set the color of the tab borders.</summary>
				/// <param name="color_tabs_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& color_tabs_border(const color& color_tabs_border);

				/// <summary>Get or set the thickness of the tab borders.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& tabs_border();

				/// <summary>Set the thickness of the tab borders.</summary>
				/// <param name="tabs_border">The thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& tabs_border(const float& tabs_border);

				/// <summary>Get or set the selected tab.</summary>
				/// <returns>A reference to the property.</returns>
				std::string& selected();

				/// <summary>Set the selected tab.</summary>
				/// <param name="selected">The name of the tab.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane& selected(const std::string& selected);

			public:
				/// <summary>Make a tab pane.</summary>
				/// <param name="page">A reference to the container to place the tab pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_pane".</param>
				/// <param name="content_margin">The margin to use inside the tabs.</param>
				/// <returns>A reference to the tab pane.</returns>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab pane leads to undefined behavior.</remarks>
				[[nodiscard]]
				static tab_pane& add(containers::page& page, const std::string& alias = std::string(), const float& content_margin = 10.f);

				/// <summary>Get the specifications of a pane.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the pane, e.g. "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static tab_pane& get(form& fm, const std::string& path);
			};

			/// <summary>Tab container. To instantiate and add to a tab pane use the <see cref="add"></see> static method.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing tab panes within tabs that are themselves in another
			/// tab pane, to virtually any depth level that the memory of the computer the app
			/// is running on can permit.</remarks>
			class lecui_api tab : public containers::page, public widgets::badge_widget {
			public:
				/// <summary>Class constructor.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="tab_name">The tab's name.</param>
				/// <remarks>Never to be called directly. This is called interfally by the library when
				/// the <see cref="add"></see> static method is called.</remarks>
				tab(form& fm, const std::string& tab_name);

				/// <summary>Check whether two tabs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The tab to compare to.</param>
				/// <returns>Returns true if the tabs are equal, else false.</returns>
				bool operator==(const tab& param);

				/// <summary>Check whether two tabs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The tab to compare to.</param>
				/// <returns>Returns true if the tabs are NOT equal, else false.</returns>
				bool operator!=(const tab& param);

				/// <summary>Get or set the badge specs.</summary>
				/// <returns>A reference to the badge specs.</returns>
				widgets::badge_specs& badge() override;

				/// <summary>Set the badge specs.</summary>
				/// <param name="badge">The badge specs.</param>
				/// <returns>A reference to the modified object.</returns>
				tab& badge(const widgets::badge_specs& badge);

				/// <summary>Get the size of the tab's page.</summary>
				/// <returns>The size, in pixels.</returns>
				/// <remarks>The size is automatically determined by the library. It is important
				/// to note that any widgets added to a page, and any other container for that
				/// matter, only see the dimensions and coordinates of that container not those
				/// of the form or another container higher up the hierarchy. Dimensions and
				/// coordinates are local to a container.</remarks>
				[[nodiscard]] const lecui::size size();

				/// <summary>Create a pane.</summary>
				/// <param name="tp">A reference to the tab pane to place the tab in.</param>
				/// <param name="tab_name">The in-page unique alias, e.g. "settings_pane".</param>
				/// <returns>A reference to the tab.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static tab& add(containers::tab_pane& tp, const std::string& tab_name);

				/// <summary>Get the tab container page of an existing tab.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the tab, e.g.
				/// "sample_page/settings_tab_pane/Options".
				/// </param>
				/// <returns>A reference to the tab.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_tab_page).</remarks>
				[[nodiscard]]
				static tab& get(form& fm, const std::string& path);

			private:
				// Default constructor and copying an object of this class are not allowed
				tab() = delete;
				tab(const tab&) = delete;
				tab& operator=(const tab&) = delete;

				friend class tab_builder;
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting tab pane widget. 'Get' method documentation applies.
#define get_tab_pane(path) liblec::lecui::containers::tab_pane::get(*this, path)

/// Helper for getting a tab's page. 'Get' method documentation applies.
#define get_tab(path) liblec::lecui::containers::tab::get(*this, path)
#endif

