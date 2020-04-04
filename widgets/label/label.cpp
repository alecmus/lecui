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

bool liblec::lecui::widgets::specs::label::operator==(const label& param) {
	return
		// generic specs
		liblec::lecui::widgets::specs::widget::operator==(param) &&
		
		// widget specific specs
		(color_hot_pressed == param.color_hot_pressed) &&
		(multiline == param.multiline) &&
		(center_h == param.center_h) &&
		(center_v == param.center_v);
}

bool liblec::lecui::widgets::specs::label::operator!=(const label& param) {
	return !(*this == param);
}

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

liblec::lecui::widgets::specs::label&
liblec::lecui::widgets::label::specs(form& fm,
	const std::string& name) {
	// parse widget path
	std::vector<std::string> path;
	std::string widget_name;
	fm.d_.parse_widget_path(name, path, widget_name);

	// find the page
	auto& page = fm.d_.find_page(fm.d_.p_pages_, path, widget_name);
	
	// find the widget
	return page.d_page_.get_label(widget_name).specs();
}
