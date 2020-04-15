/*
** combo.h - combo widget interface
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
			/// <summary>Combo widget.</summary>
			class lecui_api combo {
			public:
				/// <summary>Combo widget specifications.</summary>
				class combo_specs : public specs {
				public:
					combo_specs() {
						color_fill = { 255, 255, 255, 0 };
						color_hot = { 200, 200, 200, 50 };
					}
					color color_border = { 0, 120, 170, 255 };
					color color_dropdown_hot = { 255, 255, 255, 255 };
					color color_menu = { 255, 255, 255, 255 };
					color color_menu_hot = { 200, 230, 255, 255 };
					color color_menu_selected = { 0, 120, 170, 255 };
					float border = .5f;
					float corner_radius_x = 2.f;
					float corner_radius_y = 2.f;
					std::vector<std::string> items;
					std::string selected;

					/// <summary>Called when the selection changes. The parameter contains the
					/// selected item.</summary>
					std::function<void(const std::string&)> on_selection = nullptr;

					bool operator==(const combo_specs&);
					bool operator!=(const combo_specs&);
				};

				/// <summary>Combo constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				combo(containers::page& page, const std::string& alias);
				~combo();

				/// <summary>Get the combo specifications.</summary>
				/// <returns>A reference to the combo specifications.</returns>
				[[nodiscard]]
				combo_specs& specs();

				/// <summary>Get the combo specifications.</summary>
				/// <returns>A reference to the combo specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				combo_specs& operator()();

				/// <summary>Get the specifications of a combo.</summary>
				/// <param name="fm">The form containing the combo.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/employed".</param>
				/// <returns>A reference to the combo specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static combo_specs& specs(form& fm, const std::string& path);

			private:
				class combo_impl;
				combo_impl& d_;

				// Default constructor and copying an object of this class are not allowed
				combo();
				combo(const combo&);
				combo& operator=(const combo&);
			};
		}
	}
}
