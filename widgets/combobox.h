//
// combobox.h - combobox widget interface
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
			/// <summary>Combobox widget.</summary>
			class lecui_api combobox {
			public:
				struct combobox_item {
					std::string label;
					std::string font = "Segoe UI";
					float font_size = 9.f;
				};

				/// <summary>Combobox widget specifications.</summary>
				/// <remarks>Recommended height with defaults is 25px</remarks>
				class combobox_specs : public specs {
				public:
					combobox_specs() {
						rect.size({ 200.f, 25.f });
					}

					/// <summary>Whether the combobox is editable.</summary>
					bool editable = false;

					/// <summary>The background color is it's editable.</summary>
					color color_fill_editable;

					/// <summary>The color of the caret.</summary>
					color color_caret;

					/// <summary>The color of the border.</summary>
					color color_border;

					/// <summary>The color of the dropdown.</summary>
					color color_dropdown;

					/// <summary>The color of the dropdown when the mouse is over it.</summary>
					color color_dropdown_hot;

					/// <summary>The color of the dropdown arrow.</summary>
					color color_dropdown_arrow;

					/// <summary>The color of the dropdown arrow when the mouse is over it.</summary>
					color color_dropdown_arrow_hot;

					/// <summary>The thickness of the border.</summary>
					float border = .5f;

					/// <summary>The horizontal radius of the corners.</summary>
					float corner_radius_x = 2.f;

					/// <summary>The vertical radius of the corners.</summary>
					float corner_radius_y = 2.f;

					/// <summary>The list of items available in the dropdown.</summary>
					std::vector<combobox_item> items;

					/// <summary>The quality of the combo item images.</summary>
					image_quality quality = image_quality::high;

					/// <summary>How to sort the items.</summary>
					sort_options sort = sort_options::ascending;

					/// <summary>Forces numerical sorting.</summary>
					/// <remarks>When only numbers are in the items list numerical sorting is
					/// used. The presence of at least one non-numeric character causes string
					/// sorting to be used instead. In those instances where you need numerical
					/// sorting to be used regardless of the presence of non-numeric characters
					/// this is the way to do it.</remarks>
					bool force_numerical_sort = false;

					/// <summary>An item from the list in the dropdown that was last selected.
					/// </summary>
					/// <remarks>Note that this is not always the same as the .text variable in an
					/// editable combobox. The .text in an editable combobox only becomes part of
					/// the list of items when the user pressed the enter key. Only then can it be
					/// considered as a "selected" item.</remarks>
					std::string selected;

					/// <summary>Events specific to this widget.</summary>
					struct combobox_events : basic_events {
						/// <summary>Called when a selection is made.</summary>
						/// <remarks>The parameter contains the selected item.</remarks>
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
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				combobox(containers::page& page);

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
				/// "sample_page/sample_tab_pane/tab_one/title".</param>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static combobox_specs& specs(form& fm, const std::string& path);

			private:
				class combobox_impl;
				combobox_impl& d_;

				// Default constructor and copying an object of this class are not allowed
				combobox() = delete;
				combobox(const combobox&) = delete;
				combobox& operator=(const combobox&) = delete;
			};
		}
	}
}
