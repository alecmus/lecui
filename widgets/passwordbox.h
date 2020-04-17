/*
** passwordbox.h - passwordbox widget interface
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
#include "textbox.h"
#else
#include <liblec/lecui/widgets/textbox.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Passwordbox widget.</summary>
			class lecui_api passwordbox : public textbox {
			public:
				/// <summary>Passwordbox constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				passwordbox(containers::page& page, const std::string& alias);
				~passwordbox();

			private:
				// Default constructor and copying an object of this class are not allowed
				passwordbox();
				passwordbox(const passwordbox&);
				passwordbox& operator=(const passwordbox&);
			};
		}
	}
}
