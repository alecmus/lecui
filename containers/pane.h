//
// pane.h - pane container interface
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
			/// <summary>Pane container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing panes within panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit.</remarks>
			class lecui_api pane {
			public:
				/// <summary>Pane container specifications.</summary>
				class pane_specs : public widgets::specs {
					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 5.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 5.f;

				public:
					pane_specs() {}

					// generic specs

					std::string& text() override { return text_; }
					pane_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					pane_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					pane_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					pane_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					pane_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					pane_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					pane_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					pane_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					pane_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					pane_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					pane_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					pane_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					float& border() { return border_; }
					pane_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					color& color_border() { return color_border_; }
					pane_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					pane_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					pane_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}
				};

				/// <summary>Pane constructor.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <remarks>This constructs the container with an internally generated random
				/// alias.</remarks>
				pane(containers::page& page);

				/// <summary>Pane constructor.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_pane".</param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a pane leads to undefined behavior.</remarks>
				pane(containers::page& page, const std::string& alias);
				~pane();

				/// <summary>Get the pane specifications.</summary>
				/// <returns>A reference to the pane specifications.</returns>
				[[nodiscard]]
				pane_specs& specs();

				/// <summary>Get the pane specifications.</summary>
				/// <returns>A reference to the pane specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				pane_specs& operator()();

				/// <summary>Get the specifications of a pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the pane, e.g.
				/// "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane specifications.</returns>
				[[nodiscard]]
				static pane_specs& specs(form& fm, const std::string& path);

				/// <summary>Get the pane container page.</summary>
				/// <returns>A reference to the pane container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the pane, just
				/// like a regular page.</remarks>
				[[nodiscard]]
				containers::page& get();

				/// <summary>Get the pane container page of an existing pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the pane, e.g.
				/// "sample_page/settings_pane".</param>
				/// <returns>A reference to the pane container page.</returns>
				[[nodiscard]]
				static containers::page& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				pane() = delete;
				pane(const pane&) = delete;
				pane& operator=(const pane&) = delete;
			};
		}
	}
}
