/*
** controls.h - controls interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
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
		/// <summary>Manipulate a form's controls.</summary>
		/// <remarks>This manipulation should be done in the form's layout method.</remarks>
		class lecui_api controls {
		public:
			controls(form& fm);
			~controls();

			/// <summary>Whether to allow a form to be resizable.</summary>
			/// <param name="enable">Set to false to disable resizing.</param>
			/// <remarks>For resizing is enabled by default if this method is never called.
			/// </remarks>
			void resize(bool enable);

			/// <summary>Whether to add a minimize control button to the form.</summary>
			/// <param name="enable">Set to false to remove the minimize control button.</param>
			/// <remarks>The minimize control button is enabled by default if this method is never
			/// called.</remarks>
			void minimize(bool enable);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			controls() = delete;
			controls(const controls&) = delete;
			controls& operator=(const controls&) = delete;
		};

		/// <summary>Manipulate a form's dimensions.</summary>
		class lecui_api dimensions {
		public:
			dimensions(form& fm);
			~dimensions();

			/// <summary>Measure how much space a label will take.</summary>
			/// <param name="formatted_text">The formatted text.</param>
			/// <param name="font">The font, e.g. Segoe UI.</param>
			/// <param name="font_size">The font size, in points, e.g. 9.0f.</param>
			/// <param name="center_h">Whether to center the text horizontally within the area.</param>
			/// <param name="center_v">Whether to center the text vertically within the area.</param>
			/// <param name="max_rect">The area which the text should not exceed.</param>
			/// <returns>The rectangle that defines the optimal rectangle for the text.</returns>
			lecui::rect measure_label(const std::string& formatted_text,
				const std::string& font, const float font_size,
				bool center_h, bool center_v, const lecui::rect max_rect);

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
			/// <remarks>This manipulation should be done in the form's layout method.</remarks>
			void size(const lecui::size& size);

			/// <summary>Get the form's size.</summary>
			/// <returns>The size, in pixels.</returns>
			/// <remarks>Do not use this for laying out widgets on the form. Widgets are never
			/// placed directly onto a form, only into containers. Therefore, use the size
			/// of the containers for widget layout. Containers are never the same size as the
			/// form, some room is left for the form border, form controls, and depending on the
			/// container possibly scroll area etc. Refer to the container's documentation for
			/// more details.</remarks>
			[[nodiscard]] const lecui::size& size();

			/// <summary>Set the form's minimum size.</summary>
			/// <param name="size">The size, in pixels.</param>
			/// <remarks>This manipulation should be done in the form's layout method.</remarks>
			void minimum(const lecui::size& size);

			/// <summary>Get the form's minimum size.</summary>
			/// <returns>The size, in pixels.</returns>
			[[nodiscard]] const lecui::size& minimum();

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			dimensions() = delete;
			dimensions(const dimensions&) = delete;
			dimensions& operator=(const dimensions&) = delete;
		};
	}
}
