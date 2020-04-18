/*
** toggle.h - toggle widget interface
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
			/// <summary>Toggle widget.</summary>
			class lecui_api toggle {
			public:
				/// <summary>Toggle widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 20px. Recommended minimum
				/// width with defaults is 100px.</remarks>
				class toggle_specs : public specs {
				public:
					toggle_specs() {
						cursor = cursor_type::hand;
						rect.size({ 200.f, 20.f });
						text = "On";
					}
					std::string text_off = "Off";
					color color_on;
					color color_off;
					bool on = false;

					struct toggle_events : basic_events {
						/// <summary>Handler that's called when toggle is clicked or dragged. The
						/// boolean parameter indicates the toggle's new state.</summary>
						std::function<void(bool on)> toggle = nullptr;
					};

					toggle_events& events() {
						return toggle_events_;
					}

					bool operator==(const toggle_specs&);
					bool operator!=(const toggle_specs&);

				private:
					toggle_events toggle_events_;
				};

				/// <summary>Toggle constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "encryption".</param>
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
				/// "sample_page/tools_tab_pane/options_tab/encryption".</param>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static toggle_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				toggle();
				toggle(const toggle&);
				toggle& operator=(const toggle&);
			};
		}
	}
}
