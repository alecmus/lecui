//
// side_pane.h - side_pane container interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "status_pane.h"
#else
#include <liblec/lecui/containers/status_pane.h>
#endif

namespace liblec {
	namespace lecui {
		namespace containers {
			/// <summary>Side pane container. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>The side pane is essentially a special left side pane that extends into the title bar. Hence, refer to the status
			/// pane documentation remarks for additional information relating to all side panes.</remarks>
			class lecui_api side_pane : public status_pane_base {
			public:
				/// <summary>Class constructor.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="thickness">The thickness of the side pane.</param>
				/// <remarks>Never to be called directly. This is called interfally by the library when
				/// the <see cref="add"></see> static method is called.</remarks>
				side_pane(form& fm, const float& thickness);

				[[nodiscard]]
				/// <summary>Create a side pane.</summary>
				/// <param name="fm">The form to place it in.</param>
				/// <param name="thickness">The thickness of the side pane.</param>
				/// <returns>A reference to the status pane.</returns>
				/// <remarks>Side panes are transparent by default, and have no scroll bars.
				/// A background can be added by using a rectangle widget. Ensure that there is a
				/// maximum of one side pane per form and that there isn't a left status pane. Adding more
				/// than one side pane leads to undefined behavior, same with adding a left status
				/// pane together with a side pane since a side pane is really a special left side pane.</remarks>
				static status_pane& add(form& fm, const float& thickness);

				/// <summary>Get a reference to an existing side pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <returns>A reference to the status pane.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_side_pane).</remarks>
				[[nodiscard]]
				static containers::status_pane& get(form& fm);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting a side pane. 'Get' method documentation applies.
#define get_side_pane() liblec::lecui::containers::side_pane::get(*this)
#endif
