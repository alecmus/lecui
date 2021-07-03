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
			class lecui_api progress_indicator_specs : public specs {
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

				std::string& text() override;
				progress_indicator_specs& text(const std::string& text);

				std::string& tooltip() override;
				progress_indicator_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				progress_indicator_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				progress_indicator_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				progress_indicator_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				progress_indicator_specs& font(const std::string& font);

				float& font_size() override;
				progress_indicator_specs& font_size(const float& font_size);

				color& color_text() override;
				progress_indicator_specs& color_text(const color& color_text);

				color& color_fill() override;
				progress_indicator_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				progress_indicator_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				progress_indicator_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				progress_indicator_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the color to use for the empty portion of the indicator.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_empty();

				/// <summary>Set the color to use for the empty portion of the indicator</summary>
				/// <param name="color_empty">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& color_empty(const color& color_empty);

				/// <summary>Get or set the line thickness of the empty portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_empty();

				/// <summary>Set the line thickness of the empty portion of the indicator.</summary>
				/// <param name="line_thickness_empty">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& line_thickness_empty(const float& line_thickness_empty);

				/// <summary>Get or set the line thickness of the filled portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_fill();

				/// <summary>Set the line thickness of the filled portion of the indicator.</summary>
				/// <param name="line_thickness_fill">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& line_thickness_fill(const float& line_thickness_fill);

				/// <summary>Get or set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage();

				/// <summary>Set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& percentage(const float& percentage);

				/// <summary>Get or set the precision to use for displaying the percentage.</summary>
				/// <returns>A reference to the precision.</returns>
				int& precision();

				/// <summary>Set the precision to use for displaying the percentage.</summary>
				/// <param name="precision">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator_specs& precision(const int& precision);
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
