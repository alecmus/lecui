//
// progress_indicator.h - progress indicator widget interface
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
			/// <summary>Progress indicator widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 50x50px</remarks>
			class progress_indicator_specs : public specs {
				color color_empty_;
				float line_thickness_empty_ = .5f;
				float line_thickness_fill_ = 1.5f;
				float percentage_ = 0.f;
				int precision_ = 0;

			public:
				progress_indicator_specs() {
					rect_.size({ 50.f, 50.f });
				}

				bool operator==(const progress_indicator_specs&);
				bool operator!=(const progress_indicator_specs&);

				// generic specs

				std::string& text() override { return text_; }
				progress_indicator_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				progress_indicator_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				progress_indicator_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				progress_indicator_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				progress_indicator_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				progress_indicator_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				progress_indicator_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				progress_indicator_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				progress_indicator_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				progress_indicator_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				progress_indicator_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				progress_indicator_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the color to use for the empty portion of the indicator.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_empty() { return color_empty_; }

				/// <summary>Set the color to use for the empty portion of the indicator</summary>
				/// <param name="color_empty">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& color_empty(const color& color_empty) {
					color_empty_ = color_empty;
					return *this;
				}

				/// <summary>Get or set the line thickness of the empty portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_empty() { return line_thickness_empty_; }

				/// <summary>Set the line thickness of the empty portion of the indicator.</summary>
				/// <param name="line_thickness_empty">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& line_thickness_empty(const float& line_thickness_empty) {
					line_thickness_empty_ = line_thickness_empty;
					return *this;
				}

				/// <summary>Get or set the line thickness of the filled portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_fill() { return line_thickness_fill_; }

				/// <summary>Set the line thickness of the filled portion of the indicator.</summary>
				/// <param name="line_thickness_fill">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& line_thickness_fill(const float& line_thickness_fill) {
					line_thickness_fill_ = line_thickness_fill;
					return *this;
				}

				/// <summary>Get or set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage() { return percentage_; }

				/// <summary>Set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& percentage(const float& percentage) {
					percentage_ = percentage;
					return *this;
				}

				/// <summary>Get or set the precision to use for displaying the percentage.</summary>
				/// <returns>A reference to the precision.</returns>
				int& precision() { return precision_; }

				/// <summary>Set the precision to use for displaying the percentage.</summary>
				/// <param name="precision">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& precision(const int& precision) {
					precision_ = precision;
					return *this;
				}
			};

			/// <summary>Progress indicator widget builder.</summary>
			class lecui_api progress_indicator_builder {
			public:
				/// <summary>Progress indicator builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				progress_indicator_builder(containers::page& page);

				/// <summary>Progress indicator builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "file_transfer".</param>
				progress_indicator_builder(containers::page& page, const std::string& alias);
				~progress_indicator_builder();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				[[nodiscard]]
				progress_indicator_specs& specs();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				progress_indicator_specs& operator()();

				/// <summary>Get the specifications of a progress indicator.</summary>
				/// <param name="fm">The form containing the progress indicator.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/files/file_transfer".</param>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static progress_indicator_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				progress_indicator_builder() = delete;
				progress_indicator_builder(const progress_indicator_builder&) = delete;
				progress_indicator_builder& operator=(const progress_indicator_builder&) = delete;
			};
		}
	}
}
