/*
** controls.cpp - controls implementation
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

#include "controls.h"
#include "form_impl.h"

#if defined(min)
#undef min	// to circumvent conflict with dimensions::min
#endif

namespace liblec {
	namespace lecui {
		class controls::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		controls::controls(form& fm) : d_(*(new impl(fm))) {}
		controls::~controls() { delete& d_; }

		void controls::resize(bool enable) { d_.fm_.d_.allow_resizing_ = enable; }
		void controls::minimize(bool enable) { d_.fm_.d_.allow_minimize_ = enable; }

		class dimensions::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		dimensions::dimensions(form& fm) :
			d_(*(new impl(fm))) {}

		dimensions::~dimensions() { delete& d_; }

		void dimensions::size(const lecui::size& size) {
			if (size.width) {
				d_.fm_.d_.size_.width = size.width;

				// force minimum width to conform (it cannot be greater)
				d_.fm_.d_.min_size_.width = smallest(d_.fm_.d_.size_.width, d_.fm_.d_.min_size_.width);
			}

			if (size.height) {
				d_.fm_.d_.size_.height = size.height;

				// force minimum height to conform (it cannot be greater)
				d_.fm_.d_.min_size_.height = smallest(d_.fm_.d_.size_.height, d_.fm_.d_.min_size_.height);
			}
		}

		const size& dimensions::size() {
			return d_.fm_.d_.size_;
		}

		void dimensions::min(const lecui::size& size) {
			if (size.width)		// do not allow minimum width to be greater than current window width
				d_.fm_.d_.min_size_.width = smallest(size.width, d_.fm_.d_.size_.width);

			if (size.height)	// do not allow minimum height to be greater than current window height
				d_.fm_.d_.min_size_.height = smallest(size.height, d_.fm_.d_.size_.height);
		}

		const size& dimensions::min() {
			return d_.fm_.d_.min_size_;
		}
	}
}
