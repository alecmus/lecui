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
		namespace widgets {
			/// <summary>Checkbox widget.</summary>
			class lecui_api checkbox {
			public:
				/// <summary>Checkbox widget specifications.</summary>
				/// <remarks>Recommended size with defaults is 20x20px.</remarks>
				class checkbox_specs : public specs {
				public:
					checkbox_specs() {
						cursor = cursor_type::hand;
						rect.size({ 20.f, 20.f });
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

					struct checkbox_events : basic_events {
						/// <summary>Called when the checkbox is clicked.</summary>
						std::function<void(checkbox_status checked)> check = nullptr;
					};

					checkbox_events& events() {
						return checkbox_events_;
					}

					bool operator==(const checkbox_specs&);
					bool operator!=(const checkbox_specs&);

				private:
					checkbox_events checkbox_events_;
				};

				/// <summary>Checkbox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "employed".</param>
				checkbox(containers::page& page, const std::string& alias);
				~checkbox();

				/// <summary>Get the checkbox specifications.</summary>
				/// <returns>A reference to the checkbox specifications.</returns>
				[[nodiscard]]
				checkbox_specs& specs();

				/// <summary>Get the checkbox specifications.</summary>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				checkbox_specs& operator()();

				/// <summary>Get the specifications of a checkbox.</summary>
				/// <param name="fm">The form containing the checkbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/employed".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static checkbox_specs& specs(form& fm, const std::string& path);

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
