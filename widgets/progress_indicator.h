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
				/// <summary>Constructor.</summary>
				progress_indicator() {
					_rect.size({ 50.f, 50.f });
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const progress_indicator& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const progress_indicator& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_indicator& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
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

#if defined(lecui_helper)
/// Helper for getting progress indicator widget. 'Get' method documentation applies.
#define get_progress_indicator(path) liblec::lecui::widgets::progress_indicator::get(*this, path)
#endif
