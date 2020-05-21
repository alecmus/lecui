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
				struct slider_range {
					float min = 0.f;
					float max = 100.f;
				};

				/// <summary>Slider widget specifications.</summary>
				class slider_specs : public specs {
				public:
					slider_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 20.f });
					}
					color color_border;
					color color_knob;
					color color_knob_hot;
					color color_knob_border;
					color color_tick;
					float border = 1.f;
					bool snap_to_ticks = true;
					bool show_tick_marks = true;
					bool show_tick_labels = true;
					slider_range range;
					float knob_radius = 8.f;
					float slide_thickness = 6.f;
					float major_tick_unit = 25.f;
					long minor_tick_count = 4;
					float value = 0.f;
					
					struct slider_events : basic_events {
						/// <summary>Handler that's called when slider is moved. The
						/// float parameter indicates the sliders's new value.</summary>
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
				/// <param name="alias">The in-page unique alias, e.g. "encryption".</param>
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
				/// "sample_page/tools_tab_pane/options_tab/encryption".</param>
				/// <returns>A reference to the slider specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static slider_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				slider();
				slider(const slider&);
				slider& operator=(const slider&);
			};
		}
	}
}
