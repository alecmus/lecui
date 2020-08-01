/*
** slider.h - slider widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Slider widget.</summary>
			class lecui_api slider {
			public:
				/// <summary>Slider range.</summary>
				struct slider_range {
					/// <summary>The minimum value of the slider.</summary>
					float minimum = 0.f;

					/// <summary>The maximum value of the slider.</summary>
					float maximum = 100.f;
				};

				/// <summary>Slider widget specifications.</summary>
				/// <remarks>The slider's orientation is computed automatically based on the its
				/// size. If the width is greater than the height it's oriented horizontally, else
				/// vertically. With the defaults, the recommended minimum height for horizontal
				/// orientation is 35px and the recommended minimum with for vertical orientation
				/// is 40px.</remarks>
				class slider_specs : public specs {
				public:
					slider_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 35.f });
					}

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The color of the knob.</summary>
					color color_knob;

					/// <summary>The color of the knob when the mouse is over the slider.</summary>
					color color_knob_hot;

					/// <summary>The color of the knob's border.</summary>
					color color_knob_border;

					/// <summary>The color of the slider ticks.</summary>
					color color_tick;

					/// <summary>The thickness of the border.</summary>
					float border = 1.f;

					/// <summary>Whether to snap slider movements to the ticks.</summary>
					bool snap_to_ticks = true;

					/// <summary>Whether to show ticks marks on the slider.</summary>
					bool show_tick_marks = true;

					/// <summary>Whether to show tick labels on the slider.</summary>
					bool show_tick_labels = true;

					/// <summary>The slider's range.</summary>
					slider_range range;

					/// <summary>The radius of the knob.</summary>
					float knob_radius = 8.f;

					/// <summary>The thickness of the slide.</summary>
					float slide_thickness = 6.f;

					/// <summary>The unit intervals for placing major ticks.</summary>
					float major_tick_unit = 25.f;

					/// <summary>The number of minor ticks to place between successive major
					/// ticks.</summary>
					long minor_tick_count = 4;
					
					/// <summary>The position of the knob along the slide.</summary>
					float value = 0.f;
					
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

				private:
					slider_events slider_events_;
				};

				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				slider(containers::page& page);

				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "volume".</param>
				slider(containers::page& page, const std::string& alias);
				~slider();

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
				slider() = delete;
				slider(const slider&) = delete;
				slider& operator=(const slider&) = delete;
			};
		}
	}
}
