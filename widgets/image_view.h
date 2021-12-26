//
// image_view.h - image view widget interface
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
			/// <summary>Image view widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Default size is 200x200px.</remarks>
			class lecui_api image_view : public widget, public badge_widget {
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = .0f;
				float _corner_radius_y = .0f;
				int _png_resource = 0;
				std::string _file;
				image_quality _quality = image_quality::medium;
				bool _enlarge_if_smaller = false;
				bool _keep_aspect_ratio = true;
				float _opacity = 100.f;

			public:
				image_view() {
					_cursor = cursor_type::hand;
					_rect.size({ 200.f, 200.f });
				}

				bool operator==(const image_view&);
				bool operator!=(const image_view&);

				// generic widget

				std::string& text() override;
				image_view& text(const std::string& text);

				std::string& tooltip() override;
				image_view& tooltip(const std::string& tooltip);

				badge_specs& badge() override;
				image_view& badge(const badge_specs& badge);

				lecui::rect& rect() override;
				image_view& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				image_view& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				image_view& cursor(const cursor_type cursor);

				std::string& font() override;
				image_view& font(const std::string& font);

				float& font_size() override;
				image_view& font_size(const float& font_size);

				color& color_text() override;
				image_view& color_text(const color& color_text);

				color& color_fill() override;
				image_view& color_fill(const color& color_fill);

				color& color_hot() override;
				image_view& color_hot(const color& color_hot);

				color& color_selected() override;
				image_view& color_selected(const color& color_selected);

				color& color_disabled() override;
				image_view& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the icon's PNG resource.</summary>
				/// <returns>A reference to the resource's ID.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				int& png_resource();

				/// <summary>Set the icon's PNG resource.</summary>
				/// <param name="png_resource">The ID of PNG resource.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				image_view& png_resource(const int& png_resource);

				/// <summary>Get or set the path to the icon's image file.</summary>
				/// <returns>A reference to the path.<returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				std::string& file();

				/// <summary>Set the path to the icon's image file.</summary>
				/// <param name="file">The path to the file.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				image_view& file(const std::string& file);

				/// <summary>Get or set the quality of the image in the icon.</summary>
				/// <returns>A reference to the quality property.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				image_quality& quality();

				/// <summary>Set the quality of the image in the icon.</summary>
				/// <param name="quality">The quality of the image, as defined in <see cref="image_quality"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				image_view& quality(const image_quality& quality);

				/// <summary>Get or set the property for enlarging the image to fill up the rect if the image
				/// is smaller than the rectangle.</summary>
				/// <returns>A reference to the property.</returns>
				bool& enlarge_if_smaller();

				/// <summary>Set the property for enlarging the image to fill up the rect if the image
				/// is smaller than the rectangle.</summary>
				/// <param name="enlarge_if_smaller">Whether to enlarge the image to fill up the rect if the
				/// image is smaller than the rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& enlarge_if_smaller(const bool& enlarge_if_smaller);

				/// <summary>Get or set the property for whether to keep the image aspect ratio.</summary>
				/// <returns>A reference to the property.</returns>
				bool& keep_aspect_ratio();

				/// <summary>Set the property for whether to keep the image aspect ratio.</summary>
				/// <param name="keep_aspect_ratio">Whether to keep the image aspect ratio.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& keep_aspect_ratio(const bool& keep_aspect_ratio);

				/// <summary>Get or set the opacity of the image.</summary>
				/// <returns>A reference to the opacity, as a percentage.</returns>
				float& opacity();

				/// <summary>Set the opacity of the image.</summary>
				/// <param name="border">The opacity of the image, as a percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				image_view& opacity(const float& opacity);

			public:
				/// <summary>Add an image view to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static image_view& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of an image view.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static image_view& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting image view widget. Builder documentation applies.
#define get_image_view(path) liblec::lecui::widgets::image_view::get(*this, path)
