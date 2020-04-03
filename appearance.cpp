/*
** appearance.cpp - appearance implementation
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

#include "appearance.h"
#include "form_impl.h"

class liblec::lecui::appearance::appearance_impl {
public:
	appearance_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::appearance::appearance(form& fm) :
	d_(*(new appearance_impl(fm))) {}

liblec::lecui::appearance::~appearance() { delete& d_; }

void liblec::lecui::appearance::shadow(const bool& enable) {
	if (IsWindow(d_.fm_.d_.hWnd_))
		d_.fm_.d_.set_borderless_shadow(d_.fm_.d_.hWnd_, enable);
	else
		d_.fm_.d_.borderless_shadow_ = enable;
}

void liblec::lecui::appearance::theme(const liblec::lecui::color& color,
	const liblec::lecui::color& color_hot,
	const liblec::lecui::color& color_disabled) {
	d_.fm_.d_.clr_theme_ = color;
	d_.fm_.d_.clr_theme_hot_ = color_hot;
	d_.fm_.d_.clr_theme_disabled_ = color_disabled;
}

void liblec::lecui::appearance::background(const liblec::lecui::color& color,
	const liblec::lecui::color& color_titlebar) {
	d_.fm_.d_.clr_background_ = color;
	d_.fm_.d_.clr_titlebar_background_ = color_titlebar;
}
