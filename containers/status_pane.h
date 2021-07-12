//
// status_pane.h - status_pane container interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
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
			/// <summary>Status pane container specifications.</summary>
			/// <remarks>Note that this does not inherit from widget::specs as regular
			/// panes do. This is by design because of the nature of the status pane.</remarks>
			class status_pane_specs {
			private:
				float _thickness = 0.f;
				bool _floating = false;

			public:
				enum class location {
					bottom,
					top,
					left,
					right,
				};

				/// <summary>Get or set the pane's thickness. This will be a height for top and bottom
				/// panes, and a width for left and right panes.</summary>
				/// <returns>A reference to the thickness, in pixels.</returns>
				float& thickness() { return _thickness; }

				/// <summary>Set the pane's thickness. This will be a height for top and bottom
				/// panes, and a width for left and right panes.</summary>
				/// <param name="thickness">The thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				status_pane_specs& thickness(const float& thickness) {
					_thickness = thickness;
					return *this;
				}

				/// <summary>Get or set the floating property.</summary>
				/// <returns>A reference to the property.</returns>
				/// <remarks>The floating property determines whether the pane should just float
				/// above the form without eating into the client area.</remarks>
				bool& floating() { return _floating; }

				/// <summary>Set the floating property.</summary>
				/// <param name="floating">Whether to make this a floating pane.</param>
				/// <returns>A reference to the modified object.</returns>
				/// <remarks>The floating property determines whether the pane should just float
				/// above the form without eating into the client area.</remarks>
				status_pane_specs& floating(const bool& floating) {
					_floating = floating;
					return *this;
				}
			};

			/// <summary>Status pane container.</summary>
			/// <remarks>Any widget can be added to this container. Consequently, recursion is
			/// fully supported, allowing status panes within status panes, to virtually any depth level that
			/// the memory of the computer the app is running on can permit. However, it is recommended to
			/// limit the number of widgets within status panes as these panes persist throughout the
			/// runtime of the app, and are visible in all pages. Keep the contents of status panes
			/// minimal and straightforward as a good gui design principle.</remarks>
			class lecui_api status_pane {
			public:
				/// <summary>status_pane constructor.</summary>
				/// <param name="fm">The form to place it in.</param>
				/// <param name="location">The location of the status pane.</param>
				/// <remarks>Status panes are transparent by default, and have no scroll bars.
				/// A background can be added by using a rectangle widget. Ensure that there is a
				/// maximum of one pane per location. Readding a status pane in a given location
				/// leads to undefined behavior, i.e. a maximum of one left, one right, one top
				/// and one bottom status pane. If you need to split the content of a given pane
				/// simply add regular panes within it like you with any other container. However,
				/// keep in mind that regular panes come with scroll bars. In fact, if you need a
				/// status pane with scroll bars you can simply add a single pane that fills the
				/// status pane entirely and you will have the desired effect.</remarks>
				status_pane(form& fm, status_pane_specs::location location);
				~status_pane();

				/// <summary>Get the status pane specifications.</summary>
				/// <returns>A reference to the status pane specifications.</returns>
				[[nodiscard]]
				status_pane_specs& specs();

				/// <summary>Get the status pane specifications.</summary>
				/// <returns>A reference to the status pane specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				status_pane_specs& operator()();

				/// <summary>Get the specifications of a status pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="alias">The status pane alias, e.g.
				/// "status::bottom".</param>
				/// <returns>A reference to the status pane specifications.</returns>
				/// <remarks>Throws of failure.</remarks>
				[[nodiscard]]
				static status_pane_specs& specs(form& fm, const std::string& alias);

				/// <summary>Get the status pane container page.</summary>
				/// <returns>A reference to the status pane container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The status pane, however,
				/// does not have scroll bars like other containers. If you really need a status
				/// pane to have scroll bars (though it's unimaginable why one would need to do
				/// this) add a single pane within it that fills it up entirely.</remarks>
				[[nodiscard]]
				containers::page& get();

				/// <summary>Get the status pane container page of an existing status pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="alias">The status pane alias, e.g. "status::right".</param>
				/// <returns>A reference to the status pane container page.</returns>
				[[nodiscard]]
				static containers::page& get(form& fm, const std::string& alias);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				status_pane() = delete;
				status_pane(const status_pane&) = delete;
				status_pane& operator=(const status_pane&) = delete;
			};
		}
	}
}
