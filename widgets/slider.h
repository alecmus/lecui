//
// slider.h - slider widget interface
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
			/// <summary>Slider widget specifications.</summary>
			/// <remarks>The slider's orientation is computed automatically based on the its
			/// size. If the width is greater than the height it's oriented horizontally, else
			/// vertically. With the defaults, the recommended minimum height for horizontal
			/// orientation is 35px and the recommended minimum with for vertical orientation
			/// is 40px.</remarks>
			class slider_specs : public specs {
			public:
				/// <summary>Slider range.</summary>
				struct slider_range {
					/// <summary>The minimum value of the slider.</summary>
					float minimum = 0.f;

					/// <summary>The maximum value of the slider.</summary>
					float maximum = 100.f;
				};

			private:
				float border_ = 1.f;
				color color_border_;
				color color_knob_;
				color color_knob_hot_;
				color color_knob_border_;
				color color_tick_;
				bool snap_to_ticks_ = true;
				bool show_tick_marks_ = true;
				bool show_tick_labels_ = true;
				slider_range range_;
				float knob_radius_ = 8.f;
				float slide_thickness_ = 6.f;
				float major_tick_unit_ = 25.f;
				long minor_tick_count_ = 4;
				float value_ = 0.f;

			public:
				slider_specs() {
					cursor_ = cursor_type::hand;
					rect_.size({ 200.f, 35.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct slider_events : basic_events {
					/// <summary>Handler that's called when slider is moved.</summary>
					/// <remarks>The parameter indicates the knob's new position.</remarks>
					std::function<void(float value)> slider = nullptr;
				};

				slider_events& events() {
					return slider_events_;
				}

				bool operator==(const slider_specs&);
				bool operator!=(const slider_specs&);

				// generic specs

				std::string& text() override { return text_; }
				slider_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				slider_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				slider_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				slider_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				slider_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				slider_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				slider_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				slider_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				slider_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				slider_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				slider_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				slider_specs& color_disabled(const color& color_disabled) {
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
				slider_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the color of the knob.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob() { return color_knob_; }

				/// <summary>Set the color of the knob.</summary>
				/// <param name="color_knob">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& color_knob(const color& color_knob) {
					color_knob_ = color_knob;
					return *this;
				}

				/// <summary>Get or set the color of the knob when the mouse is over the slider.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob_hot() { return color_knob_hot_; }

				/// <summary>Set the color of the knob when the mouse is over the slider.</summary>
				/// <param name="color_knob_hot">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& color_knob_hot(const color& color_knob_hot) {
					color_knob_hot_ = color_knob_hot;
					return *this;
				}

				/// <summary>Get or set the color of the knob's border.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_knob_border() { return color_knob_border_; }

				/// <summary>Set the color of the knob's border.</summary>
				/// <param name="color_knob_border">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& color_knob_border(const color& color_knob_border) {
					color_knob_border_ = color_knob_border;
					return *this;
				}

				/// <summary>Get or set the color of the slider ticks.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_tick() { return color_tick_; }

				/// <summary>Set the color of the slider ticks.</summary>
				/// <param name="color_tick">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& color_tick(const color& color_tick) {
					color_tick_ = color_tick;
					return *this;
				}

				/// <summary>Get or set the property for whether to snap slider movements to the ticks.</summary>
				/// <returns>A reference to the property.</returns>
				bool& snap_to_ticks() { return snap_to_ticks_; }

				/// <summary>Set the property for whether to snap slider movements to the ticks.</summary>
				/// <param name="snap_to_ticks">Whether to snap slider movements to the ticks.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& snap_to_ticks(const bool& snap_to_ticks) {
					snap_to_ticks_ = snap_to_ticks;
					return *this;
				}

				/// <summary>Get or set the property for whether to show ticks marks on the slider.</summary>
				/// <returns>A reference to the property.</returns>
				bool& show_tick_marks() { return show_tick_marks_; }

				/// <summary>Set the property for whether to show ticks marks on the slider.</summary>
				/// <param name="show_tick_marks">Whether to show tick marks on the slider.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& show_tick_marks(const bool& show_tick_marks) {
					show_tick_marks_ = show_tick_marks;
					return *this;
				}

				/// <summary>Get or set the property for whether to show tick labels on the slider.</summary>
				/// <returns>A reference to the property.</returns>
				bool& show_tick_labels() { return show_tick_labels_; }

				/// <summary>Set the property for whether to show tick labels on the slider.</summary>
				/// <param name="show_tick_labels">Whether to show tick labels on the slider.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& show_tick_labels(const bool& show_tick_labels) {
					show_tick_labels_ = show_tick_labels;
					return *this;
				}

				/// <summary>Get or set the slider's range.</summary>
				/// <returns>A reference to the range, as defined in <see cref="slider_range"></see>.</returns>
				slider_range& range() { return range_; }

				/// <summary>Set the slider's range.</summary>
				/// <param name="range">The range.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& range(const slider_range& range) {
					range_ = range;
					return *this;
				}

				/// <summary>Get or set the radius of the knob.</summary>
				/// <returns>A reference to the radius, in pixels.</returns>
				float& knob_radius() { return knob_radius_; }

				/// <summary>Set the radius of the knob.</summary>
				/// <param name="knob_radius">The knob's radius, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& knob_radius(const float& knob_radius) {
					knob_radius_ = knob_radius;
					return *this;
				}

				/// <summary>Get or set the thickness of the slide.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& slide_thickness() { return slide_thickness_; }

				/// <summary>Set the thickness of the slide.</summary>
				/// <param name="slide_thickness">The thickness of the slide, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& slide_thickness(const float& slide_thickness) {
					slide_thickness_ = slide_thickness;
					return *this;
				}

				/// <summary>Get or set the unit intervals for placing major ticks.</summary>
				/// <returns>A reference to the unit interval property.</returns>
				float& major_tick_unit() { return major_tick_unit_; }

				/// <summary>Set the unit intervals for placing major ticks, e.g. 25.f.</summary>
				/// <param name="major_tick_unit">The unit interval.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& major_tick_unit(const float& major_tick_unit) {
					major_tick_unit_ = major_tick_unit;
					return *this;
				}

				/// <summary>Get or set the number of minor ticks to place between
				/// successive major ticks.</summary>
				/// <returns>The number of ticks.</returns>
				long& minor_tick_count() { return minor_tick_count_; }

				/// <summary>Set the number of minor ticks to place between successive
				/// major ticks, e.g. 4.</summary>
				/// <param name="minor_tick_count">The number of ticks.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& minor_tick_count(const long& minor_tick_count) {
					minor_tick_count_ = minor_tick_count;
					return *this;
				}

				/// <summary>Get or set the position of the knob along the slide.</summary>
				/// <returns>A reference to the position.</returns>
				float& value() { return value_; }

				/// <summary>Set the position of the knob along the slide, e.g. 63.5f.</summary>
				/// <param name="value">The position of the knob.</param>
				/// <returns>A reference to the modified object.</returns>
				slider_specs& value(const float& value) {
					value_ = value;
					return *this;
				}

			private:
				slider_events slider_events_;
			};

			/// <summary>Slider widget builder.</summary>
			class lecui_api slider_builder {
			public:
				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				slider_builder(containers::page& page);

				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "volume".</param>
				slider_builder(containers::page& page, const std::string& alias);
				~slider_builder();

				/// <summary>Get the slider specifications.</summary>
				/// <returns>A reference to the slider specifications.</returns>
				[[nodiscard]]
				slider_specs& specs();

				/// <summary>Get the slider specifications.</summary>
				/// <returns>A reference to the slider specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				slider_specs& operator()();

				/// <summary>Get the specifications of a slider.</summary>
				/// <param name="fm">The form containing the slider.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/tools_tab_pane/options_tab/volume".</param>
				/// <returns>A reference to the slider specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static slider_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				slider_builder() = delete;
				slider_builder(const slider_builder&) = delete;
				slider_builder& operator=(const slider_builder&) = delete;
			};
		}
	}
}
