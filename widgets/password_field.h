/*
** password_field.h - password_field widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

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
