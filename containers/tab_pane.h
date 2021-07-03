//
// tab_pane.h - tab pane interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
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
			class tab_builder;

			/// <summary>Tab pane specifications.</summary>
			class lecui_api tab_pane_specs : public widgets::specs {
			public:
				enum class side {
					left,
					right,
					top,
					bottom,
				};

				enum class orientation {
					horizontal,
					vertical,
				};

			private:
				float border_ = .5f;
				color color_border_;
				float corner_radius_x_ = 5.f;
				float corner_radius_y_ = 5.f;
				side tab_side_ = side::top;
				std::vector<std::string> caption_reserve_;
				orientation caption_orientation_ = orientation::horizontal;
				color color_tabs_;
				color color_tabs_border_;
				float tabs_border_ = .5f;

			public:
				tab_pane_specs() {
					cursor_ = cursor_type::hand;
				}

				// generic specs

				std::string& text() override;
				tab_pane_specs& text(const std::string& text);

				std::string& tooltip() override;
				tab_pane_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				tab_pane_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				tab_pane_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				tab_pane_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				tab_pane_specs& font(const std::string& font);

				float& font_size() override;
				tab_pane_specs& font_size(const float& font_size);

				color& color_text() override;
				tab_pane_specs& color_text(const color& color_text);

				color& color_fill() override;
				tab_pane_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				tab_pane_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				tab_pane_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				tab_pane_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the side on which to place the tab.</summary>
				/// <returns>A reference to the property, as defined in <see cref="side"></see>.</returns>
				side& tab_side();

				/// <summary>Set the side on which to place the tab.</summary>
				/// <param name="tab_side">The property, as defined in <see cref="side"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& tab_side(const side& tab_side);

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
				tab_pane_specs& caption_reserve(const std::vector<std::string>& caption_reserve);

				/// <summary>Get or set the orientation of the caption text.</summary>
				/// <returns>A reference to the property, as defined in <see cref="orientation"></see>.</returns>
				orientation& caption_orientation();

				/// <summary>Set the orientation of the caption text.</summary>
				/// <param name="caption_orientation">The caption orientation, as defined in <see cref="orientation"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& caption_orientation(const orientation& caption_orientation);

				/// <summary>Get or set the fill color of the tabs.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tabs();

				/// <summary>Set the fill color of the tabs.</summary>
				/// <param name="color_tabs">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& color_tabs(const color& color_tabs);

				/// <summary>Get or set the color of the tab borders.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tabs_border();

				/// <summary>Set the color of the tab borders.</summary>
				/// <param name="color_tabs_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& color_tabs_border(const color& color_tabs_border);

				/// <summary>Get or set the thickness of the tab borders.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& tabs_border();

				/// <summary>Set the thickness of the tab borders.</summary>
				/// <param name="tabs_border">The thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				tab_pane_specs& tabs_border(const float& tabs_border);
			};

			/// <summary>Tab pane container builder.</summary>
			/// <remarks>Only tab containers can be added to the tab pane container.</remarks>
			class lecui_api tab_pane_builder {
			public:
				/// <summary>Tab pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <remarks>This constructs the container with an internally generated random
				/// alias.</remarks>
				tab_pane_builder(containers::page& page);

				/// <summary>Tab pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_pane".
				/// </param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab pane leads to undefined behavior.</remarks>
				tab_pane_builder(containers::page& page, const std::string& alias);
				~tab_pane_builder();

				/// <summary>Get the tab pane specifications.</summary>
				/// <returns>A reference to the control specifications.</returns>
				[[nodiscard]]
				tab_pane_specs& specs();

				/// <summary>Get the tab pane specifications.</summary>
				/// <returns>A reference to the control specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				tab_pane_specs& operator()();

				/// <summary>Get the specifications of a tab pane.</summary>
				/// <param name="fm">The form the control is in.</param>
				/// <param name="path">The full path to the control, e.g.
				/// "sample_page/settings_tab_pane".</param>
				/// <returns>A reference to the control specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static tab_pane_specs& specs(form& fm, const std::string& path);

				/// <summary>Select the visible tab in the tab pane.</summary>
				/// <param name="tab_name">The in-control unique name of the tab, e.g. "Options".
				/// </param>
				/// <remarks>By default none of the tabs is selected if this method is
				/// never called and the tab pane will load empty. The first selection will
				/// happen when the user clicks a tab, at which point the contents of that tab
				/// will be displayed.</remarks>
				void select(const std::string& tab_name);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tab_pane_builder() = delete;
				tab_pane_builder(const tab_pane_builder&) = delete;
				tab_pane_builder& operator=(const tab_pane_builder&) = delete;

				friend class tab_builder;
			};

			/// <summary>Tab container builder.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing tab panes within tabs that are themselves in another
			/// tab pane, to virtually any depth level that the memory of the computer the app
			/// is running on can permit.</remarks>
			class lecui_api tab_builder {
			public:
				/// <summary>Tab builder constructor.</summary>
				/// <param name="tp">The tab pane to place it in.</param>
				/// <param name="tab_name">The in-tab_pane unique name of the tab,
				/// e.g. "Options".</param>
				tab_builder(tab_pane_builder& tp, const std::string& tab_name);
				~tab_builder();

				/// <summary>Get the tab container page.</summary>
				/// <returns>A reference to the tab container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the tab, just
				/// like a regular page.</remarks>
				[[nodiscard]]
				containers::page& get();

				/// <summary>Get the tab container page of an existing tab.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the tab, e.g.
				/// "sample_page/settings_tab_pane/Options".
				/// </param>
				/// <returns>A reference to the tab container page.</returns>
				[[nodiscard]]
				static containers::page& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				tab_builder() = delete;
				tab_builder(const tab_builder&) = delete;
				tab_builder& operator=(const tab_builder&) = delete;
			};
		}
	}
}
