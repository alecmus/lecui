//
// widget.h - widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../form.h"
#include "../containers/page.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Resize parameters.</summary>
			/// <remarks>Affects how the widget behaves when the immediate container's
			/// dimensions change.</remarks>
			class lecui_api resize_params {
				float _perc_x = 0.f;
				float _perc_y = 0.f;
				float _perc_width = 0.f;
				float _perc_height = 0.f;
				float _min_x = 0.f;
				float _min_y = 0.f;
				float _max_x = 0.f;
				float _max_y = 0.f;
				float _min_width = 0.f;
				float _min_height = 0.f;
				float _max_width = 0.f;
				float _max_height = 0.f;

			public:
				/// <summary>Constructor.</summary>
				/// <remarks>Initializes with all fields set to zero.</remarks>
				resize_params();

				/// <summary>Constructor that allows initializing the most important properties.</summary>
				/// <param name="perc_x">The percentage rate for following the container's right border.</param>
				/// <param name="perc_y">The percentage rate for following the container's bottom border.</param>
				/// <param name="perc_width">The percentage rate for following the container's change in width.</param>
				/// <param name="perc_height">The percentage rate for following the container's change in 
				/// height.</param>
				/// <remarks>Initializes the rest of the fields set to zero.</remarks>
				resize_params(const float perc_x, const float perc_y, const float perc_width, const float perc_height);

				/// <summary>Get or set the percentage rate for following the container's right border.
				/// 0 = doesn't move horizontally, 100 = moves same number of pixels
				/// horizontally that the container's right border has moved.</summary>
				/// <returns>A reference to the property.</returns>
				float& perc_x();

				/// <summary>Get the percentage rate for following the container's right border.
				/// 0 = doesn't move horizontally, 100 = moves same number of pixels
				/// horizontally that the container's right border has moved.</summary>
				/// <returns>The value of the property.</returns>
				float get_perc_x() const;

				/// <summary>Set the percentage rate for following the container's right border.
				/// 0 = doesn't move horizontally, 100 = moves same number of pixels
				/// horizontally that the container's right border has moved.</summary>
				/// <param name="perc_x">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& perc_x(const float& perc_x);

				/// <summary>Get or set the percentage rate for following the container's bottom border.
				/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
				/// that the container's bottom border has moved.</summary>
				/// <returns>A reference to the property.</returns>
				float& perc_y();

				/// <summary>Get the percentage rate for following the container's bottom border.
				/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
				/// that the container's bottom border has moved.</summary>
				/// <returns>The value of the property.</returns>
				float get_perc_y() const;

				/// <summary>Set the percentage rate for following the container's bottom border.
				/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
				/// that the container's bottom border has moved.</summary>
				/// <param name="perc_y">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& perc_y(const float& perc_y);

				/// <summary>Get or set the percentage rate for following the container's change in width.
				/// 0 = doesn't follow change in container's width, 100 = width changes at 
				/// same rate as that of container.</summary>
				/// <returns>A reference to the property.</returns>
				float& perc_width();

				/// <summary>Get the percentage rate for following the container's change in width.
				/// 0 = doesn't follow change in container's width, 100 = width changes at 
				/// same rate as that of container.</summary>
				/// <returns>The value of the property.</returns>
				float get_perc_width() const;

				/// <summary>Set the percentage rate for following the container's change in width.
				/// 0 = doesn't follow change in container's width, 100 = width changes at 
				/// same rate as that of container.</summary>
				/// <param name="perc_width">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& perc_width(const float& perc_width);

				/// <summary>Get or set the percentage rate for following the container's change in 
				/// height. 0 = doesn't follow change in container's height, 100 = height 
				/// changes at same rate as that of container.</summary>
				/// <returns>A reference to the property.</returns>
				float& perc_height();

				/// <summary>Get the percentage rate for following the container's change in 
				/// height. 0 = doesn't follow change in container's height, 100 = height 
				/// changes at same rate as that of container.</summary>
				/// <returns>The value of the property.</returns>
				float get_perc_height() const;

				/// <summary>Set the percentage rate for following the container's change in 
				/// height. 0 = doesn't follow change in container's height, 100 = height 
				/// changes at same rate as that of container.</summary>
				/// <param name="perc_height">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& perc_height(const float& perc_height);

				/// <summary>Get or set the minimum x-coordinate permitted as widget moves.</summary>
				/// <returns>A reference to the property.</returns>
				float& min_x();

				/// <summary>Get the minimum x-coordinate permitted as widget moves.</summary>
				/// <returns>The value of the property.</returns>
				const float get_min_x() const;

				/// <summary>Set the minimum x-coordinate permitted as widget moves.</summary>
				/// <param name="min_x">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& min_x(const float& min_x);

				/// <summary>Get or set the minimum y-coordinate permitted as widget moves.</summary>
				/// <returns>A reference to the property.</returns>
				float& min_y();

				/// <summary>Get minimum y-coordinate permitted as widget moves.</summary>
				/// <returns>The value of the property.</returns>
				float get_min_y() const;

				/// <summary>Set the minimum y-coordinate permitted as widget moves.</summary>
				/// <param name="min_y">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& min_y(const float& min_y);

				/// <summary>Get or set the maximum x-coordinate permitted as widget moves.</summary>
				/// <returns>A reference to the property.</returns>
				float& max_x();

				/// <summary>Get the maximum x-coordinate permitted as widget moves.</summary>
				/// <returns>The value of the property.</returns>
				float get_max_x() const;

				/// <summary>Set the maximum x-coordinate permitted as widget moves.</summary>
				/// <param name="max_x">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& max_x(const float& max_x);

				/// <summary>Get or set the maximum y-coordinate permitted as widget moves.</summary>
				/// <returns>A reference to the property.</returns>
				float& max_y();

				/// <summary>Get maximum y-coordinate permitted as widget moves.</summary>
				/// <returns>The value of the property.</returns>
				float get_max_y() const;

				/// <summary>Set the maximum y-coordinate permitted as widget moves.</summary>
				/// <param name="max_y">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& max_y(const float& max_y);

				/// <summary>Get or set the minimum width permitted as widget is resized.</summary>
				/// <returns>A reference to the property.</returns>
				float& min_width();

				/// <summary>Get the minimum width permitted as widget is resized.</summary>
				/// <returns>The value of the property.</returns>
				float get_min_width() const;

				/// <summary>Set the minimum width permitted as widget is resized.</summary>
				/// <param name="min_width">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& min_width(const float& min_width);

				/// <summary>Get or set the minimum height permitted as widget is resized.</summary>
				/// <returns>A reference to the property.</returns>
				float& min_height();

				/// <summary>Get the minimum height permitted as widget is resized.</summary>
				/// <returns>The value of the property.</returns>
				float get_min_height() const;

				/// <summary>Set the minimum height permitted as widget is resized.</summary>
				/// <param name="min_height">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& min_height(const float& min_height);

				/// <summary>Get or set the maximum width permitted as widget is resized.</summary>
				/// <returns>A reference to the property.</returns>
				float& max_width();

				/// <summary>Get the maximum width permitted as widget is resized.</summary>
				/// <returns>The value of the property.</returns>
				float get_max_width() const;

				/// <summary>Set the maximum width permitted as widget is resized.</summary>
				/// <param name="max_width">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& max_width(const float& max_width);

				/// <summary>Get or set the maximum height permitted as widget is resized.</summary>
				/// <returns>A reference to the property.</returns>
				float& max_height();

				/// <summary>Get the maximum height permitted as widget is resized.</summary>
				/// <returns>The value of the property.</returns>
				float get_max_height() const;

				/// <summary>Set the maximum height permitted as widget is resized.</summary>
				/// <param name="max_height">The value of the property.</param>
				/// <returns>A reference to the modified object.</returns>
				resize_params& max_height(const float& max_height);

				bool operator==(const resize_params&);
				bool operator!=(const resize_params&);
			};

			/// <summary>Base class for all widget specifications.</summary>
			class lecui_api specs {
			public:
				/// <summary>Mouse cursor type.</summary>
				enum class cursor_type {
					/// <summary>Arrow cursor. Used in most cases.</summary>
					arrow,

					/// <summary>Hand cursor. Used to show that an action or link is available over the control.</summary>
					hand,

					/// <summary>Caret cursor. Used to show the position in text where the next character will be inserted.</summary>
					caret,
				};

			protected:
				std::string _text;
				std::string _tooltip;
				lecui::rect _rect;
				resize_params _on_resize = { 0.f, 0.f, 0.f, 0.f };
				cursor_type _cursor = cursor_type::arrow;
				std::string _font = "Segoe UI";
				float _font_size = 9.f;
				color _color_text = { 0, 0, 0, 255 };
				color _color_fill = { 0, 120, 170, 255 };
				color _color_hot = { 0, 120, 170, 255 };
				color _color_selected = { 0, 120, 170, 30 };
				color _color_disabled = { 180, 180, 180, 255 };

			public:
				specs() {};
				virtual ~specs() {};

				bool operator==(const specs&);
				bool operator!=(const specs&);

				/// <summary>Events common to all widgets.</summary>
				struct basic_events {
					/// <summary>The handler to be called when either the space bar or enter key
					/// is pressed, or if the widget is clicked.</summary>
					std::function<void()> action = nullptr;

					/// <summary>The handler to be called when the widget is clicked.</summary>
					std::function<void()> click = nullptr;

					/// <summary>The handler to be called when the widget is right-clicked.</summary>
					std::function<void()> right_click = nullptr;

					/// <summary>The handler to be called when the mouse enters the widget.</summary>
					std::function<void()> mouse_enter = nullptr;

					/// <summary>The handler to be called when the mouse leaves the widget.</summary>
					std::function<void()> mouse_leave = nullptr;
				};

				/// <summary>Get or set the widget text.</summary>
				/// <returns>A reference to the widget text.</returns>
				/// <remarks>The use of the text varies from widget to widget. Some widgets don't
				/// use this field.</remarks>
				virtual std::string& text() = 0;

				/// <summary>Get or set the widget's tooltip text.</summary>
				/// <returns>A reference to the tooltip text.</returns>
				/// <remarks>The tooltip text is displayed when the mouse is hovered over the widget.</remarks>
				virtual std::string& tooltip() = 0;

				/// <summary>Get or set the position and dimensions of the widget.</summary>
				/// <returns>A reference to the widget rectangle.</returns>
				virtual lecui::rect& rect() = 0;

				/// <summary>Get or set the behaviour of the widget when its container is resized.</summary>
				/// <returns>A reference to the resize parameters as defined in <see cref="resize_params"></see>.</returns>
				virtual resize_params& on_resize() = 0;

				/// <summary>Get or set the mouse cursor to use then over the widget.</summary>
				/// <returns>A reference to the cursor type as defined in <see cref="cursor_type"></see>.</returns>
				/// <remarks>This is only used if the widget is non-static.</remarks>
				virtual cursor_type& cursor() = 0;

				/// <summary>Get or set the default widget font, e.g. "Georgia".</summary>
				/// <returns>A reference to the widget's font name.</returns>
				virtual std::string& font() = 0;

				/// <summary>Get or set the default widget font size in points, e.g. 9.0.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				virtual float& font_size() = 0;

				/// <summary>Get or set the color of the widget's text.</summary>
				/// <returns>A reference to the widget's text color.</returns>
				virtual color& color_text() = 0;

				/// <summary>Get or set the fill color of the widget.</summary>
				/// <returns>A reference to the widget's fill color.</returns>
				/// <remarks>The use of the fill color varies from widget to widget. Refer to the
				/// specific widget's documentation for details.</remarks>
				virtual color& color_fill() = 0;

				/// <summary>Get or set the widget's color when the mouse is hovered over it.</summary>
				/// <returns>A reference to the widget's hover color.</returns>
				/// <remarks>The use of the hover color varies from widget to widget. Refer to the
				/// specific widget's documentation for details.</remarks>
				virtual color& color_hot() = 0;

				/// <summary>Get or set the widget's color when selected.</summary>
				/// <returns>A reference to the widget's color when selected.</returns>
				virtual color& color_selected() = 0;

				/// <summary>Get or set the widget's color when it's disabled.</summary>
				/// <returns>A reference to the widget's color when disabled.</returns>
				virtual color& color_disabled() = 0;

				/// <summary>Widget events.</summary>
				/// <remarks>Widgets with additional events need to override this method.</remarks>
				/// <returns>A reference to the widget's basic events.</returns>
				basic_events& events() { return _events; }

				/// <summary>The widget's alias, either user defined or automatically generated by the library.</summary>
				/// <returns>The alias.</returns>
				const std::string& alias() { return _alias; }

			private:
				basic_events _events;
				std::string _alias;

				void alias(const std::string alias) {
					_alias = alias;
				}

#if defined(LECUI_EXPORTS)
				friend class containers::page;

				// implementation classes for widgets that use special panes ... they need access to private variables for redirection
				friend class table_view_impl;
				friend class tree_view_impl;
				friend class html_editor_impl;
				friend class time_impl;
				friend class date_impl;
				friend class icon_impl;
#endif
			};
		}

		/// <summary>Widget manager class.</summary>
		class lecui_api widget_manager {
		public:
			widget_manager(form& fm);
			~widget_manager();

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
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			widget_manager() = delete;
			widget_manager(const widget_manager&) = delete;
			widget_manager& operator=(const widget_manager&) = delete;
		};
	}
}
