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
			/// <summary>Toggle widget specifications.</summary>
			/// <remarks>Recommended height with defaults is 20px. Recommended minimum
			/// width with defaults is 100px. Default size is 200x20px.</remarks>
			class lecui_api toggle_specs : public specs {
				std::string _text_off = "Off";
				color _color_on;
				color _color_off;
				bool _on = false;

			public:
				toggle_specs() {
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

				bool operator==(const toggle_specs&);
				bool operator!=(const toggle_specs&);

				// generic specs

				std::string& text() override;
				toggle_specs& text(const std::string& text);

				std::string& tooltip() override;
				toggle_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				toggle_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				toggle_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				toggle_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				toggle_specs& font(const std::string& font);

				float& font_size() override;
				toggle_specs& font_size(const float& font_size);

				color& color_text() override;
				toggle_specs& color_text(const color& color_text);

				color& color_fill() override;
				toggle_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				toggle_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				toggle_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				toggle_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the text to show when the toggle is off.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_off();

				/// <summary>Set the text to show when the toggle is off.</summary>
				/// <param name="text_off">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle_specs& text_off(const std::string& text_off);

				/// <summary>Get or set the background color to use when the toggle is on.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_on();

				/// <summary>Set the background color to use when the toggle is on.</summary>
				/// <param name="color_on">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle_specs& color_on(const color& color_on);

				/// <summary>Get or set the background color to use when the toggle is off.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_off();

				/// <summary>Set the background color to use when the toggle is off.</summary>
				/// <param name="color_off">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				toggle_specs& color_off(const color& color_off);

				/// <summary>Get or set the toggle status.</summary>
				/// <returns>A reference to the toggle status.</returns>
				bool& on();
				toggle_specs& on(const bool& on);

			private:
				toggle_events _toggle_events;
			};

			/// <summary>Toggle widget builder.</summary>
			class lecui_api toggle_builder {
			public:
				/// <summary>Toggle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				toggle_builder(containers::page& page);

				/// <summary>Toggle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "use_ssl".</param>
				toggle_builder(containers::page& page, const std::string& alias);
				~toggle_builder();

				/// <summary>Get the toggle specifications.</summary>
				/// <returns>A reference to the toggle specifications.</returns>
				[[nodiscard]]
				toggle_specs& specs();

				/// <summary>Get the toggle specifications.</summary>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				toggle_specs& operator()();

				/// <summary>Get the specifications of a toggle.</summary>
				/// <param name="fm">The form containing the toggle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/tools_tab_pane/options_tab/use_ssl".</param>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static toggle_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				toggle_builder() = delete;
				toggle_builder(const toggle_builder&) = delete;
				toggle_builder& operator=(const toggle_builder&) = delete;
			};
		}
	}
}
