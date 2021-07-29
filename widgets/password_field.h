//
// password_field.h - password_field widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "text_field.h"
#else
#include <liblec/lecui/widgets/text_field.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			class lecui_api password_field : public text_field {
			public:
				/// <summary>Add a password field to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static text_field& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a password field.</summary>
				/// <param name="fm"></param>
				/// <param name="path">The full path to the widget, e.g. "login_page/credentials_pane/password".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
			/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static text_field& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting password field widget. Builder documentation applies.
#define get_password_field(path) liblec::lecui::widgets::password_field::get(*this, path)
