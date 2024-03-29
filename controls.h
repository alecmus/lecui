//
// controls.h - controls interface
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
		/// <summary>Manipulate a form's controls.</summary>
		/// <remarks>This manipulation is best done in the form's initialize event.</remarks>
		class lecui_api controls {
		public:
			/// <summary>Class constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			controls(form& fm);

			/// <summary>Class destructor.</summary>
			~controls();

			/// <summary>Whether to allow a form to be resizable.</summary>
			/// <param name="enable">Set to false to disable resizing.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>If resizing is enabled by default if this method is never called.
			/// </remarks>
			controls& allow_resize(bool enable);

			/// <summary>Whether to add a minimize control button to the form.</summary>
			/// <param name="enable">Set to false to remove the minimize control button.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The minimize control button is enabled by default if this method is never
			/// called.</remarks>
			controls& allow_minimize(bool enable);

			/// <summary>Whether to make this a top most form.</summary>
			/// <param name="enable">Set to false for a normal form and to true to make the form
			/// appear above all other windows.</param>
			/// <returns>A reference to the modified object.</returns>
			controls& make_top_most(bool enable);

			/// <summary>Whether to keep the form hidden after its creation.</summary>
			/// <param name="hidden">Set to false to keep normal form bahaviour and to true to initialize
			/// app with the form hidden. Useful when starting app in the background and when using the
			/// system tray.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>To hide a form after it has already been created use the
			/// <see cref="form::hide"></see> method. To restore a form after it's creation use the
			/// <see cref="form::restore"></see> method.</remarks>
			controls& start_hidden(bool hidden);

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			controls() = delete;
			controls(const controls&) = delete;
			controls& operator=(const controls&) = delete;
		};

		/// <summary>Manipulate a form's dimensions.</summary>
		/// <remarks>This manipulation is best done in the form's initialize event.</remarks>
		class lecui_api dimensions {
		public:
			/// <summary>Class constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			dimensions(form& fm);

			/// <summary>Class destructor.</summary>
			~dimensions();

			/// <summary>Measure how much space a label will take.</summary>
			/// <param name="formatted_text">The formatted text.</param>
			/// <param name="font">The font, e.g. Segoe UI.</param>
			/// <param name="font_size">The font size, in points, e.g. 9.0f.</param>
			/// <param name="alignment">The alignment of the text.</param>
			/// <param name="paragraph_alignment">The paragraph alignment.</param>
			/// <param name="max_rect">The area which the text should not exceed.</param>
			/// <returns>The rectangle that defines the optimal rectangle for the text.</returns>
			lecui::rect measure_label(const std::string& formatted_text,
				const std::string& font, const float font_size,
				text_alignment alignment, paragraph_alignment paragraph_alignment, const lecui::rect max_rect);

			/// <summary>Measure how much space an html widget will take.</summary>
			/// <param name="formatted_text">The formatted text.</param>
			/// <param name="font">The font, e.g. Segoe UI.</param>
			/// <param name="font_size">The font size, in points, e.g. 9.0f.</param>
			/// <param name="alignment">The alignment of the text.</param>
			/// <param name="editor">Whether this is an html_editor. Use false for an html_view.</param>
			/// <param name="max_rect">The area which the text should not exceed.</param>
			/// <returns>The rectangle that defines the optimal rectangle for the text.</returns>
			lecui::rect measure_html_widget(const std::string& formatted_text,
				const std::string& font, const float font_size,
				text_alignment alignment, const bool editor, const lecui::rect max_rect);

			/// <summary>Get the working area of the screen. This is the part of the screen that
			/// excludes the operating system's task bar.</summary>
			/// <returns>The working area rectangle.</returns>
			/// <remarks>As with all lecui methods, this method is also DPI aware.</remarks>
			[[nodiscard]] lecui::rect working_area();

			/// <summary>Get the position of the mouse cursor.</summary>
			/// <returns>The rectangle bounding the cursor.</returns>
			/// <remarks>This method is DPI aware.</remarks>
			[[nodiscard]] lecui::rect cursor_rect();

			/// <summary>Set the form's size.</summary>
			/// <param name="size">The size, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>To pre-set the form's design size, this manipulation should be done in the form's initialize event.</remarks>
			dimensions& set_size(const lecui::size& size);

			/// <summary>Get the form's size.</summary>
			/// <returns>The size, in pixels, as defined in <see cref='lecui::size'></see>.</returns>
			/// <remarks>Do not use this for laying out widgets on the form. Widgets are never
			/// placed directly onto a form, only into containers. Therefore, use the size
			/// of the containers for widget layout. Containers are never the same size as the
			/// form, some room is left for the form border, form controls, and depending on the
			/// container possibly scroll area etc. Refer to the container's documentation for
			/// more details.</remarks>
			[[nodiscard]] const lecui::size get_size();

			/// <summary>Get the form's design size.</summary>
			/// <returns>The size, in pixels, as defined in <see cref='lecui::size'></see>.</returns>
			/// <remarks>The design size is the the size that pages 'see' when they are added to the form.
			/// This is the size of the form minus any external resizing by the user or the operating system.</remarks>
			[[nodiscard]] const lecui::size get_design_size();

			/// <summary>Get the form's position.</summary>
			/// <returns>Returns the coordinates of the top left corner.</returns>
			[[nodiscard]] const lecui::point get_position();

			/// <summary>Set the form's minimum size.</summary>
			/// <param name="size">The size, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>This manipulation should be done in the form's layout method.</remarks>
			dimensions& set_minimum(const lecui::size& size);

			/// <summary>Get the form's minimum size.</summary>
			/// <returns>The size, in pixels.</returns>
			[[nodiscard]] const lecui::size& get_minimum();

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			dimensions() = delete;
			dimensions(const dimensions&) = delete;
			dimensions& operator=(const dimensions&) = delete;
		};
	}
}
