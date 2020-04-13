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
		namespace containers {
			class page;
		}

		namespace widgets {
			namespace specs {
				/// <summary>Toggle widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 20px. Recommended minimum
				/// width with defaults is 100px.</remarks>
				class toggle : public widget {
				public:
					toggle() { text = "On"; color_fill = { 255, 255, 255, 200 }; }
					std::string text_off = "Off";
					color color_on = { 0, 120, 170, 255 };
					color color_off = { 150, 150, 150, 255 };
					bool on = false;

					/// <summary>Handler that's called when toggle is clicked or dragged. The
					/// boolean parameter indicates the toggle's new state.</summary>
					std::function<void(bool on)> on_toggle = nullptr;

					bool operator==(const toggle&);
					bool operator!=(const toggle&);
				};
			}

			/// <summary>Toggle widget.</summary>
			class lecui_api toggle {
			public:
				toggle(containers::page& page);
				~toggle();

				/// <summary>Add a toggle widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "encryption".</param>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::toggle&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing toggle.</summary>
				/// <param name="fm">The form containing the toggle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/tools_tab_control/options_tab/encryption".</param>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::toggle&
					specs(form& fm, const std::string& path);

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
