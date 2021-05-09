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
			/// <summary>Password field widget.</summary>
			/// <remarks>Recommended default size is the same as the text_field widget.</remarks>
			class lecui_api password_field : public text_field {
			public:
				/// <summary>Password field constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				password_field(containers::page& page);

				/// <summary>Password field constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				password_field(containers::page& page, const std::string& alias);
				~password_field();

			private:
				// Default constructor and copying an object of this class are not allowed
				password_field() = delete;
				password_field(const password_field&) = delete;
				password_field& operator=(const password_field&) = delete;
			};
		}
	}
}
