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
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_textbox(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
