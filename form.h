//
// form.h - form interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			class status_pane_builder;
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
			class strength_bar_builder;

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
			class pane_impl;
			class tab_pane_impl;
		}
#endif

		/// <summary>Form position.</summary>
		/// <remarks>Offset members use 10 pixels.</remarks>
		enum class form_position {
			/// <summary>Center the form to the working area. The working area is the desktop, minus the taskbar (if visible).</summary>
			center_to_working_area,

			/// <summary>Center the form to its parent.</summary>
			center_to_parent,

			/// <summary>Pin the form to the top left corner of the working area.</summary>
			top_left,

			/// <summary>Pin the form to the top left corner of the working area, with an offset.</summary>
			top_left_offset,

			/// <summary>Pin the form to the bottom left corner of the working area.</summary>
			bottom_left,

			/// <summary>Pin the form to the bottom left corner of the working area, with an offset.</summary>
			bottom_left_offset,

			/// <summary>Pin the form to the top right corner of the working area.</summary>
			top_right,

			/// <summary>Pin the form to the top right corner of the working area, with an offset.</summary>
			top_right_offset,

			/// <summary>Pin the form to the bottom right corner of the working area.</summary>
			bottom_right,

			/// <summary>Pin the form to the bottom right corner of the working area, with an offset.</summary>
			bottom_right_offset,
		};

		/// <summary>Form base class. The user must inherit from this class and call one of the
		/// two constructors from its own constructor.</summary>
		/// <remarks>Fully DPI aware, but needs to be recreated if the system DPI changes while
		/// already open to rescale properly.</remarks>
		class lecui_api form {
		public:
			/// <summary>Form events.</summary>
			struct form_events {
				/// <summary>The handler to be called when the form's size changes.</summary>
				/// <remarks>The parameter contains the new size of the form. This should be interpreted in
				/// reference to the form's design size (its size at creation). More details about the form's
				/// design size can be found in the dimensions class under the controls.h header.</remarks>
				std::function<void(const lecui::size&)> size = nullptr;

				/// <summary>The handler to be called when files are dropped on the form.</summary>
				/// <remarks>The parameter contains the full path to the dropped file,
				/// including the file's extension. This handler needs to be set before the form is created.
				/// It is recommended that it be set in the <see cref="initialize"></see> event.</remarks>
				std::function<void(const std::string&)> drop_files = nullptr;

				/// <summary>The handler to be called when data is received from another instance.</summary>
				/// <remarks>The parameter will contain the received data.</remarks>
				std::function<void(const std::string&)> receive_data = nullptr;

				/// <summary>The handler to be called when the form's caption is clicked.</summary>
				/// <remarks>This handler needs to be set before the form is created. It is recommended that it
				/// be set in the <see cref="initialize"></see> event.</remarks>
				std::function<void()> caption;

				/// <summary>The handler to be called for all essential app initialization, like reading registry settings etc.</summary>
				/// <remarks>The handler should return false if an error occurs that makes it necessary to abort the application.
				/// In such a case, error information should be written to the parameter (note that it's a reference). When false
				/// is returned application initialization is aborted and the layout event is never called.</remarks>
				std::function<bool(std::string&)> initialize;

				/// <summary>The handler to be called for the form's layout.</summary>
				/// <remarks>If there's an error in the layout's implementation write the error information
				/// to the parameter (note that it's a reference), and return false. This information will
				/// be internally passed to the blocking <see cref="create"></see> method.
				/// It is recommended to add at least one page container to the form in this
				/// method and show it before returning from this method. This becomes the home page.
				/// All the form's pages can be made in this method but this is not recommended. It is
				/// recommended to create other pages in the handlers that attempt to open them so that
				/// they are created dynamically at runtime so that the app uses the least amount of
				/// resources at any given time.
				/// </remarks>
				std::function<bool(std::string&)> layout;

				/// <summary>Handler to be called after the <see cref="form_events::layout"></see> event just before
				/// the form is displayed. This is a good place to close the splash screen. The splash screen itself
				/// is best created in the <see cref="form_events::initialize"></see> event.</summary>
				std::function<void()> start;

				/// <summary>Handler to be called when either the close button on the top right is clicked, or when
				/// Alt+F4 is pressed on the keyboard. This is a good place to ask the user if they
				/// really wish to close the form, and then call <see cref="form::close"></see> if they confirm.</summary>
				/// <remarks>The <see cref="form::close"></see> method is called internally if this handler isn't set.</remarks>
				std::function<void()> close;

				/// <summary>Handler to be called after closing is confirmed, before the form's destructor is
				/// called. This is a good place to save user settings.</summary>
				/// <remarks>Avoid such logic errors as attempting to create widgets here.</remarks>
				std::function<void()> shutdown;
			};

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
			/// <remarks>This method causes the <see cref="create"></see> method to continue execution
			/// regardless of whether an existing instance is present. You would typically do this if you
			/// need your app to reach the <see cref="form_events::initialize"></see> event without the intention of
			/// allowing it to go as far as the <see cref="form_events::layout"></see> event, in which case you would call
			/// the <see cref="close"></see> method within the <see cref="form_events::initialize"></see> event. A typical use-case
			/// is when the app is calling itself during an update process. Not calling this method would
			/// mean the spawning of the new instance is not guaranteed since the previous may still be
			/// running when the new one reaches the instance checking logic. The best place to call this
			/// method is in the constructor of the class that inherits from this base class.</remarks>
			void force_instance();

			/// <summary>Create the form.</summary>
			/// <param name="error">Error information on fail.</param>
			/// <returns>True if successful, else false. If false, error information will be
			/// written to the error reference.</returns>
			/// <remarks>This is a blocking function and will only return when either the form is
			/// closed or in the event of an error during form creation.</remarks>
			[[nodiscard]] bool create(std::string& error);

			/// <summary>Close the form. When this is called all activity is stopped and the form
			/// is closed.</summary>
			/// <remarks>If called in the <see cref="form_events::initialize"></see> event then
			/// the <see cref="form_events::layout"></see> event never happens and the app is
			/// terminated gracefully.</remarks>
			void close();

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

			/// <summary>Check whether the form is visible.</summary>
			/// <returns>Returns true if the form is visible, even if it's minimized or not in the foreground, else returns false.</returns>
			bool visible();

			/// <summary>Show the form.</summary>
			/// <remarks>Neccessary to display a form that was hidden through a call to <see cref="hide"></see>.</remarks>
			void show();

			/// <summary>Check whether the form is minimized.</summary>
			/// <returns>Returns true if the form is minimized, else returns false.</returns>
			bool minimized();

			/// <summary>Prevent the form from being closed.</summary>
			/// <remarks>Typically useful when you need to perform a lengthy operation that should not or cannot be interrupted.
			/// To allow the form to be closed call the <see cref="allow_quit"></see> method.</remarks>
			void prevent_quit();

			/// <summary>Allow the form to be closed.</summary>
			/// <remarks>Required at some point after a call to <see cref="prevent_quit"></see> else 
			/// it will not be possible to close the form.</remarks>
			void allow_quit();

			/// <summary>Get or set form events.</summary>
			/// <returns>A reference to the form's events.</returns>
			form_events& events();

		private:
			class impl;
			impl& _d;

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
			friend class containers::status_pane_builder;
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
			friend class widgets::strength_bar_builder;

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
