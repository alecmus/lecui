//
// icon.h - icon widget interface
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
			/// <summary>Icon widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 180x78px.</remarks>
			class lecui_api icon_specs : public specs {
			public:
				/// <summary>Text position in icon.</summary>
				enum class icon_text_position {
					right,
					left,
					top,
					bottom,
				};

			private:
				float _border = .5f;
				color _color_border;
				float _corner_radius_x = 3.f;
				float _corner_radius_y = 3.f;
				float _max_image_size = 48.f;
				color _color_text_description;
				float _font_size_description = 8.5f;
				int _png_resource = 0;
				std::string _file;
				image_quality _quality = image_quality::high;
				std::string _description;
				icon_text_position _text_position = icon_text_position::right;
				void* _p_special_pane_specs = nullptr;

			public:
				icon_specs() {
					_rect.size({ 160.f, 58.f });
					_font_size = 11.f;
				};

				bool operator==(const icon_specs&);
				bool operator!=(const icon_specs&);

				// generic specs

				std::string& text() override;
				icon_specs& text(const std::string& text);

				std::string& tooltip() override;
				icon_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				icon_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				icon_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				icon_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				icon_specs& font(const std::string& font);

				float& font_size() override;
				icon_specs& font_size(const float& font_size);

				color& color_text() override;
				icon_specs& color_text(const color& color_text);

				color& color_fill() override;
				icon_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				icon_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				icon_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				icon_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the maximum size of the image square's side.</summary>
				/// <returns>A reference to the property, in pixels.</returns>
				/// <remarks>Set to 0.f for unlimited image size.</remarks>
				float& max_image_size();

				/// <summary>Set the maximum size of the image square's side.</summary>
				/// <param name="max_image_size">The size of the side, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Set to 0.f for unlimited image size.</remarks>
				icon_specs& max_image_size(const float& max_image_size);

				/// <summary>Get or set the color of the description text.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_text_description();

				/// <summary>Set the color of the description text.</summary>
				/// <param name="color_text_description">The color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& color_text_description(const color& color_text_description);

				/// <summary>Get or set the size of the description text.</summary>
				/// <returns>A reference to the size, in points.</returns>
				float& font_size_description();

				/// <summary>Set the size of the description text.</summary>
				/// <param name="font_size_description">The size of the font, in points.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& font_size_description(const float& font_size_description);

				/// <summary>Get or set the icon's PNG resource.</summary>
				/// <returns>A reference to the resource's ID.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				int& png_resource();

				/// <summary>Set the icon's PNG resource.</summary>
				/// <param name="png_resource">The ID of PNG resource.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				icon_specs& png_resource(const int& png_resource);

				/// <summary>Get or set the path to the icon's image file.</summary>
				/// <returns>A reference to the path.<returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				std::string& file();

				/// <summary>Set the path to the icon's image file.</summary>
				/// <param name="file">The path to the file.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>PNG resource takes precedence over image file.</remarks>
				icon_specs& file(const std::string& file);

				/// <summary>Get or set the quality of the image in the icon.</summary>
				/// <returns>A reference to the quality property.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				image_quality& quality();

				/// <summary>Set the quality of the image in the icon.</summary>
				/// <param name="quality">The quality of the image, as defined in <see cref="image_quality"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>Lower quality means greater performance and vice-versa.</remarks>
				icon_specs& quality(const image_quality& quality);

				/// <summary>Get or set the description text.</summary>
				/// <returns>A reference to the description text.</returns>
				std::string& description();

				/// <summary>Set the description text.</summary>
				/// <param name="description">The description text.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& description(const std::string& description);

				/// <summary>Get or set the position of the text.</summary>
				/// <returns>A reference to the text position.</returns>
				icon_text_position& text_position();

				/// <summary>Set the position of the text.</summary>
				/// <param name="text_position">The position of the text as defined in <see cref="icon_text_position"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				icon_specs& text_position(const icon_text_position& text_position);

			private:
#if defined(LECUI_EXPORTS)
				friend class form;
#endif
			};

			/// <summary>Icon widget builder.</summary>
			class lecui_api icon_builder {
			public:
				/// <summary>Icon builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				icon_builder(containers::page& page);

				/// <summary>Icon builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "users".</param>
				icon_builder(containers::page& page, const std::string& alias);
				~icon_builder();

				/// <summary>Get the icon specifications.</summary>
				/// <returns>A reference to the icon specifications.</returns>
				[[nodiscard]]
				icon_specs& specs();

				/// <summary>Get the icon specifications.</summary>
				/// <returns>A reference to the icon specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				icon_specs& operator()();

				/// <summary>Get the specifications of a icon.</summary>
				/// <param name="fm">The form containing the icon.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/users".</param>
				/// <returns>A reference to the icon specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static icon_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				icon_builder() = delete;
				icon_builder(const icon_builder&) = delete;
				icon_builder& operator=(const icon_builder&) = delete;
			};
		}
	}
}
