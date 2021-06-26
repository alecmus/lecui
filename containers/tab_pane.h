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
			class tab;

			/// <summary>Tab pane container.</summary>
			/// <remarks>Only tab containers can be added to this container.</remarks>
			class lecui_api tab_pane {
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

				/// <summary>Tab pane specifications.</summary>
				class tab_pane_specs : public widgets::specs {
					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 5.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 5.f;

					/// <summary>The side on which to place the tab, as defined in <see cref="side"></see>.</summary>
					side tab_side_ = side::top;

					/// <summary>Only affects tab captions whose orientation is perpendicular to
					/// the tab area. Make a reservation for the captions of all the tabs that are
					/// going to be added. If there is no way to know before-hand it is recommended
					/// to insert at least one dummy caption that estimates the size of the longest
					/// expected caption.</summary>
					/// <remarks>This is not required if the tab caption orientation is parallel
					/// to the tab area. If the tab caption orientation is perpendicular, it is
					/// important to set this reservation before adding the first tab to the tab
					/// pane. Failure to do this will result in the thickness of the tab area being
					/// clipped to the size of the first tab's caption. This, however, won't be an
					/// issue if the rest of the tab captions are shorter than the first.</remarks>
					std::vector<std::string> caption_reserve_;

					/// <summary>The orientation of the caption text, as defined in <see cref="orientation"></see>.</summary>
					orientation caption_orientation_ = orientation::horizontal;

					/// <summary>The fill color of the tabs.</summary>
					color color_tabs_;

					/// <summary>The color of the tab borders.</summary>
					color color_tabs_border_;

					/// <summary>The thickness of the tab borders.</summary>
					float tabs_border_ = .5f;

				public:
					tab_pane_specs() {
						cursor_ = cursor_type::hand;
					}

					// generic specs

					std::string& text() override { return text_; }
					tab_pane_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					tab_pane_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					tab_pane_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					tab_pane_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					tab_pane_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					tab_pane_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					tab_pane_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					tab_pane_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					tab_pane_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					tab_pane_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					tab_pane_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					tab_pane_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					float& border() { return border_; }
					tab_pane_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					color& color_border() { return color_border_; }
					tab_pane_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					tab_pane_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					tab_pane_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}

					side& tab_side() { return tab_side_; }
					tab_pane_specs& tab_side(const side& tab_side) {
						tab_side_ = tab_side;
						return *this;
					}

					std::vector<std::string>& caption_reserve() { return caption_reserve_; }
					tab_pane_specs& caption_reserve(const std::vector<std::string>& caption_reserve) {
						caption_reserve_ = caption_reserve;
						return *this;
					}

					orientation& caption_orientation() { return caption_orientation_; }
					tab_pane_specs& caption_orientation(const orientation& caption_orientation) {
						caption_orientation_ = caption_orientation;
						return *this;
					}

					color& color_tabs() { return color_tabs_; }
					tab_pane_specs& color_tabs(const color& color_tabs) {
						color_tabs_ = color_tabs;
						return *this;
					}

					color& color_tabs_border() { return color_tabs_border_; }
					tab_pane_specs& color_tabs_border(const color& color_tabs_border) {
						color_tabs_border_ = color_tabs_border;
						return *this;
					}

					float& tabs_border() { return tabs_border_; }
					tab_pane_specs& tabs_border(const float& tabs_border) {
						tabs_border_ = tabs_border;
						return *this;
					}
				};

				/// <summary>Tab pane constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <remarks>This constructs the container with an internally generated random
				/// alias.</remarks>
				tab_pane(containers::page& page);

				/// <summary>Tab pane constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_pane".
				/// </param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab pane leads to undefined behavior.</remarks>
				tab_pane(containers::page& page, const std::string& alias);
				~tab_pane();

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
				tab_pane() = delete;
				tab_pane(const tab_pane&) = delete;
				tab_pane& operator=(const tab_pane&) = delete;

				friend class tab;
			};

			/// <summary>Tab container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing tab panes within tabs that are themselves in another
			/// tab pane, to virtually any depth level that the memory of the computer the app
			/// is running on can permit.</remarks>
			class lecui_api tab {
			public:
				/// <summary>Tab constructor.</summary>
				/// <param name="tp">The tab pane to place it in.</param>
				/// <param name="tab_name">The in-tab_pane unique name of the tab,
				/// e.g. "Options".</param>
				tab(tab_pane& tp, const std::string& tab_name);
				~tab();

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
				tab() = delete;
				tab(const tab&) = delete;
				tab& operator=(const tab&) = delete;
			};
		}
	}
}
