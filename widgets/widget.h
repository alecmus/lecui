/*
** widget.h - widget interface
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
#include "../form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Base class for all widget specifications.</summary>
			class specs {
			public:
				specs() {};
				virtual ~specs() {};

				/// <summary>Resize parameters.</summary>
				/// <remarks>Affects how the widget behaves when the immediate container's
				/// dimensions change.</remarks>
				struct resize_params {
					/// <summary>The percentage rate for following the container's right border.
					/// 0 = doesn't move horizontally, 100 = moves same number of pixels
					/// horizontally that the container's right border has moved.
					/// </summary>
					float perc_x = 0.f;

					/// <summary>The percentage rate for following the container's bottom border.
					/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
					/// that the container's bottom border has moved.</summary>
					float perc_y = 0.f;

					/// <summary>The percentage rate for following the container's change in width.
					/// 0 = doesn't follow change in container's width, 100 = width changes at 
					/// same rate as that of container.</summary>
					float perc_width = 0.f;

					/// <summary>The percentage rate for following the container's change in 
					/// height. 0 = doesn't follow change in container's height, 100 = height 
					/// changes at same rate as that of container.</summary>
					float perc_height = 0.f;

					/// Minimum coordinates permitted as widget moves
					float min_x = 0.f;
					float min_y = 0.f;

					/// Maximum coordinates permitted as widget moves
					float max_x = 0.f;
					float max_y = 0.f;

					/// Minimum dimensions permitted as widget shrinks
					float min_width = 0.f;
					float min_height = 0.f;

					/// Maximum dimensions permitted as widget grows
					float max_width = 0.f;
					float max_height = 0.f;

					bool operator==(const resize_params&);
					bool operator!=(const resize_params&);
				};

				/// <summary>Mouse cursor type.</summary>
				enum class cursor_type {
					arrow,
					hand,
					caret,
				};

				/// <summary>Events common to all widgets.</summary>
				struct basic_events {
					/// <summary>The handler to be called when either the space bar or enter key
					/// is pressed, or if the widget is clicked.</summary>
					std::function<void()> action = nullptr;

					/// <summary>The handler to be called when the widget is clicked.</summary>
					std::function<void()> click = nullptr;

					/// <summary>The handler to be called when the mouse enters the widget.</summary>
					std::function<void()> mouse_enter = nullptr;

					/// <summary>The handler to be called when the mouse leaves the widget.</summary>
					std::function<void()> mouse_leave = nullptr;
				};

				/// Content
				std::string text;

				/// Dimensions
				lecui::rect rect;
				resize_params on_resize;

				/// Widget cursor
				cursor_type cursor = cursor_type::arrow;

				/// Appearance
				std::string font = "Segoe UI";
				float font_size = 9.f;
				color color_text = { 0, 0, 0, 255 };
				color color_fill = { 0, 120, 170, 255 };
				color color_hot = { 0, 120, 170, 255 };
				color color_selected = { 0, 120, 170, 30 };
				color color_disabled = { 180, 180, 180, 255 };

				/// <summary>Widgets with additional events need to override this method.</summary>
				/// <returns>A reference to the widget's basic events.</returns>
				basic_events& events() {
					return events_;
				}

				bool operator==(const specs&);
				bool operator!=(const specs&);

			private:
				basic_events events_;
			};
		}

		/// <summary>Widget management.</summary>
		class lecui_api widget_management {
		public:
			widget_management(form& fm);
			~widget_management();

			/// <summary>Enable or disable a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="enable">Set to true to enable the widget, and false to disable it.
			/// </param>
			void enable(const std::string& path, bool enable);

			/// <summary>Show or hide a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="show">Set to true to make the widget visible, and false to hide it.
			/// </param>
			void show(const std::string& path, bool show);

			/// <summary>Close a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <remarks>Avoid closing a widget from within its own handler to avoid access
			/// violation errors. Also make sure to never keep 'dangling widget references'
			/// because attempting to use those references after this method is called will
			/// result in access violation errors as well. Make sure to employ proper logic. As a
			/// failsafe, the widget is not closed immediately when this method is called. Rather
			/// a special timer is scheduled internally to close the widget. After this, the widget
			/// is closed as soon as possible, typically soon after the current handler exits.
			/// </remarks>
			void close(const std::string& path);

			/// <summary>Select a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <remarks>Any previous selection within the page is removed.</remarks>
			void select(const std::string& path);

		private:
			class impl;
			impl& d_;

			// Default constructor and copying an object of this class are not allowed
			widget_management();
			widget_management(const widget_management&);
			widget_management& operator=(const widget_management&);
		};
	}
}
