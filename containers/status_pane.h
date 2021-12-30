//
// status_pane.h - status_pane container interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../widgets/widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace containers {
#if defined(LECUI_EXPORTS)
			class side_pane;
			class status_pane_builder;
#endif

			/// <summary>Status pane container specifications.</summary>
			/// <remarks>Note that this does not inherit from widgets::widget as regular
			/// panes do. This is by design because of the nature of the status pane.</remarks>
			class lecui_api status_pane_specs {
			public:
				enum class pane_location {
					bottom,
					top,
					left,
					right,
				};

			private:
				status_pane_specs::pane_location _location = status_pane_specs::pane_location::bottom;
				float _thickness = 32.f;
				bool _floating = false;
				bool _side_pane = false;

#if defined(LECUI_EXPORTS)
				friend class side_pane;
				friend class status_pane_builder;
#endif

			public:
				/// <summary>Get or set the pane's thickness. This will be a height for top and bottom
				/// panes, and a width for left and right panes.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& thickness();

				/// <summary>Set the pane's thickness. This will be a height for top and bottom
				/// panes, and a width for left and right panes.</summary>
				/// <param name="thickness">The thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				status_pane_specs& thickness(const float& thickness);

				/// <summary>Get or set the floating property.</summary>
				/// <returns>A reference to the property.</returns>
				/// <remarks>The floating property determines whether the pane should just float
				/// above the form without eating into the client area.</remarks>
				bool& floating();

				/// <summary>Set the floating property.</summary>
				/// <param name="floating">Whether to make this a floating pane.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>The floating property determines whether the pane should just float
				/// above the form without eating into the client area.</remarks>
				status_pane_specs& floating(const bool& floating);

				/// <summary>Get or set the location of the status pane.</summary>
				/// <returns>A reference to the property.</returns>
				pane_location& location();

				/// <summary>Set the location of the status pane.</summary>
				/// <param name="location">The location of the status pane.</param>
				/// <returns>A reference to the modified object.</returns>
				status_pane_specs& location(const status_pane_specs::pane_location location);
			};

			/// <summary>Base class for all status panes.</summary>
			class lecui_api status_pane_base : public containers::page {
			public:
				status_pane_base(form& fm, const std::string& alias);
				virtual ~status_pane_base();
			};

			/// <summary>Status pane container. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing other containers to be added to status panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit. However, it is recommended to
			/// limit the number of widgets within status panes as these panes persist throughout the
			/// runtime of the app, and are visible in all pages. Keep the contents of status panes
			/// minimal and straightforward as a good gui design principle. Note that it is recommended to add all status panes
			/// before adding any pages so that the pages can return dimensions that factor for the status panes.</remarks>
			class lecui_api status_pane : public status_pane_base {
			public:
				status_pane(form&, status_pane_specs);

				[[nodiscard]]
				/// <summary>Create a status pane.</summary>
				/// <param name="fm">The form to place it in.</param>
				/// <param name="specs">The specifications of the status pane.</param>
				/// <returns>A reference to the status pane.</returns>
				/// <remarks>Status panes are transparent by default, and have no scroll bars.
				/// A background can be added by using a rectangle widget. Ensure that there is a
				/// maximum of one pane per location. Readding a status pane in a given location
				/// leads to undefined behavior, i.e. a maximum of one left, one right, one top
				/// and one bottom status pane. If you need to split the content of a given pane
				/// simply add regular panes within it like you with any other container. However,
				/// keep in mind that regular panes come with scroll bars. In fact, if you need a
				/// status pane with scroll bars you can simply add a single pane that fills the
				/// status pane entirely and you will have the desired effect.</remarks>
				static status_pane& add(form& fm, status_pane_specs specs);

				/// <summary>Get a reference to an existing status pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="alias">The status pane alias, e.g. "status::right".</param>
				/// <returns>A reference to the status pane.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_status_pane).</remarks>
				[[nodiscard]]
				static containers::status_pane& get(form& fm, const std::string& alias);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting a status pane. 'Get' method documentation applies.
#define get_status_pane(path) liblec::lecui::containers::status_pane::get(*this, path)
#endif
