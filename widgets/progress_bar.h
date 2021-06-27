//
// progress_bar.h - progress bar widget interface
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
			/// <summary>Progress bar widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 200x12px</remarks>
			class progress_bar_specs : public specs {
				float border_ = 1.f;
				color color_border_;
				float percentage_ = 0.f;

			public:
				progress_bar_specs() {
					rect_.size({ 200.f, 12.f });
				}

				bool operator==(const progress_bar_specs&);
				bool operator!=(const progress_bar_specs&);

				// generic specs

				std::string& text() override { return text_; }
				progress_bar_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				progress_bar_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				progress_bar_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				progress_bar_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				progress_bar_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				progress_bar_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				progress_bar_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				progress_bar_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				progress_bar_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				progress_bar_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				progress_bar_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				progress_bar_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border() { return border_; }

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage() { return percentage_; }

				/// <summary>Set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& percentage(const float& percentage) {
					percentage_ = percentage;
					return *this;
				}
			};

			/// <summary>Progress bar widget builder.</summary>
			class lecui_api progress_bar_builder {
			public:
				/// <summary>Progress bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				progress_bar_builder(containers::page& page);

				/// <summary>Progress bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "encryption_status".</param>
				progress_bar_builder(containers::page& page, const std::string& alias);
				~progress_bar_builder();

				/// <summary>Get the progress bar specifications.</summary>
				/// <returns>A reference to the progress bar specifications.</returns>
				[[nodiscard]]
				progress_bar_specs& specs();

				/// <summary>Get the progress bar specifications.</summary>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				progress_bar_specs& operator()();

				/// <summary>Get the specifications of a progress bar.</summary>
				/// <param name="fm">The form containing the progress bar.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/encryption_pane/encryption_status".</param>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static progress_bar_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				progress_bar_builder() = delete;
				progress_bar_builder(const progress_bar_builder&) = delete;
				progress_bar_builder& operator=(const progress_bar_builder&) = delete;
			};
		}
	}
}
