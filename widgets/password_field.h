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
			class lecui_api password_field : public text_field {
			public:
				/// <summary>Password field constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				password_field(containers::page& page, const std::string& alias);
				~password_field();

			private:
				// Default constructor and copying an object of this class are not allowed
				password_field();
				password_field(const password_field&);
				password_field& operator=(const password_field&);
			};
		}
	}
}