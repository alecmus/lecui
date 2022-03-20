//
// toggle.h - toggle widget interface
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
			/// <summary>Toggle widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended height with defaults is 20px. Recommended minimum
			/// width with defaults is 100px. Default size is 200x20px.</remarks>
			class lecui_api toggle : public widget {
				std::string _text_off = "Off";
				color _color_on;
				color _color_off;
				bool _on = false;

			public:
				/// <summary>Constructor.</summary>
				toggle() {
					_cursor = cursor_type::hand;
					_rect.size({ 200.f, 20.f });
					_text = "On";
				}

				/// <summary>Events specific to this widget.</summary>
				struct toggle_events : basic_events {
					/// <summary>Toggle event. Happens when toggle is clicked or dragged.</summary>
					/// <remarks>The parameter indicates the toggle's new state.</remarks>
					std::function<void(bool on)> toggle = nullptr;
				};

				/// <summary>Get or set toggle widget events.</summary>
				/// <returns>A reference to the toggle widget events.</returns>
				toggle_events& events() {
					return _toggle_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const toggle& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const toggle& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the text to show when the toggle is off.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_off();

				/// <summary>Set the text to show when the toggle is off.</summary>
				/// <param name="text_off">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& text_off(const std::string& text_off);

				/// <summary>Get or set the background color to use when the toggle is on.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_on();

				/// <summary>Set the background color to use when the toggle is on.</summary>
				/// <param name="color_on">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_on(const color& color_on);

				/// <summary>Get or set the background color to use when the toggle is off.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_off();

				/// <summary>Set the background color to use when the toggle is off.</summary>
				/// <param name="color_off">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& color_off(const color& color_off);

				/// <summary>Get or set the toggle status.</summary>
				/// <returns>A reference to the toggle status.</returns>
				bool& on();

				/// <summary>Set the toggle status.</summary>
				/// <param name="on">The toggle status.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle& on(const bool& on);

			public:
				/// <summary>Add a toggle to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "use_ssl".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static toggle& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a toggle.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/tools_tab_pane/options_tab/use_ssl".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static toggle& get(form& fm, const std::string& path);

			private:
				/// <summary>Toggle widget events.</summary>
				toggle_events _toggle_events;
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting toggle widget. 'Get' method documentation applies.
#define get_toggle(path) liblec::lecui::widgets::toggle::get(*this, path)
#endif
