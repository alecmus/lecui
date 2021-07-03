//
// combobox.h - combobox widget interface
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
			/// <summary>Combobox widget specifications.</summary>
			/// <remarks>Recommended height with defaults is 25px</remarks>
			class lecui_api combobox_specs : public specs {
			public:
				/// <summary>A combobox item.</summary>
				struct combobox_item {
					/// <summary>The label of the item.</summary>
					std::string label;

					/// <summary>The font to use for the item.</summary>
					std::string font = "Segoe UI";

					/// <summary>The size of the font, in points.</summary>
					float font_size = 9.f;
				};

			private:
				bool editable_ = false;
				color color_fill_editable_;
				color color_caret_;
				color color_border_;
				color color_dropdown_;
				color color_dropdown_hot_;
				color color_dropdown_arrow_;
				color color_dropdown_arrow_hot_;
				float border_ = .5f;
				float corner_radius_x_ = 2.f;
				float corner_radius_y_ = 2.f;
				std::vector<combobox_item> items_;
				image_quality quality_ = image_quality::high;
				sort_options sort_ = sort_options::ascending;
				bool force_numerical_sort_ = false;
				std::string selected_;

			public:
				combobox_specs() {
					rect_.size({ 200.f, 25.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct combobox_events : basic_events {
					/// <summary>Called when a selection is made.</summary>
					/// <remarks>The parameter contains the selected item.</remarks>
					std::function<void(const std::string&)> selection = nullptr;
				};

				combobox_events& events() {
					return combobox_events_;
				}

				bool operator==(const combobox_specs&);
				bool operator!=(const combobox_specs&);

				// generic specs

				std::string& text() override;
				combobox_specs& text(const std::string& text);

				std::string& tooltip() override;
				combobox_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				combobox_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				combobox_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				combobox_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				combobox_specs& font(const std::string& font);

				float& font_size() override;
				combobox_specs& font_size(const float& font_size);

				color& color_text() override;
				combobox_specs& color_text(const color& color_text);

				color& color_fill() override;
				combobox_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				combobox_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				combobox_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				combobox_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the combobox editable property.</summary>
				/// <returns>A reference to the property.</returns>
				bool& editable();

				/// <summary>Set the combobox editable property.</summary>
				/// <param name="editable">Whether to make the combobox editable.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& editable(const bool& editable);

				/// <summary>Get or set the fill color of an editable combobox.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_editable();

				/// <summary>Set the fill color of an editable combobox.</summary>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_fill_editable(const color& color_fill_editable);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the caret color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_caret(const color& color_caret);

				/// <summary>Get or set the color of the dropdown.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown();

				/// <summary>Set the color of the dropdown.</summary>
				/// <param name="color_dropdown">The color of the dropdown.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_dropdown(const color& color_dropdown);

				/// <summary>Get or set the color of the dropdown when the mouse is hovered over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_hot();

				/// <summary>Set the color of the dropdown when the mouse is hovered over it.</summary>
				/// <param name="color_dropdown_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_dropdown_hot(const color& color_dropdown_hot);

				/// <summary>Get or set the color of the dropdown arrow.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_arrow();

				/// <summary>Set the color of the dropdown arrow.</summary>
				/// <param name="color_dropdown_arrow">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_dropdown_arrow(const color& color_dropdown_arrow);

				/// <summary>Get or set the color of the dropdown arrow when the mouse is hovered over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_arrow_hot();

				/// <summary>Set the color of the dropdown arrow when the mouse is hovered over it.</summary>
				/// <param name="color_dropdown_arrow_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& color_dropdown_arrow_hot(const color& color_dropdown_arrow_hot);

				/// <summary>Get or set the list of combobox items.</summary>
				/// <returns>A reference to the list.</returns>
				std::vector<combobox_item>& items();

				/// <summary>Set the list of combobox items.</summary>
				/// <param name="items">The list.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& items(const std::vector<combobox_item>& items);

				/// <summary>Get or set the quality of the images.</summary>
				/// <returns>A reference to the image quality.</returns>
				image_quality& quality();

				/// <summary>Set the quality of the images.</summary>
				/// <param name="quality">The quality, as defined in <see cref="quality"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& quality(const image_quality& quality);

				/// <summary>Get or set the sort options.</summary>
				/// <returns>A reference to the sort options.</returns>
				sort_options& sort() { return sort_; }

				/// <summary>Set the sort options.</summary>
				/// <param name="sort">The sort options, as defined in <see cref="sort_options"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& sort(const sort_options& sort);

				/// <summary>Get or set whether numerical sorting is forced.</summary>
				/// <returns>A reference to the property.</returns>
				/// <remarks>When only numbers are in the items list numerical sorting is
				/// used. The presence of at least one non-numeric character causes string
				/// sorting to be used instead. In those instances where you need numerical
				/// sorting to be used regardless of the presence of non-numeric characters
				/// this is the way to do it.</remarks>
				bool& force_numerical_sort();

				/// <summary>Set whether numerical sorting is forced.</summary>
				/// <param name="force_numerical_sort">Whether to force numerical sorting.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>When only numbers are in the items list numerical sorting is
				/// used. The presence of at least one non-numeric character causes string
				/// sorting to be used instead. In those instances where you need numerical
				/// sorting to be used regardless of the presence of non-numeric characters
				/// this is the way to do it.</remarks>
				combobox_specs& force_numerical_sort(const bool& force_numerical_sort);

				/// <summary>Get or set the selected item.</summary>
				/// <returns>A reference to the selected item.</returns>
				/// <remarks>Note that this is not always the same as the .text variable in an
				/// editable combobox. The .text in an editable combobox only becomes part of
				/// the list of items when the user presses the enter key. Only then can it be
				/// considered as a "selected" item.</remarks>
				std::string& selected();

				/// <summary>Set the selected item.</summary>
				/// <param name="selected">The selected item.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox_specs& selected(const std::string& selected);

			private:
				combobox_events combobox_events_;
			};

			/// <summary>Combobox widget builder.</summary>
			class lecui_api combobox_builder {
			public:
				/// <summary>Combobox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				combobox_builder(containers::page& page);

				/// <summary>Combobox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				combobox_builder(containers::page& page, const std::string& alias);
				~combobox_builder();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				[[nodiscard]]
				combobox_specs& specs();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				combobox_specs& operator()();

				/// <summary>Get the specifications of a combobox.</summary>
				/// <param name="fm">The form containing the combobox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/title".</param>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static combobox_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				combobox_builder() = delete;
				combobox_builder(const combobox_builder&) = delete;
				combobox_builder& operator=(const combobox_builder&) = delete;
			};
		}
	}
}
