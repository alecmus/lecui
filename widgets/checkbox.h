/*
** checkbox.h - checkbox widget interface
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
				/// <summary>Checkbox widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 20x20px.</remarks>
				class checkbox : public widget {
				public:
					checkbox() {
						color_fill = { 229, 229, 229, 255 };
						color_hot = { 240, 240, 240, 255 };
						color_selected = { 0, 120, 170, 100 };
					}
					std::string text_unchecked;
					std::string text_indeterminate;
					float border = .5f;
					color color_check = { 0, 120, 170, 255 };
					color color_border = { 150, 150, 150, 255 };

					enum class checkbox_status {
						checked,
						unchecked,
						indeterminate,
					} status = checkbox_status::indeterminate;

					/// <summary>Called when the checkbox is clicked.</summary>
					std::function<void(checkbox_status checked)> on_check = nullptr;

					bool operator==(const checkbox&);
					bool operator!=(const checkbox&);
				};
			}

			/// <summary>Checkbox widget.</summary>
			class lecui_api checkbox {
			public:
				checkbox(containers::page& page);
				~checkbox();

				/// <summary>Add a checkbox widget.</summary>
				/// <param name="alias">The in-page unique alias, e.g. "marital_status".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] widgets::specs::checkbox&
					add(const std::string& alias);

				/// <summary>Get the specifications of an existing checkbox.</summary>
				/// <param name="fm">The form containing the checkbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_control/tab_one/marital_status".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]] static widgets::specs::checkbox&
					specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				checkbox();
				checkbox(const checkbox&);
				checkbox& operator=(const checkbox&);
			};
		}
	}
}
