//
// page.h - page container interface
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
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
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
				~page();

				/// <summary>Get the size of the page.</summary>
				/// <returns>The size, in pixels.</returns>
				/// <remarks>The size is automatically determined by the library. It is important
				/// to note that any widgets added to a page, and any other container for that
				/// matter, only see the dimensions and coordinates of that container not those
				/// of the form or another container higher up the hierarchy. Dimensions and
				/// coordinates are local to a container.</remarks>
				[[nodiscard]] const size size();

			private:
				class impl;
				impl& _d_page;

				// Default constructor and copying an object of this class are not allowed
				page() = delete;
				page(const page&) = delete;
				page& operator=(const page&) = delete;

#if defined(LECUI_EXPORTS)
				friend class form;
				friend class page_manager;
				friend class containers::status_pane;
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
			page_manager(form& fm);
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
			/// <remarks>Throws on failure.</remarks>
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
