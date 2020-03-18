/*
** appearance.h - appearance interface
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

#ifdef LECUI_EXPORTS
#include "form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		class form_api appearance {
		public:
			appearance(form& fm);
			~appearance();

			void shadow(const bool& enable);
			void theme(const liblec::lecui::color& color,
				const liblec::lecui::color& color_hot,
				const liblec::lecui::color& color_disabled);
			void background(const liblec::lecui::color& color,
				const liblec::lecui::color& color_titlebar);

		private:
			class appearance_impl;
			appearance_impl& d_;

			appearance();
			appearance(const appearance&);
			appearance& operator=(const appearance&);
		};
	}
}
