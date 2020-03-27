/*
** custom.h - custom widget interface
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
		namespace containers {
			class page;
		}

		namespace widgets {
			namespace specs {
				class custom : public widget {
				public:
					std::function<void(void* ID2D1HwndRenderTarget_, void* IDWriteFactory_)>
						on_create_resources = nullptr;
					std::function<void()> on_discard_resources = nullptr;
					std::function<
						void(void* D2D1_RECT_F_, bool enabled_, bool hit_,
							bool pressed_, bool selected_)> on_render = nullptr;
				};
			}

			class lecui_api custom {
			public:
				custom(liblec::lecui::containers::page& page);
				~custom();

				liblec::lecui::widgets::specs::custom& add(const std::string& name);

			private:
				class custom_impl;
				custom_impl& d_;

				custom();
				custom(const custom&);
				custom& operator=(const custom&);
			};
		}
	}
}
