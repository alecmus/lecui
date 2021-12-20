//
// progress_indicator.h - progress indicator widget interface
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
			/// <summary>Progress indicator widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 50x50px</remarks>
			class lecui_api progress_indicator : public widget {
				color _color_empty;
				float _line_thickness_empty = .5f;
				float _line_thickness_fill = 1.5f;
				float _percentage = 0.f;
				int _precision = 0;

			public:
				progress_indicator() {
					_rect.size({ 50.f, 50.f });
				}

				bool operator==(const progress_indicator&);
				bool operator!=(const progress_indicator&);

				// generic widget

				std::string& text() override;
				progress_indicator& text(const std::string& text);

				std::string& tooltip() override;
				progress_indicator& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				progress_indicator& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				progress_indicator& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				progress_indicator& cursor(const cursor_type cursor);

				std::string& font() override;
				progress_indicator& font(const std::string& font);

				float& font_size() override;
				progress_indicator& font_size(const float& font_size);

				color& color_text() override;
				progress_indicator& color_text(const color& color_text);

				color& color_fill() override;
				progress_indicator& color_fill(const color& color_fill);

				color& color_hot() override;
				progress_indicator& color_hot(const color& color_hot);

				color& color_selected() override;
				progress_indicator& color_selected(const color& color_selected);

				color& color_disabled() override;
				progress_indicator& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the color to use for the empty portion of the indicator.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_empty();

				/// <summary>Set the color to use for the empty portion of the indicator</summary>
				/// <param name="color_empty">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& color_empty(const color& color_empty);

				/// <summary>Get or set the line thickness of the empty portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_empty();

				/// <summary>Set the line thickness of the empty portion of the indicator.</summary>
				/// <param name="line_thickness_empty">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& line_thickness_empty(const float& line_thickness_empty);

				/// <summary>Get or set the line thickness of the filled portion of the indicator.</summary>
				/// <returns>A reference to the line's thickness, in pixels.</returns>
				float& line_thickness_fill();

				/// <summary>Set the line thickness of the filled portion of the indicator.</summary>
				/// <param name="line_thickness_fill">The line's thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& line_thickness_fill(const float& line_thickness_fill);

				/// <summary>Get or set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage();

				/// <summary>Set the percentage of the progress indicator (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& percentage(const float& percentage);

				/// <summary>Get or set the precision to use for displaying the percentage.</summary>
				/// <returns>A reference to the precision.</returns>
				int& precision();

				/// <summary>Set the precision to use for displaying the percentage.</summary>
				/// <param name="precision">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& precision(const int& precision);

			public:
				/// <summary>Add a progress indicator to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "file_transfer".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static progress_indicator& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a progress indicator.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/files/file_transfer".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static progress_indicator& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting progress indicator widget. Builder documentation applies.
#define get_progress_indicator(path) liblec::lecui::widgets::progress_indicator::get(*this, path)
