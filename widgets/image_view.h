//
// image_view.h - image view widget interface
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

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Image view widget specifications.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api image_view_specs : public specs {
				float border_ = .5f;
				color color_border_;
				float corner_radius_x_ = .0f;
				float corner_radius_y_ = .0f;
				int png_resource_ = 0;
				std::string file_;
				image_quality quality_ = image_quality::medium;
				bool enlarge_if_smaller_ = false;
				bool keep_aspect_ratio_ = true;

			public:
				image_view_specs() {
					cursor_ = cursor_type::hand;
					rect_.size({ 200.f, 200.f });
				}

				bool operator==(const image_view_specs&);
				bool operator!=(const image_view_specs&);

				// generic specs

				std::string& text() override { return text_; }
				image_view_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				image_view_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				image_view_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				image_view_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				image_view_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				image_view_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				image_view_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				image_view_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				image_view_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				image_view_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				image_view_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				image_view_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border() { return border_; }

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x() { return corner_radius_x_; }

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& corner_radius_x(const float& corner_radius_x) {
					corner_radius_x_ = corner_radius_x;
					return *this;
				}

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y() { return corner_radius_y_; }

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& corner_radius_y(const float& corner_radius_y) {
					corner_radius_y_ = corner_radius_y;
					return *this;
				}

				/// <summary>Get or set the icon's PNG resource.</summary>
				/// <returns>A reference to the resource's ID.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				int& png_resource() { return png_resource_; }

				/// <summary>Set the icon's PNG resource.</summary>
				/// <param name="png_resource">The ID of PNG resource.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				image_view_specs& png_resource(const int& png_resource) {
					png_resource_ = png_resource;
					return *this;
				}

				/// <summary>Get or set the path to the icon's image file.</summary>
				/// <returns>A reference to the path.<returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				std::string& file() { return file_; }

				/// <summary>Set the path to the icon's image file.</summary>
				/// <param name="file">The path to the file.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				image_view_specs& file(const std::string& file) {
					file_ = file;
					return *this;
				}

				/// <summary>Get or set the quality of the image in the icon.</summary>
				/// <returns>A reference to the quality property.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				image_quality& quality() { return quality_; }

				/// <summary>Set the quality of the image in the icon.</summary>
				/// <param name="quality">The quality of the image, as defined in <see cref="image_quality"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				image_view_specs& quality(const image_quality& quality) {
					quality_ = quality;
					return *this;
				}

				/// <summary>Get or set the property for enlarging the image to fill up the rect if the image
				/// is smaller than the rectangle.</summary>
				/// <returns>A reference to the property.</returns>
				bool& enlarge_if_smaller() { return enlarge_if_smaller_; }

				/// <summary>Set the property for enlarging the image to fill up the rect if the image
				/// is smaller than the rectangle.</summary>
				/// <param name="enlarge_if_smaller">Whether to enlarge the image to fill up the rect if the
				/// image is smaller than the rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& enlarge_if_smaller(const bool& enlarge_if_smaller) {
					enlarge_if_smaller_ = enlarge_if_smaller;
					return *this;
				}

				/// <summary>Get or set the property for whether to keep the image aspect ratio.</summary>
				/// <returns>A reference to the property.</returns>
				bool& keep_aspect_ratio() { return keep_aspect_ratio_; }

				/// <summary>Set the property for whether to keep the image aspect ratio.</summary>
				/// <param name="keep_aspect_ratio">Whether to keep the image aspect ratio.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view_specs& keep_aspect_ratio(const bool& keep_aspect_ratio) {
					keep_aspect_ratio_ = keep_aspect_ratio;
					return *this;
				}
			};

			/// <summary>Image view widget builder.</summary>
			class lecui_api image_view_builder {
			public:
				/// <summary>Image view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				image_view_builder(containers::page& page);

				/// <summary>Image view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				image_view_builder(containers::page& page, const std::string& alias);
				~image_view_builder();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				[[nodiscard]]
				image_view_specs& specs();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				image_view_specs& operator()();

				/// <summary>Get the specifications of an image.</summary>
				/// <param name="fm">The form containing the image.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static image_view_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				image_view_builder() = delete;
				image_view_builder(const image_view_builder&) = delete;
				image_view_builder& operator=(const image_view_builder&) = delete;
			};
		}
	}
}
