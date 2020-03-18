/*
** controls.h - controls interface
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
		class form_api controls {
		public:
			controls(form& fm);
			~controls();

			void resize(bool enable);
			void minimize(bool enable);

		private:
			class controls_impl;
			controls_impl& d_;

			controls();
			controls(const controls&);
			controls& operator=(const controls&);
		};

		class form_api dimensions {
		public:
			dimensions(form& fm);
			~dimensions();

			void size(const liblec::lecui::size& size);
			const liblec::lecui::size& size();
			void min(const liblec::lecui::size& size);
			const liblec::lecui::size& min();

		private:
			class dimensions_impl;
			dimensions_impl& d_;

			dimensions();
			dimensions(const dimensions&);
			dimensions& operator=(const dimensions&);
		};
	}
}
