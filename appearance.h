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
			light,		///< The default theme. Best in bright environments.
			dark,		///< Easier on the eyes in darker environments.
		};

		/// <summary>The element to color.</summary>
		/// <remarks>All hover colors apply to non-static widgets. Refer to widget documentation for more information.</remarks>
		enum class element {
			form,							///< The background color of the form.
			titlebar,						///< The fill color of the form's title bar.
			accent,							///< The accent color.
			accent_hover,					///< The accent color on hover.
			disabled,						///< The color of disabled controls.
			
			scrollbar,						///< The fill color of scrollbars.
			scrollbar_border,				///< The color of scrollbar borders.
			scrollbar_hover,				///< The fill color of scrollbars when the mouse hovers over them.
			scrollbar_pressed,				///< The fill color of scrollbars when they are pressed.


			label,							///< The color of labels (text color).
			label_selected,					///< The background color of selected labels.
			label_hover,					///< The background color of labels when the mouse hovers over them.
			label_pressed,					///< The background color of labels when they are pressed.
			label_disabled,					///< The color of disabled labels (text color).

			button,							///< The fill color of buttons.
			button_border,					///< The color of button borders.
			button_hover,					///< The color of buttons when the mouse hovers over them.
			button_selected,				///< The border color of selected buttons.

			checkbox,						///< The fill color of checkboxes.
			checkbox_border,				///< The color of checkbox borders.
			checkbox_tick,					///< The color of checkbox ticks.
			checkbox_hover,					///< The fill color of checkboxes when the mouse hovers over them.
			checkbox_selected,				///< The border color of selected checkboxes.

			progress_bar,					///< The color of the filled portion progress bars.
			progress_bar_border,			///< The color of progress bar borders.
			progress_bar_unfilled,			///< The color of the un-filled portion of the progress bar.

			progress_indicator,				///< The line color of the filled portion of progress indicators (including the dot).
			progress_indicator_text,		///< The color of progress indicator text.
			progress_indicator_unfilled,	///< The line color of the un-filled portion of progress indicators.

			text_field,						///< The fill color of text fields.
			text_field_border,				///< The color of text field borders.
			text_field_disabled,			///< The color of a disabled text field.
			text_field_prompt,				///< The color of prompt text in a text field.
			text_field_selected,			///< The border color of a selected text field.
			text_field_caret,				///< The color of a text field caret.
			text_field_input_error,			///< The border color of a text field when an invalid input is detected.

			table_view,						///< The background color of table view odd rows.
			table_view_border,				///< The color of a table view borders.
			table_view_alternate,			///< The alternate color of table view even rows.
			table_view_row_hover,			///< The color of a row when the mouse hovers over it.
			table_view_row_selected,		///< The background color of a selected table view row.
			table_view_text_header,			///< The color of table view header text.
			table_view_text_header_hot,		///< The color of table view header text when the mouse is hovered over it.
			table_view_text_selected,		///< The border color of a selected table view.
			table_view_header,				///< The background color of table view headers.
			table_view_grid,				///< The line color of table view grids.

			toggle,							///< The color of the toggle circle.
			toggle_on,						///< The background color of toggles when they're on.
			toggle_off,						///< The background color of toggles when they're off.
			toggle_selected,				///< The background color of toggle labels when the toggle is selected.

			tree_view,						///< The background color of tree views.
			tree_view_border,				///< The color of tree view borders.
			tree_view_hover,				///< The background color of tree view labels when the mouse is hovered over them.
			tree_view_selected,				///< The border color of a selected tree view.

			tab_pane,						///< The background color of tab panes.
			tab_pane_border,				///< The border color of tab panes.
			tab_pane_tabs,					///< The background color of tabs.
			tab_pane_selected_tab,			///< The color of the bar that indicates the selected tab.

			pane,							///< The background color of panes.
			pane_border,					///< The border color of panes.

			image_view,						///< The background color of image views.
			image_view_border,				///< The border color of image views.
			image_view_hover,				///< The border color of image views when the mouse is hovered over them.
			image_view_selected,			///< The border color of a selected image view.

			rectangle,						///< The background color of rectangles.
			rectangle_border,				///< The border color of rectangles.
			rectangle_border_hot,			///< The border color of rectangles when the mouse is hovered over them.
			rectangle_hover,				///< The background color of rectangles when the mouse is hovered over them.
			rectangle_selected,				///< The border color of a selected rectangle.

			group,							///< The background color of groups.
			group_border,					///< The border color of groups.

			slider,							///< The background color of a slider.
			slider_border,					///< The border color of a slider.
			slider_knob,					///< The color of a slider's knob.
			slider_knob_hot,				///< The color of a slider's knob when the mouse is hovered over it.
			slider_knob_border,				///< The border color of a slider's knob.
			slider_tick,					///< The color of a slider's tick lines.
			slider_selected,				///< The border color of a slider's knob when the slider is selected.

			html_editor,					///< The background color of html editors.
			html_editor_border,				///< The border color of html editors.
			html_editor_disabled,			///< The background color of disabled html editors.
			html_editor_selected,			///< The border color of a selected html editor.
			html_editor_caret,				///< The color of the caret in the html editor.
			html_control,					///< The background color of the html editor control pane.
			html_control_border,			///< The border color of the html editor control pane.

			combobox,						///< The background color of non-editable comboboxes.
			combobox_editable,				///< The background color of editable comboboxes.
			combobox_caret,					///< The color of the cared in editable comboboxes.
			combobox_border,				///< The border color of comboboxes.
			combobox_hover,					///< The background color of comboboxes when the mouse is hovered over them.
			combobox_selected,				///< The border color of a selected combobox.
			combobox_dropdown,				///< The background color of a combobox dropdown.
			combobox_dropdown_hot,			///< The background color of a combobox dropdown when the mouse is hovered over it.
			combobox_dropdown_arrow,		///< The line color of the combobox dropdown arrow.
			combobox_dropdown_arrow_hot,	///< The line color of the combobox dropdown when the mouse is hovered over it.
			
			line,							///< The color of lines.
			line_hover,						///< The color of lines when the mouse is hovered over them.
			line_selected,					///< The color of a selected line.

			icon,							///< The background color of icons.
			icon_border,					///< The color of icon borders.
			icon_hot,						///< The background color of icons when the mouse is hovered over them.
			icon_text,						///< The color of icon main text.
			icon_description_text,			///< The color of icon description text.

			strength_bar,					///< The background color of strength bars.
			strength_bar_border,			///< The border color of strength bars.

			badge,							///< The background color of badges.
			badge_border,					///< The border color of badges.
			badge_text,						///< The color of badge text.
		};

		/// <summary>lecui defaults class.</summary>
		class lecui_api defaults {
		public:
			/// <summary>Get default lecui color.</summary>
			/// <param name="theme">The ui theme, as defined in <see cref='themes'></see>.</param>
			/// <param name="element">The ui element, as defined in <see cref='element'></see>.</param>
			/// <returns></returns>
			static lecui::color color(themes theme, element element);
		};

		/// <summary>Manipulate the form's appearance.</summary>
		class lecui_api appearance {
		public:
			/// <summary>Class constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			appearance(form& fm);

			/// <summary>Class destructor.</summary>
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
