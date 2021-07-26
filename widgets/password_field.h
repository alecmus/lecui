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
			/// <summary>Password field widget builder.</summary>
			/// <remarks>Recommended default size is the same as the text_field widget.</remarks>
			class lecui_api password_field_builder : public text_field_builder {
			public:
				/// <summary>Password field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				password_field_builder(containers::page& page);

				/// <summary>Password field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				password_field_builder(containers::page& page, const std::string& alias);
				~password_field_builder();

			private:
				// Default constructor and copying an object of this class are not allowed
				password_field_builder() = delete;
				password_field_builder(const password_field_builder&) = delete;
				password_field_builder& operator=(const password_field_builder&) = delete;
			};
		}
	}
}

/// Helper for getting password field specs. Builder documentation applies.
#define get_password_field_specs(path) liblec::lecui::widgets::password_field_builder::specs(*this, path)
