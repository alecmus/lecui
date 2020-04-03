/*
** combo.cpp - combo implementation
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

#include "../combo.h"
#include "../../form_impl.h"

class liblec::lecui::widgets::combo::combo::combo_impl {
public:
	combo_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::combo::combo(liblec::lecui::containers::page& page) :
	d_(*(new combo_impl(page))) {}

liblec::lecui::widgets::combo::~combo() { delete& d_; }

liblec::lecui::widgets::specs::combo& liblec::lecui::widgets::combo::add(const std::string& name) {
	return d_.page_.d_page_.add_combo(name);
}
