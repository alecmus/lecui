//
// checkbox.h - checkbox widget interface
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
			/// <summary>Checkbox widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 20x20px.</remarks>
			class lecui_api checkbox : public widget {
			public:
				/// <summary>Checkbox status.</summary>
				enum class checkbox_status {
					checked,		///< 'Checked' status.
					unchecked,		///< 'Unchecked' status.
					indeterminate,	///< 'Indeterminate' status.
				};

			private:
				float _border = .5f;
				color _color_border;
				std::string _text_unchecked;
				std::string _text_indeterminate;
				color _color_check;
				checkbox_status _status = checkbox_status::indeterminate;

			public:
				/// <summary>Constructor.</summary>
				checkbox() {
					_cursor = cursor_type::hand;
					_rect.size({ 20.f, 20.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct checkbox_events : basic_events {
					/// <summary>Called when the checkbox is clicked.</summary>
					/// <remarks>The parameter indicates the new checkbox status.</remarks>
					std::function<void(checkbox_status checked)> check = nullptr;
				};

				/// <summary>Get or set checkbox events.</summary>
				/// <returns></returns>
				checkbox_events& events() {
					return _checkbox_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const checkbox& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const checkbox& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_border(const color& color_border);

				/// <summary>Get or set the text to display when the widget is unchecked.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_unchecked();

				/// <summary>Set the text to display when the widget is unchecked.</summary>
				/// <param name="text_unchecked">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& text_unchecked(const std::string& text_unchecked);

				/// <summary>Get or set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_indeterminate();

				/// <summary>Set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <param name="text_indeterminate">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& text_indeterminate(const std::string& text_indeterminate);

				/// <summary>Get or set the color to use for the check symbol.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_check();

				/// <summary>Set the color to use for the check symbol.</summary>
				/// <param name="color_check">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_check(const color& color_check);

				/// <summary>Get or set the status of the checkbox widget.</summary>
				/// <returns>The status of the widget as defined in <see cref="checkbox_status"></see>.</returns>
				checkbox_status& status();

				/// <summary>Set the status of the checkbox widget.</summary>
				/// <param name="status">The status of the widget as defined in <see cref="checkbox_status"></see>.</param>
				/// <returns></returns>
				checkbox& status(const checkbox_status& status);

			public:
				/// <summary>Add a checkbox to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "employment_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static checkbox& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a checkbox.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_one/employment_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static checkbox& get(form& fm, const std::string& path);

			private:
				/// <summary>Checkbox events.</summary>
				checkbox_events _checkbox_events;
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting checkbox widget. 'Get' method documentation applies.
#define get_checkbox(path) liblec::lecui::widgets::checkbox::get(*this, path)
#endif
