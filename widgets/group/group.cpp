/*
** group.cpp - group implementation
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

#include "../group.h"
#include "../../form_impl.h"

class liblec::lecui::widgets::group::group::group_impl {
public:
	group_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::group::group(liblec::lecui::containers::page& page) :
	d_(*(new group_impl(page))) {}

liblec::lecui::widgets::group::~group() { delete& d_; }

liblec::lecui::widgets::specs::group&
liblec::lecui::widgets::group::add(const std::string& name) {
	return d_.page_.d_page_.add_group(name);
}
