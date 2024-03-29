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
					std::string label;				///< The label of the item.
					std::string font = "Segoe UI";	///< The font to use for the item.
					float font_size = 9.f;			///< The size of the font, in points.
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
				/// <summary>Constructor.</summary>
				combobox() {
					_rect.size({ 200.f, 25.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct combobox_events : basic_events {
					/// <summary>Selection event. Happens when a selection is made.</summary>
					/// <remarks>The parameter contains the selected item.</remarks>
					std::function<void(const std::string&)> selection = nullptr;
				};

				/// <summary>Get or set combobox events.</summary>
				/// <returns>A reference to the combobox events.</returns>
				combobox_events& events() {
					return _combobox_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const combobox& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const combobox& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				combobox& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
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
				/// <summary>Combobox events.</summary>
				combobox_events _combobox_events;
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting combobox widget. 'Get' method documentation applies.
#define get_combobox(path) liblec::lecui::widgets::combobox::get(*this, path)
#endif
