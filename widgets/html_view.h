//
// html_view.h - html_view widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "html_editor.h"
#else
#include <liblec/lecui/widgets/html_editor.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>HTML view widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is a minimum of 320x215px.</remarks>
			class lecui_api html_view : public html_editor {
			public:
				/// <summary>Add an html view to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "review_content".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static html_editor& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of an html view.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "login_page/credentials_pane/email_text".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static html_editor& get(form& fm, const std::string& path);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting html view widget. 'Get' method documentation applies.
#define get_html_view(path) liblec::lecui::widgets::html_view::get(*this, path)
#endif
