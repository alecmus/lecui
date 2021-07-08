//
// form.h - form interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

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
		class page_manager;
		class widget_manager;
		class timer_manager;
		class dimensions;
		class appearance;
		class controls;
		class filesystem;
		class color_picker;
		class splash;
		class instance_manager;
		class form_menu;
		class tray_icon;

		namespace containers {
			class page;
			class status_pane;
			class tab_pane_builder;
			class tab_builder;
			class pane_builder;
			class group_builder;
		}

		namespace widgets {
			class label_builder;
			class image_view_builder;
			class button_builder;
			class custom_builder;
			class table_view_builder;
			class rectangle_builder;
			class toggle_builder;
			class progress_indicator_builder;
			class progress_bar_builder;
			class checkbox_builder;
			class text_field_builder;
			class password_field_builder;
			class tree_view_builder;
			class slider_builder;
			class html_editor_builder;
			class combobox_builder;
			class line_builder;
			class time_builder;
			class date_builder;
			class icon_builder;

			class widget_impl;
			class combobox_impl;
			class html_editor_impl;
			class text_field_impl;
			class tree_view_impl;
			class image_view_impl;
			class table_view_impl;
			class time_impl;
			class date_impl;
			class icon_impl;
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

			/// <summary>A special caption used to make a tooltip form.</summary>
			/// <returns>The internally reserved caption for tooltip forms.</returns>
			[[nodiscard]]
			static std::string tooltip_form_caption();

			/// <summary>Move the form.</summary>
			/// <param name="point">The coordinates of the top left corner of the form, in pixels.
			/// </param>
			void move(const point& point);

			/// <summary>Move the form.</summary>
			/// <param name="form_position">The predefined form position.</param>
			void move(const form_position& form_position);

			/// <summary>Force this instance to run even if an existing instance is found.</summary>
			/// <remarks>This method causes the <see cref="show"></see> method to continue execution
			/// regardless of whether an existing instance is present. You would typically do this if you
			/// need your app to reach the <see cref="initialize"></see> method without the intention of
			/// allowing it to go as far as <see cref="on_layout"></see>, in which case you would call
			/// the <see cref="close"></see> method within <see cref="initialize"></see>. A typical use-case
			/// is when the app is calling itself during an update process. Not calling this method would
			/// mean the spawning of the new instance is not guaranteed since the previous may still be
			/// running when the new one reaches the instance checking logic. The best place to call this
			/// method is in the constructor of the class that inherits from this base class.</remarks>
			void force_instance();

			/// <summary>Show the form.</summary>
			/// <param name="error">Error information on fail.</param>
			/// <returns>True if successful, else false. If false, error information will be
			/// written to the error reference.</returns>
			/// <remarks>This is a blocking function and will only return when either the form is
			/// closed or in the event of an error during form creation.</remarks>
			[[nodiscard]] bool show(std::string& error);

			/// <summary>Close the form. When this is called all activity is stopped and the form
			/// is closed.</summary>
			/// <remarks>If called in <see cref="on_initialize"></see> then on_layout is never called
			/// and the app is terminated gracefully.</remarks>
			void close();

			/// <summary>Called before on_layout(). This is a good place to create the splash
			/// screen and do all essential app initilization like reading registry settings etc.
			/// </summary>
			/// <param name="error">Error information on fail.</param>
			/// <returns>True if successful, else false. If false, error information will be
			/// written to the error reference.</returns>
			/// <remarks>Return false if you want to discountinue application initialization.
			/// This means <see cref="on_layout"></see> won't even be called.
			/// </remarks>
			virtual bool on_initialize(std::string& error);

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
			/// place to close the splash screen. The splash screen itself is best created in
			/// <see cref="on_initialize"></see>.</summary>
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
			/// <param name="tooltip">The (optional) tooltip text.</param>
			/// <remarks>The handler has to be set in on_layout() or before, else it will not
			/// register. Setting the handler in on_start() is too late because the form caption
			/// will have already been created by then.</remarks>
			void on_caption(std::function<void()>on_caption, const std::string& tooltip);

			/// <summary>Set the handler to be called when files are dropped on the form.</summary>
			/// <param name="on_drop_files">The handler. When it is called, the parameter will
			/// contain the full path to the dropped file, including the file's extension.</param>
			/// <remarks>If this method is never called the form will not accept dropped files. To
			/// stop accepting dropped files pass a nullptr to this method.</remarks>
			void on_drop_files(std::function<void(const std::string& file)>on_drop_files);

			/// <summary>Set the handler to be called when data is received from another instance.</summary>
			/// <param name="on_receive_data">The handler. When it is called, the parameter will
			/// contain the received data.</param>
			void on_receive_data(std::function<void(const std::string& data)>on_receive_data);

			/// <summary>Keep UI responsive during lengthy calls.</summary>
			/// <returns>Returns true to continue operation, and false to quit immediately.</returns>
			/// <remarks>In lengthy operations it is useful to call this method between successive
			/// steps in order to keep the UI responsive.</remarks>
			[[nodiscard]]
			static bool keep_alive();

			/// <summary>Get the current DPI scale.</summary>
			/// <returns>The current DPI scale.</returns>
			[[nodiscard]]
			float get_dpi_scale();

			/// <summary>Minimize the form.</summary>
			/// <remarks>This method does nothing if the form is already minimized or if it doesn't have the minimize button.</remarks>
			void allow_minimize();

			/// <summary>Restore the form.</summary>
			/// <remarks>If the form is minimized it will be restored. If it is maximized it will also be restored to normal size. If
			/// the form is already in normal form nothing will be done (except bring it to the foreground if it isn't already). If the
			/// form was hidded it will be shown.</remarks>
			void restore();

			/// <summary>Maximize the form.</summary>
			/// <remarks>This method does nothing if the form is already maximized or if it doesn't have the maximize button.</remarks>
			void maximize();

			/// <summary>Hide the form. Useful when using a system tray icon.</summary>
			/// <remarks>Note that once called the form can only be restored through a call to the
			/// <see cref="restore"></see> method. Besides that there will be no other means to make the form visible again. To
			/// manipulate a form's visibility at creation time use the <see cref="controls::start_hidden"></see> method.</remarks>
			void hide();

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			form() = delete;
			form(const form&) = delete;
			form& operator=(const form&) = delete;

#if defined(LECUI_EXPORTS)
			friend class page_manager;
			friend class widget_manager;
			friend class timer_manager;
			friend class dimensions;
			friend class appearance;
			friend class controls;
			friend class filesystem;
			friend class color_picker;
			friend class splash;
			friend class instance_manager;
			friend class form_menu;
			friend class tray_icon;

			friend class containers::page;
			friend class containers::status_pane;
			friend class containers::tab_pane_builder;
			friend class containers::tab_builder;
			friend class containers::pane_builder;
			friend class containers::group_builder;
			friend class widgets::label_builder;
			friend class widgets::image_view_builder;
			friend class widgets::button_builder;
			friend class widgets::custom_builder;
			friend class widgets::table_view_builder;
			friend class widgets::rectangle_builder;
			friend class widgets::toggle_builder;
			friend class widgets::progress_indicator_builder;
			friend class widgets::progress_bar_builder;
			friend class widgets::checkbox_builder;
			friend class widgets::text_field_builder;
			friend class widgets::tree_view_builder;
			friend class widgets::slider_builder;
			friend class widgets::html_editor_builder;
			friend class widgets::combobox_builder;
			friend class widgets::line_builder;
			friend class widgets::time_builder;
			friend class widgets::date_builder;
			friend class widgets::icon_builder;

			friend class widgets::widget_impl;
			friend class widgets::combobox_impl;
			friend class widgets::image_view_impl;
			friend class widgets::tree_view_impl;
			friend class widgets::time_impl;
			friend class widgets::date_impl;
			friend class widgets::html_editor_impl;
			friend class widgets::icon_impl;
#endif
		};
	}
}
