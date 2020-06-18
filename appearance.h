/*
** appearance.h - appearance interface
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
	#include "form.h"
#else
	#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Themes that are implemented within the library.</summary>
		enum class themes {
			/// <summary>The default theme. Best in bright environments.</summary>
			light,

			/// <summary>Easier on the eyes in darker environments.</summary>
			dark,
		};

		/// <summary>The item to color.</summary>
		enum class item {
			form,
			titlebar,
			accent,
			accent_hover,
			disabled,

			scrollbar,
			scrollbar_border,
			scrollbar_hover,
			scrollbar_pressed,

			label,
			label_selected,
			label_hover,
			label_pressed,
			label_disabled,

			button,
			button_border,
			button_hover,
			button_selected,

			checkbox,
			checkbox_border,
			checkbox_tick,
			checkbox_hover,
			checkbox_selected,

			progress_bar,
			progress_bar_border,

			progress_indicator,
			progress_indicator_text,
			progress_indicator_unfilled,

			textbox,
			textbox_border,
			textbox_disabled,
			textbox_prompt,
			textbox_selected,
			textbox_caret,

			table,
			table_border,
			table_alternate,
			table_row_hover,
			table_row_selected,
			table_text_header,
			table_header,
			table_grid,

			toggle,
			toggle_on,
			toggle_off,
			toggle_selected,

			tree,
			tree_border,
			tree_hover,
			tree_selected,

			tab_pane,
			tab_pane_border,
			tab_bar,
			tab_selected,

			pane,
			pane_border,

			image,
			image_border,
			image_hover,
			image_selected,

			rectangle,
			rectangle_border,
			rectangle_border_hot,
			rectangle_hover,
			rectangle_selected,

			group,
			group_border,

			slider,
			slider_border,
			slider_knob,
			slider_knob_hot,
			slider_knob_border,
			slider_tick,
			slider_selected,

			html_editor,
			html_editor_border,
			html_editor_disabled,
			html_editor_selected,
			html_editor_caret,

			combobox,
			combobox_editable,
			combobox_caret,
			combobox_border,
			combobox_hover,
			combobox_selected,
			combobox_dropdown,
			combobox_dropdown_hot,
			combobox_dropdown_arrow,
			combobox_dropdown_arrow_hot,
			
			line,
			line_hover,
			line_selected,
		};

		class lecui_api defaults {
		public:
			static lecui::color color(themes theme, item wc);
		};

		/// <summary>Manipulate the form's appearance.</summary>
		class lecui_api appearance {
		public:
			appearance(form& fm);
			~appearance();

			/// <summary>Whether to add a shadow to the form.</summary>
			/// <param name="enable">Set to false to disable the shadow.</param>
			/// <remarks>The shadow is enabled by default if this method is never called.</remarks>
			void shadow(const bool& enable);

			/// <summary>Set a preset theme.</summary>
			/// <param name="theme">The theme to set.</param>
			/// /// <remarks>This option has to be set early in on_layout(), or in the
			/// constructor. Changing this later is not yet supported.</remarks>
			void theme(themes theme);

			/// <summary>Get the preset theme.</summary>
			/// <returns>The theme.</returns>
			[[nodiscard]]
			themes theme();

			/// <summary>Set the theme color.</summary>
			/// <param name="color">The color of controls and the form border.</param>
			/// <param name="color_hot">The color on-hovering over a control.</param>
			/// <param name="color_disabled">The color when a control is disabled.</param>
			/// <param name="color_text">The text color.</param>
			void theme(const lecui::color& color,
				const lecui::color& color_hot,
				const lecui::color& color_disabled,
				const lecui::color& color_text);

			/// <summary>Set the form's background colors.</summary>
			/// <param name="color">The background color of the entire form.</param>
			/// <param name="color_titlebar">The background color of the titlebar.</param>
			void background(const lecui::color& color,
				const lecui::color& color_titlebar);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			appearance();
			appearance(const appearance&);
			appearance& operator=(const appearance&);
		};
	}
}
