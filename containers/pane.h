//
// pane.h - pane container interface
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
			/// <summary>Pane container specifications.</summary>
			class lecui_api pane_specs : public widgets::widget, public widgets::badge_widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 5.f;
				float _corner_radius_y = 5.f;

			public:
				pane_specs() {}

				/// <summary>Check whether pane specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const pane_specs& param);

				/// <summary>Check whether pane specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const pane_specs& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& tooltip(const std::string& tooltip);

				/// <summary>Get or set the badge specs.</summary>
				/// <returns>A reference to the badge specs.</returns>
				widgets::badge_specs& badge() override;

				/// <summary>Set the badge specs.</summary>
				/// <param name="badge">The badge specs.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& badge(const widgets::badge_specs& badge);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				/// <remarks>This property is not used.</remarks>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				/// <remarks>This property is not used.</remarks>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				/// <remarks>This property is not used.</remarks>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				/// <remarks>This property is not used.</remarks>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>This property is not used.</remarks>
				pane_specs& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				pane_specs& corner_radius_y(const float& corner_radius_y);
			};

			/// <summary>Pane container. To create an instance use the <see cref="add"></see> static method.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing panes within panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit.</remarks>
			class lecui_api pane :
				public pane_specs, public containers::page {
			public:
				/// <summary>Class constructor.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="alias">The pane's unique alias.</param>
				/// /// <remarks>Never to be called directly. This is called interfally by the library when
				/// the <see cref="add"></see> static method is called.</remarks>
				pane(form& fm, const std::string& alias);

				/// <summary>Get the size of the pane's page.</summary>
				/// <returns>The size, in pixels.</returns>
				/// <remarks>The size is automatically determined by the library. It is important
				/// to note that any widgets added to a page, and any other container for that
				/// matter, only see the dimensions and coordinates of that container not those
				/// of the form or another container higher up the hierarchy. Dimensions and
				/// coordinates are local to a container.</remarks>
				[[nodiscard]] const lecui::size size() override;

				/// <summary>Make a pane.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_pane".</param>
				/// <param name="content_margin">The margin to use inside the pane.</param>
				/// <returns>A reference to the pane.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static pane& add(containers::page& page, const std::string& alias = std::string(), const float& content_margin = 10.f);

				/// <summary>Get the specifications of a pane.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the pane, e.g. "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static pane& get(form& fm, const std::string& path);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting pane widget. 'Get' method documentation applies.
#define get_pane(path) liblec::lecui::containers::pane::get(*this, path)
#endif
