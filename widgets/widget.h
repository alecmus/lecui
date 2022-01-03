//
// widget.h - widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			/// <summary>Base class for all widgets.</summary>
			class lecui_api widget {
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
				widget() {};
				virtual ~widget() {};

				bool operator==(const widget&);
				bool operator!=(const widget&);

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

				/// <summary>Get or set the default widget font size in points, e.g. 9.0f.</summary>
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

			/// <summary>Badge specifications.</summary>
			class lecui_api badge_specs {
			public:
				/// <summary>Badge position within the widget.</summary>
				enum class badge_position {
					/// <summary>Badget on the top left of the widget.</summary>
					top_left,

					/// <summary>Badget on the top right of the widget.</summary>
					top_right,

					/// <summary>Badget on the bottom left of the widget.</summary>
					bottom_left,

					/// <summary>Badget on the bottom right of the widget.</summary>
					bottom_right,
				};

			protected:
				std::string _text;
				std::string _font = "Segoe UI";
				float _font_size = 8.f;
				float _border = .5f;
				color _color = { 255, 0, 0, 180 };
				color _color_border = { 255, 255, 255, 180 };
				color _color_text = { 255, 255, 255, 255 };
				badge_position _position = badge_position::bottom_right;

			public:
				bool operator==(const badge_specs&);
				bool operator!=(const badge_specs&);

				/// <summary>Get or set the badge text.</summary>
				/// <returns>A reference to the badge text.</returns>
				std::string& text();

				/// <summary>Set the badge text.</summary>
				/// <param name="text">The badge text.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& text(const std::string& text);

				/// <summary>Get or set the default badge font, e.g. "Georgia".</summary>
				/// <returns>A reference to the badge's font name.</returns>
				std::string& font();

				/// <summary>Set the badge font.</summary>
				/// <param name="font">The name of the font, e.g. "Georgia".</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& font(const std::string& font);

				/// <summary>Get or set the badge's font size in points, e.g. 9.0f.</summary>
				/// <returns>A reference to the widget's font size.</returns>
				float& font_size();

				/// <summary>Set the badge's font size.</summary>
				/// <param name="font_size">The font size in points, e.g. 9.0f.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& font_size(const float& font_size);

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& border(const float& border);

				/// <summary>Get or set the color of the badge.</summary>
				/// <returns>A reference to the badge's color.</returns>
				lecui::color& color();

				/// <summary>Set the color of the badge.</summary>
				/// <param name="color">The color of the badge, as defined in <see cref="lecui::color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& color(const lecui::color& color);

				/// <summary>Get or set the color of the badge's border.</summary>
				/// <returns>A reference to the badge's border color.</returns>
				lecui::color& color_border();

				/// <summary>Set the color of the badge's border.</summary>
				/// <param name="color_border">The color of the badge's border, as defined in <see cref="lecui::color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& color_border(const lecui::color& color_border);

				/// <summary>Get or set the color of the badge's text.</summary>
				/// <returns>A reference to the badge's text color.</returns>
				lecui::color& color_text();

				/// <summary>Set the color of the badge's text.</summary>
				/// <param name="color_text">The color of the badge's text, as defined in <see cref="lecui::color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& color_text(const lecui::color& color_text);

				/// <summary>Get or set the badge's position.</summary>
				/// <returns>A reference to the badge's position.</returns>
				badge_position& position();

				/// <summary>Set the badge's position.</summary>
				/// <param name="position">The position of the badge, as defined in <see cref="badge_position"></see></param>
				/// <returns>A reference to the modified object.</returns>
				badge_specs& position(const badge_position& position);
			};

			/// <summary>Base class for all widgets with badges.</summary>
			/// <remarks>Widgets that use badges inherit from both this class and the <see cref="widget"></see> class.</remarks>
			class lecui_api badge_widget {
			protected:
				badge_specs _badge;

			public:
				bool operator==(const badge_widget&);
				bool operator!=(const badge_widget&);

				/// <summary>Get or set the widget's badge.</summary>
				/// <returns>A reference to the badge specs.</returns>
				/// <remarks>The badge is useful in indicating when something requires user attention, e.g., for
				/// showing how many unread messages the user has or for indicating when an update is available.</remarks>
				virtual badge_specs& badge() = 0;
			};
		}

		/// <summary>Widget manager class.</summary>
		class lecui_api widget_manager {
		public:
			/// <summary>Widget manager constructor.</summary>
			/// <param name="fm">The form whose widgets we would like to manage.</param>
			widget_manager(form& fm);
			~widget_manager();

			/// <summary>Enable a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			/// <remarks>Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			bool enable(const std::string& path, std::string& error);

			/// <summary>Disable a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			/// <remarks>Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			bool disable(const std::string& path, std::string& error);

			/// <summary>Show a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			/// <remarks>Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			bool show(const std::string& path, std::string& error);

			/// <summary>Hide a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			/// <remarks>Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			bool hide(const std::string& path, std::string& error);

			/// <summary>Close a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <remarks>Avoid closing a widget from within its own handler to avoid access
			/// violation errors. Also make sure to never keep 'dangling widget references'
			/// because attempting to use those references after this method is called will
			/// result in access violation errors as well. Make sure to employ proper logic. As a
			/// failsafe, the widget is not closed immediately when this method is called. Rather
			/// a special timer is scheduled internally to close the widget. After this, the widget
			/// is closed as soon as possible, typically soon after the current handler exits.
			/// Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.
			/// Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			void close(const std::string& path);

			/// <summary>Select a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <remarks>Any previous selection within the page is removed.
			/// Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			void select(const std::string& path);

			/// <summary>Refresh a widget.</summary>
			/// <param name="path">The full path to the widget, e.g. "home_page/username".</param>
			/// <param name="error">Error information.</param>
			/// <returns>Returns true if successful, else false.</returns>
			/// <remarks>This method causes all the widget's graphic resources to be recreated. This is
			/// useful if you need a resource being used by the widget, e.g. a .png file being used by an
			/// image_view, freed immediately after the widget's properties are changed. A call to form::reload()
			/// would do this but it's a lot more expensive since it refreshes every widget in every page, and a call
			/// to form::update() would only free the resource after the next time a paint operation is done.
			/// Note that a call to form::update() is not necessary after this method is called because
			/// it sends this call internally.</remarks>
			bool refresh(const std::string& path, std::string& error);

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
