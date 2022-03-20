//
// slider.h - slider widget interface
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
			/// <summary>Slider widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>The slider's orientation is computed automatically based on the its
			/// size. If the width is greater than the height it's oriented horizontally, else
			/// vertically. With the defaults, the recommended minimum height for horizontal
			/// orientation is 35px and the recommended minimum with for vertical orientation
			/// is 40px.</remarks>
			class lecui_api slider : public widget {
			public:
				/// <summary>Slider range.</summary>
				struct slider_range {
					/// <summary>The minimum value of the slider.</summary>
					float minimum = 0.f;

					/// <summary>The maximum value of the slider.</summary>
					float maximum = 100.f;
				};

			private:
				float _border = 1.f;
				color _color_border;
				color _color_knob;
				color _color_knob_hot;
				color _color_knob_border;
				color _color_tick;
				bool _snap_to_ticks = true;
				bool _show_tick_marks = true;
				bool _show_tick_labels = true;
				slider_range _range;
				float _knob_radius = 8.f;
				float _slide_thickness = 6.f;
				float _major_tick_unit = 25.f;
				long _minor_tick_count = 4;
				float _value = 0.f;

			public:
				/// <summary>Constructor.</summary>
				slider() {
					_cursor = cursor_type::hand;
					_rect.size({ 200.f, 35.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct slider_events : basic_events {
					/// <summary>Slider event. Happens when the slider is moved.</summary>
					/// <remarks>The parameter indicates the knob's new position.</remarks>
					std::function<void(float value)> slider = nullptr;
				};

				/// <summary>Get or set slider events.</summary>
				/// <returns></returns>
				slider_events& events() {
					return _slider_events;
				}

				/// <summary>Check whether widget specs are equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are equal, else false.</returns>
				bool operator==(const slider& param);

				/// <summary>Check whether widget specs are NOT equal. Only those properties that
				/// require the widget resources to be re-created are considered.</summary>
				/// <param name="param">The specs to compare to.</param>
				/// <returns>Returns true if the specs are NOT equal, else false.</returns>
				bool operator!=(const slider& param);

				// generic widget

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				std::string& text() override;

				/// <summary>Set the widget's text.</summary>
				/// <param name="text">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& text(const std::string& text);

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				std::string& tooltip() override;

				/// <summary>Set the tooltip text.</summary>
				/// <param name="tooltip">The tooltip text.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& tooltip(const std::string& tooltip);

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				/// <remarks>The position is in reference to the widget's container.</remarks>
				lecui::rect& rect() override;

				/// <summary>Set the position and dimensions of the widget.</summary>
				/// <param name="rect">The widget's rectangle.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& rect(const lecui::rect& rect);

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				resize_params& on_resize() override;

				/// <summary>Set the behaviour of the widget when its container is resized.</summary>
				/// <param name="on_resize">The resize parameters as defined in <see cref="resize_params"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& on_resize(const resize_params& on_resize);

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				cursor_type& cursor() override;

				/// <summary>Set the mouse cursor to use when over the widget.</summary>
				/// <param name="cursor">The cursor type as defined in <see cref="cursor_type"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& cursor(const cursor_type cursor);

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				std::string& font() override;

				/// <summary>Set the default widget font.</summary>
				/// <param name="font">The font's name, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				slider& font(const std::string& font);

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size() override;

				/// <summary>Set the widget's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& font_size(const float& font_size);

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				color& color_text() override;

				/// <summary>Set the color of the widget's text.</summary>
				/// <param name="color_text">The color of the widget's text.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_text(const color& color_text);

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				color& color_fill() override;

				/// <summary>Set the fill color of the widget.</summary>
				/// <param name="color_fill">The fill color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_fill(const color& color_fill);

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				color& color_hot() override;

				/// <summary>Set the widget's color when the mouse is hovered over it.</summary>
				/// <param name="color_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_hot(const color& color_hot);

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				color& color_selected() override;

				/// <summary>Set the widget's color when selected.</summary>
				/// <param name="color_selected">The widget's color when selected.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_selected(const color& color_selected);

				/// <summary>Get or set the widget's color when it is disabled.</summary>
				/// <returns>a reference to the property.</returns>
				color& color_disabled() override;

				/// <summary>Set the widget's color when it is disabled.</summary>
				/// <param name="color_disabled">The color of the widget when it is disabled, as defined in <see cref='color'></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_border(const color& color_border);

				/// <summary>Get or set the color of the knob.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob();

				/// <summary>Set the color of the knob.</summary>
				/// <param name="color_knob">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_knob(const color& color_knob);

				/// <summary>Get or set the color of the knob when the mouse is over the slider.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob_hot();

				/// <summary>Set the color of the knob when the mouse is over the slider.</summary>
				/// <param name="color_knob_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_knob_hot(const color& color_knob_hot);

				/// <summary>Get or set the color of the knob's border.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob_border();

				/// <summary>Set the color of the knob's border.</summary>
				/// <param name="color_knob_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_knob_border(const color& color_knob_border);

				/// <summary>Get or set the color of the slider ticks.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tick();

				/// <summary>Set the color of the slider ticks.</summary>
				/// <param name="color_tick">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& color_tick(const color& color_tick);

				/// <summary>Get or set the property for whether to snap slider movements to the ticks.</summary>
				/// <returns>A reference to the property.</returns>
				bool& snap_to_ticks();

				/// <summary>Set the property for whether to snap slider movements to the ticks.</summary>
				/// <param name="snap_to_ticks">Whether to snap slider movements to the ticks.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& snap_to_ticks(const bool& snap_to_ticks);

				/// <summary>Get or set the property for whether to show ticks marks on the slider.</summary>
				/// <returns>A reference to the property.</returns>
				bool& show_tick_marks();

				/// <summary>Set the property for whether to show ticks marks on the slider.</summary>
				/// <param name="show_tick_marks">Whether to show tick marks on the slider.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& show_tick_marks(const bool& show_tick_marks);

				/// <summary>Get or set the property for whether to show tick labels on the slider.</summary>
				/// <returns>A reference to the property.</returns>
				bool& show_tick_labels();

				/// <summary>Set the property for whether to show tick labels on the slider.</summary>
				/// <param name="show_tick_labels">Whether to show tick labels on the slider.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& show_tick_labels(const bool& show_tick_labels);

				/// <summary>Get or set the slider's range.</summary>
				/// <returns>A reference to the range, as defined in <see cref="slider_range"></see>.</returns>
				slider_range& range();

				/// <summary>Set the slider's range.</summary>
				/// <param name="range">The range.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& range(const slider_range& range);

				/// <summary>Get or set the radius of the knob.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& knob_radius();

				/// <summary>Set the radius of the knob.</summary>
				/// <param name="knob_radius">The knob's radius, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& knob_radius(const float& knob_radius);

				/// <summary>Get or set the thickness of the slide.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& slide_thickness();

				/// <summary>Set the thickness of the slide.</summary>
				/// <param name="slide_thickness">The thickness of the slide, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& slide_thickness(const float& slide_thickness);

				/// <summary>Get or set the unit intervals for placing major ticks.</summary>
				/// <returns>A reference to the unit interval property.</returns>
				float& major_tick_unit();

				/// <summary>Set the unit intervals for placing major ticks, e.g. 25.f.</summary>
				/// <param name="major_tick_unit">The unit interval.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& major_tick_unit(const float& major_tick_unit);

				/// <summary>Get or set the number of minor ticks to place between
				/// successive major ticks.</summary>
				/// <returns>The number of ticks.</returns>
				long& minor_tick_count();

				/// <summary>Set the number of minor ticks to place between successive
				/// major ticks, e.g. 4.</summary>
				/// <param name="minor_tick_count">The number of ticks.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& minor_tick_count(const long& minor_tick_count);

				/// <summary>Get or set the position of the knob along the slide.</summary>
				/// <returns>A reference to the position.</returns>
				float& value();

				/// <summary>Set the position of the knob along the slide, e.g. 63.5f.</summary>
				/// <param name="value">The position of the knob.</param>
				/// <returns>A reference to the modified object.</returns>
				slider& value(const float& value);

			public:
				/// <summary>Add a slider to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "volume".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static slider& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a slider.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/tools_tab_pane/options_tab/volume".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static slider& get(form& fm, const std::string& path);

			private:
				/// <summary>Slider events.</summary>
				slider_events _slider_events;
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting slider widget. 'Get' method documentation applies.
#define get_slider(path) liblec::lecui::widgets::slider::get(*this, path)
#endif
