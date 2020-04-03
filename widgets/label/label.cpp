/*
** label.cpp - label implementation
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

#include "../label.h"
#include "../../form_impl.h"

class liblec::lecui::widgets::label::label::label_impl {
public:
	label_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::label::label(liblec::lecui::containers::page& page) :
	d_(*(new label_impl(page))) {}

liblec::lecui::widgets::label::~label() { delete& d_; }

liblec::lecui::widgets::specs::label&
liblec::lecui::widgets::label::add(const std::string& name) {
	return d_.page_.d_page_.add_label(name);
}
