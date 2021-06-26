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
			/// <summary>Toggle widget.</summary>
			class lecui_api toggle {
			public:
				/// <summary>Toggle widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 20px. Recommended minimum
				/// width with defaults is 100px. Default size is 200x20px.</remarks>
				class toggle_specs : public specs {
					/// <summary>The text when the toggle is off.</summary>
					std::string text_off_ = "Off";

					/// <summary>The color of the background when toggle is on.</summary>
					color color_on_;

					/// <summary>The color of the background when toggle is off.</summary>
					color color_off_;

					/// <summary>Whether the toggle is on.</summary>
					bool on_ = false;

				public:
					toggle_specs() {
						cursor_ = cursor_type::hand;
						rect_.size({ 200.f, 20.f });
						text_ = "On";
					}

					/// <summary>Events specific to this widget.</summary>
					struct toggle_events : basic_events {
						/// <summary>Handler that's called when toggle is clicked or dragged.</summary>
						/// <remarks>The parameter indicates the toggle's new state.</remarks>
						std::function<void(bool on)> toggle = nullptr;
					};

					toggle_events& events() {
						return toggle_events_;
					}

					bool operator==(const toggle_specs&);
					bool operator!=(const toggle_specs&);

					// generic specs

					std::string& text() override { return text_; }
					toggle_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					toggle_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					toggle_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					toggle_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					toggle_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					toggle_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					toggle_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					toggle_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					toggle_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					toggle_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					toggle_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					toggle_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}

					// widget specific specs

					std::string& text_off() { return text_off_; }
					toggle_specs& text_off(const std::string& text_off) {
						text_off_ = text_off;
						return *this;
					}

					color& color_on() { return color_on_; }
					toggle_specs& color_on(const color& color_on) {
						color_on_ = color_on;
						return *this;
					}

					color& color_off() { return color_off_; }
					toggle_specs& color_off(const color& color_off) {
						color_off_ = color_off;
						return *this;
					}

					bool& on() { return on_; }
					toggle_specs& on(const bool& on) {
						on_ = on;
						return *this;
					}

				private:
					toggle_events toggle_events_;
				};

				/// <summary>Toggle constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				toggle(containers::page& page);

				/// <summary>Toggle constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "use_ssl".</param>
				toggle(containers::page& page, const std::string& alias);
				~toggle();

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
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				toggle() = delete;
				toggle(const toggle&) = delete;
				toggle& operator=(const toggle&) = delete;
			};
		}
	}
}
