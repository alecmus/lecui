//
// page.h - page container interface
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
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Namespace for lecui containers.</summary>
		namespace containers {
			/// <summary>Page container.</summary>
			/// <remarks>This is the generic container type, and is the only one of the containers
			/// that can be added directly to a form. Any widget and any container can
			/// be added to it. It comes full featured with scroll bars that appear if anything
			/// within it exceeds it's dimensions. It covers the entire form save for the border
			/// and the titlebar. It's size is tightly coupled to the form, and is resized
			/// automatically as the form is resized. As such, it's size cannot be directly
			/// manipulated. Other container types, however, can be resized directly, e.g.
			/// panes, but then they cannot be added directly to a form.</remarks>
			class lecui_api page {
			public:
				/// <summary>Page constructor.</summary>
				/// <param name="fm">The form to add the page to.</param>
				/// <param name="alias">The in-form unique alias, e.g. "home_page".</param>
				/// <remarks>Ensure that the alias is unique within the form. Reusing an alias
				/// in a form leads to undefined behavior.</remarks>
				page(form& fm, const std::string& alias);
				virtual ~page();

				/// <summary>Get the size of the page.</summary>
				/// <returns>The size, in pixels.</returns>
				/// <remarks>The size is automatically determined by the library. It is important
				/// to note that any widgets added to a page, and any other container for that
				/// matter, only see the dimensions and coordinates of that container not those
				/// of the form or another container higher up the hierarchy. Dimensions and
				/// coordinates are local to a container.</remarks>
				[[nodiscard]] virtual const size size();

				/// <summary>Check whether the page has been rendered.</summary>
				/// <returns>Returns true if the page has been rendered, else false.</returns>
				const bool rendered();
				
				/// <summary>Get the page's change in size.</summary>
				/// <returns>The change in size, in pixels. If the page has not been rendered this will always
				/// return no change even if the size has, in fact, changed.</returns>
				/// <remarks>This size reflects any changes made by the user through the user interface. This is
				/// useful when a widget's resizing behavior cannot be fully described using the widget's on_resize
				/// property, e.g. if you need to make an html view's size match its text. It is important to check
				/// whether the page has been rendered first. If it hasn't been rendered, as shown by
				/// <see cref='rendered'></see>, then re-call this method later to get the page's actual change in size.</remarks>
				[[nodiscard]] virtual const lecui::size change_in_size();

				/// <summary>Programmatically scroll the page vertically.</summary>
				/// <param name="amount">The amount of pixels to scroll the page widgets by.</param>
				/// <remarks>A negative value moves the widgets upwards; it's equivalent to sliding
				/// the vertical scroll bar downwards, and vice-versa. If the page has no vertical scroll
				/// bar this method has no effect.</remarks>
				void scroll_vertically(const float& amount);

				/// <summary>Programmatically scroll the page horizontally.</summary>
				/// <param name="amount">The amount of pixels to scroll the page widgets by.</param>
				/// <remarks>A negative value moves the widgets to the left; it's equivalent to sliding
				/// the horizontal scroll bar towards the right, and vice-versa. If the page has no
				/// horizontal scroll bar this method has no effect.</remarks>
				void scroll_horizontally(const float& amount);

			protected:
				class impl;
				impl& _d_page;	///< Reference to page implementation.

				// Default constructor and copying an object of this class are not allowed
				page() = delete;
				page(const page&) = delete;
				page& operator=(const page&) = delete;

#if defined(LECUI_EXPORTS)
				friend class form;
				friend class page_manager;
				friend class containers::status_pane_builder;
				friend class containers::tab_pane_builder;
				friend class containers::tab_builder;
				friend class containers::pane_builder;
				friend class containers::group_builder;
				friend class widgets::rectangle_builder;
				friend class widgets::label_builder;
				friend class widgets::button_builder;
				friend class widgets::toggle_builder;
				friend class widgets::table_view_builder;
				friend class widgets::custom_builder;
				friend class widgets::image_view_builder;
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
				friend class widgets::html_editor_impl;
				friend class widgets::image_view_impl;
				friend class widgets::table_view_impl;
				friend class widgets::tree_view_impl;
				friend class widgets::time_impl;
				friend class widgets::date_impl;
				friend class widgets::icon_impl;
				friend class widgets::pane_impl;
				friend class widgets::tab_pane_impl;
#endif
			};
		}

		/// <summary>Page manager class.</summary>
		class lecui_api page_manager {
		public:
			/// <summary>Page manager constructor.</summary>
			/// <param name="fm">A reference to the form.</param>
			page_manager(form& fm);

			/// <summary>Class destructor.</summary>
			~page_manager();

			/// <summary>Check if a page exists.</summary>
			/// <param name="alias">The in-form unique alias, e.g. "home_page".</param>
			/// <returns>True if the page has been created, else false.</returns>
			[[nodiscard]] bool exists(const std::string& alias);

			/// <summary>Add a page to the form.</summary>
			/// <param name="alias">The in-form unique alias, e.g. "home_page".</param>
			/// <returns>A reference to the page container.</returns>
			/// <remarks>throws on failure, but ensure to not attempt to add a page that already
			/// exists. Use an if statement guard as follows: if (!exists()){ add(); }.</remarks>
			[[nodiscard]] containers::page&
				add(const std::string& alias);

			/// <summary>Get the page container of an existing page.</summary>
			/// <param name="fm">The form the page is in.</param>
			/// <param name="alias">The in-form unique alias, e.g. "home_page".</param>
			/// <returns>A reference to the page container.</returns>
			/// <remarks>Throws on failure. For faster coding and more readable code consider
			/// calling this static method through the helper macro provided (get_page), unless if already
			/// using an in-class page_manager instance.</remarks>
			[[nodiscard]] static containers::page&
				get(form& fm, const std::string& alias);

			/// <summary>Show a page.</summary>
			/// <param name="alias">The in-form unique alias, e.g. "home_page".</param>
			/// <remarks>This method is only applicable to form pages and not panes and tab panes even though
			/// they are also containers. To manipulate the visibility of the other containers use the
			/// widget_manager::show method instead.</remarks>
			void show(const std::string& alias);

			/// <summary>Close a container.</summary>
			/// <param name="path">The full path to the container, e.g. "home_page/task_pane".</param>
			void close(const std::string& path);

			/// <summary>Get the name of the currently displayed page.</summary>
			/// <returns>The name of the page that is currently displayed, and an empty string
			/// if there is no page displayed.</returns>
			[[nodiscard]]
			std::string current();

			/// <summary>Get the name of the page that was displayed before the current page.</summary>
			/// <returns>The name of the page that was displayed before the current one, and
			/// an empty string if the current page is the root.</returns>
			[[nodiscard]]
			std::string previous();

		private:
			class impl;
			impl& _d;

			// Default constructor and copying an object of this class are not allowed
			page_manager() = delete;
			page_manager(const page_manager&) = delete;
			page_manager& operator=(const page_manager&) = delete;
		};
	}
}

#if defined(lecui_helper)
/// Helper for getting a form's page. 'Get' method documentation applies.
#define get_page(path) liblec::lecui::page_manager::get(*this, path)
#endif
