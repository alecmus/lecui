/*
** textbox.cpp - textbox implementation
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

#include "../textbox.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::textbox::operator==(const textbox& param) {
	// generic specs
	return
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border) &&
		(color_prompt == param.color_prompt) &&
		(color_caret == param.color_caret);
}

bool liblec::lecui::widgets::specs::textbox::operator!=(const textbox& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::textbox::textbox::textbox_impl {
public:
	textbox_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::textbox::textbox(liblec::lecui::containers::page& page) :
	d_(*(new textbox_impl(page))) {}

liblec::lecui::widgets::textbox::~textbox() { delete& d_; }

liblec::lecui::widgets::specs::textbox&
liblec::lecui::widgets::textbox::add(const std::string& name) {
	return d_.page_.d_page_.add_textbox(name);
}

liblec::lecui::widgets::specs::textbox&
liblec::lecui::widgets::textbox::specs(form& fm, const std::string& name) {
	// parse widget path
	std::vector<std::string> path;
	std::string widget_name;
	fm.d_.parse_widget_path(name, path, widget_name);

	// find the page
	auto& page = fm.d_.find_page(fm.d_.p_pages_, path);

	// find the widget
	return page.d_page_.get_textbox(widget_name).specs();
}
