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
			/// <summary>Combobox widget.</summary>
			class lecui_api combobox {
			public:
				struct combobox_item {
					std::string label;
					std::string font = "Segoe UI";
					float font_size = 9.f;
				};

				/// <summary>Combobox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class combobox_specs : public specs {
					/// <summary>Whether the combobox is editable.</summary>
					bool editable_ = false;

					/// <summary>The background color is it's editable.</summary>
					color color_fill_editable_;

					/// <summary>The color of the caret.</summary>
					color color_caret_;

					/// <summary>The color of the border.</summary>
					color color_border_;

					/// <summary>The color of the dropdown.</summary>
					color color_dropdown_;

					/// <summary>The color of the dropdown when the mouse is over it.</summary>
					color color_dropdown_hot_;

					/// <summary>The color of the dropdown arrow.</summary>
					color color_dropdown_arrow_;

					/// <summary>The color of the dropdown arrow when the mouse is over it.</summary>
					color color_dropdown_arrow_hot_;

					/// <summary>The thickness of the border.</summary>
					float border_ = .5f;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x_ = 2.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y_ = 2.f;

					/// <summary>The list of items available in the dropdown.</summary>
					std::vector<combobox_item> items_;

					/// <summary>The quality of the combo item images.</summary>
					image_quality quality_ = image_quality::high;

					/// <summary>How to sort the items.</summary>
					sort_options sort_ = sort_options::ascending;

					/// <summary>Forces numerical sorting.</summary>
					/// <remarks>When only numbers are in the items list numerical sorting is
					/// used. The presence of at least one non-numeric character causes string
					/// sorting to be used instead. In those instances where you need numerical
					/// sorting to be used regardless of the presence of non-numeric characters
					/// this is the way to do it.</remarks>
					bool force_numerical_sort_ = false;

					/// <summary>An item from the list in the dropdown that was last selected.
					/// </summary>
					/// <remarks>Note that this is not always the same as the .text variable in an
					/// editable combobox. The .text in an editable combobox only becomes part of
					/// the list of items when the user pressed the enter key. Only then can it be
					/// considered as a "selected" item.</remarks>
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

					std::string& text() override { return text_; }
					combobox_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					combobox_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					combobox_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					combobox_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					combobox_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					combobox_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					combobox_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					combobox_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					combobox_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					combobox_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					combobox_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					combobox_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					bool& editable() { return editable_; }
					combobox_specs& editable(const bool& editable) {
						editable_ = editable;
						return *this;
					}

					color& color_fill_editable() { return color_fill_editable_; }
					combobox_specs& color_fill_editable(const color& color_fill_editable) {
						color_fill_editable_ = color_fill_editable;
						return *this;
					}

					color& color_caret() { return color_caret_; }
					combobox_specs& color_caret(const color& color_caret) {
						color_caret_ = color_caret;
						return *this;
					}

					color& color_border() { return color_border_; }
					combobox_specs& color_border(const color& color_border) {
						color_border_ = color_border;
						return *this;
					}

					color& color_dropdown() { return color_dropdown_; }
					combobox_specs& color_dropdown(const color& color_dropdown) {
						color_dropdown_ = color_dropdown;
						return *this;
					}

					color& color_dropdown_hot() { return color_dropdown_hot_; }
					combobox_specs& color_dropdown_hot(const color& color_dropdown_hot) {
						color_dropdown_hot_ = color_dropdown_hot;
						return *this;
					}

					color& color_dropdown_arrow() { return color_dropdown_arrow_; }
					combobox_specs& color_dropdown_arrow(const color& color_dropdown_arrow) {
						color_dropdown_arrow_ = color_dropdown_arrow;
						return *this;
					}

					color& color_dropdown_arrow_hot() { return color_dropdown_arrow_hot_; }
					combobox_specs& color_dropdown_arrow_hot(const color& color_dropdown_arrow_hot) {
						color_dropdown_arrow_hot_ = color_dropdown_arrow_hot;
						return *this;
					}

					float& border() { return border_; }
					combobox_specs& border(const float& border) {
						border_ = border;
						return *this;
					}

					float& corner_radius_x() { return corner_radius_x_; }
					combobox_specs& corner_radius_x(const float& corner_radius_x) {
						corner_radius_x_ = corner_radius_x;
						return *this;
					}

					float& corner_radius_y() { return corner_radius_y_; }
					combobox_specs& corner_radius_y(const float& corner_radius_y) {
						corner_radius_y_ = corner_radius_y;
						return *this;
					}

					std::vector<combobox_item>& items() { return items_; }
					combobox_specs& items(const std::vector<combobox_item>& items) {
						items_ = items;
						return *this;
					}

					image_quality& quality() { return quality_; }
					combobox_specs& quality(const image_quality& quality) {
						quality_ = quality;
						return *this;
					}

					sort_options& sort() { return sort_; }
					combobox_specs& sort(const sort_options& sort) {
						sort_ = sort;
						return *this;
					}

					bool& force_numerical_sort() { return force_numerical_sort_; }
					combobox_specs& force_numerical_sort(const bool& force_numerical_sort) {
						force_numerical_sort_ = force_numerical_sort;
						return *this;
					}

					std::string& selected() { return selected_; }
					combobox_specs& selected(const std::string& selected) {
						selected_ = selected;
						return *this;
					}

				private:
					combobox_events combobox_events_;
				};

				/// <summary>Combobox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				combobox(containers::page& page);

				/// <summary>Combobox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				combobox(containers::page& page, const std::string& alias);
				~combobox();

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
				class combobox_impl;
				combobox_impl& d_;

				// Default constructor and copying an object of this class are not allowed
				combobox() = delete;
				combobox(const combobox&) = delete;
				combobox& operator=(const combobox&) = delete;
			};
		}
	}
}
