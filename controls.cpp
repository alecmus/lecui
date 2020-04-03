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
#undef min	// to circumvent conflict with liblec::lecui::dimensions::min
#endif

class liblec::lecui::controls::controls_impl {
public:
	controls_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::controls::controls(form& fm) : d_(*(new controls_impl(fm))) {}
liblec::lecui::controls::~controls() { delete& d_; }

void liblec::lecui::controls::resize(bool enable) { d_.fm_.d_.allow_resizing_ = enable; }
void liblec::lecui::controls::minimize(bool enable) { d_.fm_.d_.allow_minimize_ = enable; }

class liblec::lecui::dimensions::dimensions_impl {
public:
	dimensions_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::dimensions::dimensions(form& fm) :
	d_(*(new dimensions_impl(fm))) {}

liblec::lecui::dimensions::~dimensions() { delete& d_; }

void liblec::lecui::dimensions::size(const liblec::lecui::size& size) {
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

const liblec::lecui::size& liblec::lecui::dimensions::size() {
	return d_.fm_.d_.size_;
}

void liblec::lecui::dimensions::min(const liblec::lecui::size& size) {
	if (size.width)		// do not allow minimum width to be greater than current window width
		d_.fm_.d_.min_size_.width = smallest(size.width, d_.fm_.d_.size_.width);

	if (size.height)	// do not allow minimum height to be greater than current window height
		d_.fm_.d_.min_size_.height = smallest(size.height, d_.fm_.d_.size_.height);
}

const liblec::lecui::size& liblec::lecui::dimensions::min() {
	return d_.fm_.d_.min_size_;
}
