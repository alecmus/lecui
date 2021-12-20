//
// combobox.h - combobox widget interface
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
			/// <summary>Combobox widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended height with defaults is 25px</remarks>
			class lecui_api combobox : public widget {
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
				bool _editable = false;
				color _color_fill_editable;
				color _color_caret;
				color _color_border;
				color _color_dropdown;
				color _color_dropdown_hot;
				color _color_dropdown_arrow;
				color _color_dropdown_arrow_hot;
				float _border = .5f;
				float _corner_radius_x = 2.f;
				float _corner_radius_y = 2.f;
				std::vector<combobox_item> _items;
				image_quality _quality = image_quality::high;
				sort_options _sort = sort_options::ascending;
				bool _force_numerical_sort = false;
				std::string _selected;

			public:
				combobox() {
					_rect.size({ 200.f, 25.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct combobox_events : basic_events {
					/// <summary>Called when a selection is made.</summary>
					/// <remarks>The parameter contains the selected item.</remarks>
					std::function<void(const std::string&)> selection = nullptr;
				};

				combobox_events& events() {
					return _combobox_events;
				}

				bool operator==(const combobox&);
				bool operator!=(const combobox&);

				// generic widget

				std::string& text() override;
				combobox& text(const std::string& text);

				std::string& tooltip() override;
				combobox& tooltip(const std::string& tooltip);

				badge_specs& badge() override;
				combobox& badge(const badge_specs& badge);

				lecui::rect& rect() override;
				combobox& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				combobox& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				combobox& cursor(const cursor_type cursor);

				std::string& font() override;
				combobox& font(const std::string& font);

				float& font_size() override;
				combobox& font_size(const float& font_size);

				color& color_text() override;
				combobox& color_text(const color& color_text);

				color& color_fill() override;
				combobox& color_fill(const color& color_fill);

				color& color_hot() override;
				combobox& color_hot(const color& color_hot);

				color& color_selected() override;
				combobox& color_selected(const color& color_selected);

				color& color_disabled() override;
				combobox& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_border(const color& color_border);

				/// <summary>Get or set the horizontal radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_x();

				/// <summary>Set the horizontal radius of the corners.</summary>
				/// <param name="corner_radius_x">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& corner_radius_x(const float& corner_radius_x);

				/// <summary>Get or set the vertical radius of the corners.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& corner_radius_y();

				/// <summary>Set the vertical radius of the corners.</summary>
				/// <param name="corner_radius_y">The horizontal radius of the corner, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& corner_radius_y(const float& corner_radius_y);

				/// <summary>Get or set the combobox editable property.</summary>
				/// <returns>A reference to the property.</returns>
				bool& editable();

				/// <summary>Set the combobox editable property.</summary>
				/// <param name="editable">Whether to make the combobox editable.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& editable(const bool& editable);

				/// <summary>Get or set the fill color of an editable combobox.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_fill_editable();

				/// <summary>Set the fill color of an editable combobox.</summary>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_fill_editable(const color& color_fill_editable);

				/// <summary>Get or set the color of the caret.</summary>
				/// <returns>A reference to the caret color.</returns>
				color& color_caret();

				/// <summary>Set the color of the caret.</summary>
				/// <param name="color_caret">The color of the caret.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_caret(const color& color_caret);

				/// <summary>Get or set the color of the dropdown.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown();

				/// <summary>Set the color of the dropdown.</summary>
				/// <param name="color_dropdown">The color of the dropdown.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_dropdown(const color& color_dropdown);

				/// <summary>Get or set the color of the dropdown when the mouse is hovered over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_hot();

				/// <summary>Set the color of the dropdown when the mouse is hovered over it.</summary>
				/// <param name="color_dropdown_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_dropdown_hot(const color& color_dropdown_hot);

				/// <summary>Get or set the color of the dropdown arrow.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_arrow();

				/// <summary>Set the color of the dropdown arrow.</summary>
				/// <param name="color_dropdown_arrow">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_dropdown_arrow(const color& color_dropdown_arrow);

				/// <summary>Get or set the color of the dropdown arrow when the mouse is hovered over it.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_dropdown_arrow_hot();

				/// <summary>Set the color of the dropdown arrow when the mouse is hovered over it.</summary>
				/// <param name="color_dropdown_arrow_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_dropdown_arrow_hot(const color& color_dropdown_arrow_hot);

				/// <summary>Get or set the list of combobox items.</summary>
				/// <returns>A reference to the list.</returns>
				std::vector<combobox_item>& items();

				/// <summary>Set the list of combobox items.</summary>
				/// <param name="items">The list.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& items(const std::vector<combobox_item>& items);

				/// <summary>Get or set the quality of the images.</summary>
				/// <returns>A reference to the image quality.</returns>
				image_quality& quality();

				/// <summary>Set the quality of the images.</summary>
				/// <param name="quality">The quality, as defined in <see cref="quality"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& quality(const image_quality& quality);

				/// <summary>Get or set the sort options.</summary>
				/// <returns>A reference to the sort options.</returns>
				sort_options& sort() { return _sort; }

				/// <summary>Set the sort options.</summary>
				/// <param name="sort">The sort options, as defined in <see cref="sort_options"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& sort(const sort_options& sort);

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
				combobox& force_numerical_sort(const bool& force_numerical_sort);

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
				combobox& selected(const std::string& selected);

			public:
				/// <summary>Add a combobox to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static combobox& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a combobox.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_one/title".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static combobox& get(form& fm, const std::string& path);

			private:
				combobox_events _combobox_events;
			};
		}
	}
}

/// Helper for getting combobox widget. Builder documentation applies.
#define get_combobox(path) liblec::lecui::widgets::combobox::get(*this, path)
