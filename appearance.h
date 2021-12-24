//
// appearance.h - appearance interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

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
			progress_bar_unfilled,

			progress_indicator,
			progress_indicator_text,
			progress_indicator_unfilled,

			text_field,
			text_field_border,
			text_field_disabled,
			text_field_prompt,
			text_field_selected,
			text_field_caret,
			text_field_input_error,

			table_view,
			table_view_border,
			table_view_alternate,
			table_view_row_hover,
			table_view_row_selected,
			table_view_text_header,
			table_view_text_header_hot,
			table_view_text_selected,
			table_view_header,
			table_view_grid,

			toggle,
			toggle_on,
			toggle_off,
			toggle_selected,

			tree_view,
			tree_view_border,
			tree_view_hover,
			tree_view_selected,

			tab_pane,
			tab_pane_border,
			tab_bar,
			tab_selected,

			pane,
			pane_border,

			image_view,
			image_view_border,
			image_view_hover,
			image_view_selected,

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
			html_control,
			html_control_border,

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

			icon,
			icon_border,
			icon_hot,
			icon_text,
			icon_description_text,

			strength_bar,
			strength_bar_border,

			badge,
			badge_border,
			badge_text,
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

			/// <summary>Set a preset theme.</summary>
			/// <param name="theme">The theme to set.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>This option has to be set early in on_layout(), or in the
			/// constructor. Changing this later is not yet supported.</remarks>
			appearance& theme(themes theme);

			/// <summary>Get the preset theme.</summary>
			/// <returns>The theme.</returns>
			[[nodiscard]]
			themes theme();

			/// <summary>Set the theme color.</summary>
			/// <param name="color">The color of controls and the form border.</param>
			/// <param name="color_hot">The color on-hovering over a control.</param>
			/// <param name="color_disabled">The color when a control is disabled.</param>
			/// <param name="color_text">The text color.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& theme(const lecui::color& color,
				const lecui::color& color_hot,
				const lecui::color& color_disabled,
				const lecui::color& color_text);

			/// <summary>Set or get the form shadow property.</summary>
			/// <returns>A reference to the property.</returns>
			bool& shadow();

			/// <summary>Get the status of the form shadow.</summary>
			/// <returns>The status of the form shadow.</returns>
			bool get_shadow() const;

			/// <summary>Set the form shadow property.</summary>
			/// <param name="enable">Set to false to disable the shadow.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The shadow is enabled by default if this method is never called.</remarks>
			appearance& shadow(const bool& enable);

			/// <summary>Get or set the form's background color.</summary>
			/// <returns>Returns a reference to the property.</returns>
			lecui::color& background();

			/// <summary>Get or the form's background color.</summary>
			/// <returns>The value of the property.</returns>
			lecui::color get_background() const;

			/// <summary>Set the form's background colors.</summary>
			/// <param name="color">The background color of the entire form.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& background(const lecui::color& color);

			/// <summary>Get or set the background color of the title bar.</summary>
			/// <returns>A reference to the property.</returns>
			lecui::color& title_bar();

			/// <summary>Get the background color of the title bar.</summary>
			/// <returns>The value of the property.</returns>
			lecui::color title_bar() const;

			/// <summary>Set the background color of the titlebar.</summary>
			/// <param name="color">The background color of the titlebar.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& title_bar(const lecui::color& color);

			/// <summary>Get or set the ID of the main icon resource.</summary>
			/// <returns>A reference to the main .ico resource ID.</returns>
			int& main_icon();

			/// <summary>Get the ID of the main icon resource.</summary>
			/// <returns>The resource ID of the main .ico.</returns>
			int get_main_icon() const;

			/// <summary>Set the ID of the main .ico resource.</summary>
			/// <param name="main_icon">The ID of the main .ico resource.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& main_icon(const int& main_icon);

			/// <summary>Get or set the ID of the small icon resource.</summary>
			/// <returns>A reference to the small .ico resource ID.</returns>
			int& mini_icon();

			/// <summary>Get the ID of the small icon resource.</summary>
			/// <returns>The resource ID of the small .ico.</returns>
			int get_mini_icon() const;

			/// <summary>Set the ID of the small .ico resource.</summary>
			/// <param name="mini_icon">The ID of the small .ico resource.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& mini_icon(const int& mini_icon);

			/// <summary>Get or set the ID of the caption icon resource.</summary>
			/// <returns>A reference to the caption icon .png resource ID.</returns>
			int& caption_icon();

			/// <summary>Get the ID of the caption icon resource.</summary>
			/// <returns>The resource ID of the caption icon .png.</returns>
			int get_caption_icon() const;

			/// <summary>Set the ID of the caption icon resource.</summary>
			/// <param name="caption_icon">The resource ID of the caption icon .png.</param>
			/// <returns>A reference to the modified object.</returns>
			appearance& caption_icon(const int& caption_icon);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			appearance() = delete;
			appearance(const appearance&) = delete;
			appearance& operator=(const appearance&) = delete;
		};
	}
}
