//
// checkbox.h - checkbox widget interface
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
					}

					/// <summary>The text to display when unchecked.</summary>
					std::string text_unchecked;

					/// <summary>The text to display when in an indeterminate state.</summary>
					std::string text_indeterminate;

					/// <summary>The thickness of the border.</summary>
					float border = .5f;
					
					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The color of the check symbol.</summary>
					color color_check;

					/// <summary>Checkbox status.</summary>
					enum class checkbox_status {
						/// <summary>'Checked' status.</summary>
						checked,

						/// <summary>'Unchecked' status.</summary>
						unchecked,

						/// <summary>'Indeterminate' status.</summary>
						indeterminate,
					} status = checkbox_status::indeterminate;	// default to indeterminate

					/// <summary>Events specific to this widget.</summary>
					struct checkbox_events : basic_events {
						/// <summary>Called when the checkbox is clicked.</summary>
						/// <remarks>The parameter indicates the new checkbox status.</remarks>
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
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				checkbox(containers::page& page);

				/// <summary>Checkbox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "employment_status".</param>
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
				/// "sample_page/sample_tab_pane/tab_one/employment_status".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static checkbox_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				checkbox() = delete;
				checkbox(const checkbox&) = delete;
				checkbox& operator=(const checkbox&) = delete;
			};
		}
	}
}
