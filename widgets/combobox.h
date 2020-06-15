/*
** combobox.h - combobox widget interface
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
			/// <summary>Combobox widget.</summary>
			class lecui_api combobox {
			public:
				struct combobox_item {
					std::string label;
					std::string font = "Segoe UI";
				};

				/// <summary>Combobox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class combobox_specs : public specs {
				public:
					combobox_specs() {
						rect.size({ 200.f, 25.f });
					}
					color color_fill_editable;
					color color_caret;
					color color_border;
					color color_dropdown;
					color color_dropdown_hot;
					color color_dropdown_arrow;
					color color_dropdown_arrow_hot;
					bool editable = false;
					float border = .5f;
					float corner_radius_x = 2.f;
					float corner_radius_y = 2.f;

					/// <summary>The list of items available in the dropdown.</summary>
					std::vector<combobox_item> items;

					/// <summary>How to sort the items.</summary>
					sort_options sort = sort_options::ascending;

					/// <summary>An item from the list in the dropdown that was last selected.
					/// </summary>
					/// <remarks>Note that this is not always the same as the .text variable in an
					/// editable combobox. The .text in an editable combobox only becomes part of
					/// the list of items when the user pressed the enter key. Only then can it be
					/// considered as a "selected" item.</remarks>
					std::string selected;

					struct combobox_events : basic_events {
						/// <summary>Called when a selection is made. The parameter contains the
						/// selected item.</summary>
						std::function<void(const std::string&)> selection = nullptr;
					};

					combobox_events& events() {
						return combobox_events_;
					}

					bool operator==(const combobox_specs&);
					bool operator!=(const combobox_specs&);

				private:
					combobox_events combobox_events_;
				};

				/// <summary>Combobox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				combobox(containers::page& page, const std::string& alias);
				~combobox();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				[[nodiscard]]
				combobox_specs& specs();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				combobox_specs& operator()();

				/// <summary>Get the specifications of a combobox.</summary>
				/// <param name="fm">The form containing the combobox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/employed".</param>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static combobox_specs& specs(form& fm, const std::string& path);

			private:
				class combobox_impl;
				combobox_impl& d_;

				// Default constructor and copying an object of this class are not allowed
				combobox();
				combobox(const combobox&);
				combobox& operator=(const combobox&);
			};
		}
	}
}
