//
// toggle.h - toggle widget interface
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
			/// <summary>Toggle widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended height with defaults is 20px. Recommended minimum
			/// width with defaults is 100px. Default size is 200x20px.</remarks>
			class lecui_api toggle : public widget {
				std::string _text_off = "Off";
				color _color_on;
				color _color_off;
				bool _on = false;

			public:
				toggle() {
					_cursor = cursor_type::hand;
					_rect.size({ 200.f, 20.f });
					_text = "On";
				}

				/// <summary>Events specific to this widget.</summary>
				struct toggle_events : basic_events {
					/// <summary>Handler that's called when toggle is clicked or dragged.</summary>
					/// <remarks>The parameter indicates the toggle's new state.</remarks>
					std::function<void(bool on)> toggle = nullptr;
				};

				toggle_events& events() {
					return _toggle_events;
				}

				bool operator==(const toggle&);
				bool operator!=(const toggle&);

				// generic widget

				std::string& text() override;
				toggle& text(const std::string& text);

				std::string& tooltip() override;
				toggle& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				toggle& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				toggle& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				toggle& cursor(const cursor_type cursor);

				std::string& font() override;
				toggle& font(const std::string& font);

				float& font_size() override;
				toggle& font_size(const float& font_size);

				color& color_text() override;
				toggle& color_text(const color& color_text);

				color& color_fill() override;
				toggle& color_fill(const color& color_fill);

				color& color_hot() override;
				toggle& color_hot(const color& color_hot);

				color& color_selected() override;
				toggle& color_selected(const color& color_selected);

				color& color_disabled() override;
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
				/// <param name="fm"></param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/tools_tab_pane/options_tab/use_ssl".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static toggle& get(form& fm, const std::string& path);

			private:
				toggle_events _toggle_events;
			};
		}
	}
}

/// Helper for getting toggle widget. Builder documentation applies.
#define get_toggle(path) liblec::lecui::widgets::toggle::get(*this, path)
