/*
** page.h - page interface
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
	#include "../form.h"
#else
	#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			class rectangle;
			class label;
			class group;
			class button;
			class toggle;
			class combo;
			class list;
			class custom;
			class image;
		}

		namespace containers {
			class tab_control;
			class tab;
			class pane;

			class lecui_api page {
			public:
				page(const std::string& name);
				~page();

				liblec::lecui::size size();

			private:
				class page_impl;
				page_impl& d_page_;

				page();
				page(const page&);
				page& operator=(const page&);

				friend liblec::lecui::form;
				friend liblec::lecui::page;

				friend liblec::lecui::containers::tab_control;
				friend liblec::lecui::containers::tab;
				friend liblec::lecui::containers::pane;

				friend liblec::lecui::widgets::rectangle;
				friend liblec::lecui::widgets::label;
				friend liblec::lecui::widgets::group;
				friend liblec::lecui::widgets::button;
				friend liblec::lecui::widgets::toggle;
				friend liblec::lecui::widgets::combo;
				friend liblec::lecui::widgets::list;
				friend liblec::lecui::widgets::custom;
				friend liblec::lecui::widgets::image;
			};
		}

		class lecui_api page {
		public:
			page(form& fm);
			~page();

			bool exists(const std::string& name);
			liblec::lecui::containers::page&
				add(const std::string& name);
			static liblec::lecui::containers::page&
				get(form& fm, const std::string& name);
			void show(const std::string& name);

		private:
			class page_impl;
			page_impl& d_;

			page();
			page(const page&);
			page& operator=(const page&);
		};
	}
}
