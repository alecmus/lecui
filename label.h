/*
** label.h - label interface
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
	#include "widget.h"
#else
	#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			class page;
			namespace specs {
				class label : public widget {
				public:
					label() { color_hot = { 20, 80, 140, 20 }; };
					liblec::lecui::color color_hot_pressed = { 20, 80, 140, 40 };
					bool multiline = false;
					bool center_h = false;
					bool center_v = false;
				};
			}

			class lecui_api label {
			public:
				label(liblec::lecui::widgets::page& page);
				~label();

				liblec::lecui::widgets::specs::label& add(const std::string& name);

			private:
				class label_impl;
				label_impl& d_;

				label();
				label(const label&);
				label& operator=(const label&);
			};
		}
	}
}
