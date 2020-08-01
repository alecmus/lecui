/*
** form.h - form interface
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
#include "lecui.h"
#else
#include <liblec/lecui.h>
#endif

#include <functional>

namespace liblec {
	namespace lecui {
#if defined(LECUI_EXPORTS)
		class page_management;
		class widget_management;
		class timer_management;
		class dimensions;
		class appearance;
		class controls;
		class filesystem;
		class color_picker;
		class splash;

		namespace containers {
			class page;
			class status_pane;
			class tab_pane;
			class tab;
			class pane;
			class group;
		}

		namespace widgets {
			class label;
			class image_view;
			class button;
			class custom;
			class table_view;
			class rectangle;
			class toggle;
			class progress_indicator;
			class progress_bar;
			class checkbox;
			class text_field;
			class password_field;
			class tree_view;
			class slider;
			class html_editor;
			class combobox;
			class line;
			class time;
			class date;
			class icon;

			class widget_impl;
			class combobox_impl;
			class html_editor_impl;
			class text_field_impl;
			class tree_view_impl;
			class image_view_impl;
			class table_view_impl;
		}
#endif

		enum class form_position {
			/// offset members use 10 pixels
			center_to_working_area,
			center_to_parent,
			top_left,
			top_left_offset,
			bottom_left,
			bottom_left_offset,
			top_right,
			top_right_offset,
			bottom_right,
			bottom_right_offset,
		};

		/// <summary>Form base class. The user must inherit from this class and call one of the
		/// two constructors from its own constructor.</summary>
		/// <remarks>Fully DPI aware, but needs to be recreated if the system DPI changes while
		/// already open to rescale properly.</remarks>
		class lecui_api form {
		public:
			/// <summary>Make a form with no parent.</summary>
			/// <param name="caption">The form caption, displayed on the top left. Supports
			/// formatting like any other label widget. Care has to be taken that the formatting
			/// does not result in text of excessive height since the title bar's size
			/// is fixed. Default font size is 9px when none is specified in the formatting and
			/// it is not recommended to set a size greater than this for the above mentioned
			/// reason.</param>
			form(const std::string& caption);

			/// <summary>Make a modal form.</summary>
			/// <param name="caption">The form caption, displayed on the top left. Supports
			/// formatting like any other label widget. Care has to be taken that the formatting
			/// does not result in text of excessive height since the title bar's size
			/// is fixed. Default font size is 9px when none is specified in the formatting and
			/// it is not recommended to set a size greater than this for the above mentioned
			/// reason.</param>
			/// <param name="parent">The parent form. The user cannot click the parent form until
			/// the child form is closed. Useful for making input forms.</param>
			/// <remarks>The parent form remains active in the background, with all timers running.
			/// If it so happens that the parent is closed from one of these timers, all it's
			/// children will be forcibly closed first. A child is not allowed to continue if
			/// the parent is closing.</remarks>
			form(const std::string& caption, form& parent);
			~form();

			/// <summary>A special caption used to make a menu form.</summary>
			/// <returns>The internally reserved caption for menu forms.</returns>
			[[nodiscard]]
			static std::string menu_form_caption();

			/// <summary>Move the form.</summary>
			/// <param name="point">The coordinates of the top left corner of the form, in pixels.
			/// </param>
			void move(const point& point);

			/// <summary>Move the form.</summary>
			/// <param name="form_position">The predefined form position.</param>
			void move(const form_position& form_position);

			/// <summary>Show the form.</summary>
			/// <param name="error">Error information on fail.</param>
			/// <returns>True if successful, else false. If false, error information will be
			/// written to the error reference.</returns>
			/// <remarks>This is a blocking function and will only return when either the form is
			/// closed or in the event of an error during form creation.</remarks>
			[[nodiscard]] bool show(std::string& error);

			/// <summary>Close the form. When this is called all activity is stopped and the form
			/// is closed.</summary>
			void close();

			/// <summary>For defining the form's layout.</summary>
			/// <param name="error">If there's an error in the layout function's implementation
			/// write that error information to this reference, and return false. This information
			/// will be internally passed to the blocking show() method.</param>
			/// <returns>Return true if the layout is successful, else false.</returns>
			/// <remarks>It is recommended to add at least one page container to the form in this
			/// method and show it before returning from this method. This becomes the home page.
			/// All the form's pages can be made in this method but this is not recommended. It is
			/// recommended to create other pages in the handlers that attempt to open them so that
			/// they are created dynamically at runtime so that the app uses the least amount of
			/// resources.
			/// </remarks>
			[[nodiscard]] virtual bool on_layout(std::string& error);

			/// <summary>Called after on_layout() just before the form is displayed. This is a good
			/// place to close the splash screen. The splash screen itself is best created in the
			/// constructor of the class that inherits directly from this class.</summary>
			virtual void on_start();

			/// <summary>Called when either the close button on the top right is clicked, or when
			/// Alt+F4 is pressed on the keyboard. This is a good place to ask the user if they
			/// really wish to close the form, and then call close() if they confirm.</summary>
			/// <remarks>close() is called internally if this virtual method is never overriden.
			/// </remarks>
			virtual void on_close();

			/// <summary>Called after closing is confirmed, before the form's destructor is
			/// called. This is a good place to save user settings.</summary>
			/// <remarks>Avoid such logic errors as attempting to create widgets here.</remarks>
			virtual void on_shutdown();

			/// <summary>Prompt the user for confirmation of an action. Displays a small modal
			/// message box with yes-no buttons.</summary>
			/// <param name="question">The question to ask the user.</param>
			/// <returns>True if yes is selected, else no.</returns>
			/// <remarks>Is centered to the form, and inherits the caption from the form.</remarks>
			[[nodiscard]] bool prompt(const std::string& question);

			/// <summary>Display a message to the user in the form of a small modal message
			/// box with an ok button.</summary>
			/// <param name="message">The message to display.</param>
			/// <remarks>Is centered to the form, and inherits the caption from the form.</remarks>
			void message(const std::string& message);

			/// <summary>Update the form. A low resource refresh of the current page. Typically
			/// necessary only after adding a new widget at runtime to the currently displayed
			/// page to avoid a situation whereby the widget is only displayed the next time the
			/// mouse moves or the next time the tab key is pressed.
			/// </summary>
			void update();

			/// <summary>Reload the form and all its widgets. A high resource reload of the entire
			/// form's graphic resources.
			/// The following things happen when this method is called:
			/// 1. The form's graphic resources are all recreated.
			/// 2. The graphic resources for the currently displayed page are all recreated.
			/// 3. The graphic resources of all pages that have been added to the form but are not
			/// currently displayed are all recreated.
			/// </summary>
			/// <remarks>Depending on the computing platform, this can take a considerable amount
			/// of time in a large app, ranging from milliseconds to actual seconds. The better
			/// the app is designed the less time it will take, as such applications
			/// typically only create pages on-demand. However, even in a well designed app the
			/// difference with update() will be obvious if many pages are currently open. Note:
			/// this doesn't change the form's 'state', e.g. if a widget was closed it won't
			/// magically reappear. This reloads the current 'state'.
			/// </remarks>
			void reload();

			/// <summary>Set the handler to be called when the form's caption is clicked.</summary>
			/// <param name="on_caption">The handler.</param>
			/// <remarks>The handler has to be set in on_layout() or before, else it will not
			/// register. Setting the handler in on_start() is too late because the form caption
			/// will have already been created by then.</remarks>
			void on_caption(std::function<void()>on_caption);

			/// <summary>Set the handler to be called when files are dropped on the form.</summary>
			/// <param name="on_drop_files">The handler. When it is called, the parameter will
			/// contain the full path to the dropped file, including the file's extension.</param>
			/// <remarks>If this method is never called the form will not accept dropped files. To
			/// stop accepting dropped files pass a nullptr to this method.</remarks>
			void on_drop_files(std::function<void(const std::string& file)>on_drop_files);

			/// <summary>Keep UI responsive during lengthy calls.</summary>
			/// <returns>Returns true to continue operation, and false to quit immediately.</returns>
			/// <remarks>In lengthy operations it is useful to call this method between successive
			/// steps in order to keep the UI responsive.</remarks>
			[[nodiscard]]
			static bool keep_alive();

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			form() = delete;
			form(const form&) = delete;
			form& operator=(const form&) = delete;

#if defined(LECUI_EXPORTS)
			friend class page_management;
			friend class widget_management;
			friend class timer_management;
			friend class dimensions;
			friend class appearance;
			friend class controls;
			friend class filesystem;
			friend class color_picker;
			friend class splash;
			friend class containers::page;
			friend class containers::status_pane;
			friend class containers::tab_pane;
			friend class containers::tab;
			friend class containers::pane;
			friend class containers::group;
			friend class widgets::label;
			friend class widgets::image_view;
			friend class widgets::button;
			friend class widgets::custom;
			friend class widgets::table_view;
			friend class widgets::rectangle;
			friend class widgets::toggle;
			friend class widgets::progress_indicator;
			friend class widgets::progress_bar;
			friend class widgets::checkbox;
			friend class widgets::text_field;
			friend class widgets::tree_view;
			friend class widgets::slider;
			friend class widgets::html_editor;
			friend class widgets::combobox;
			friend class widgets::combobox_impl;
			friend class widgets::line;
			friend class widgets::time;
			friend class widgets::date;
			friend class widgets::icon;
			friend class widgets::image_view_impl;
#endif
		};
	}
}
