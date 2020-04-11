/*
** button.cpp - button implementation
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

#include "../button.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::button::operator==(const button& param) {
	// generic specs
	return
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border);
}

bool liblec::lecui::widgets::specs::button::operator!=(const button& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::button::button::button_impl {
public:
	button_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::button::button(liblec::lecui::containers::page& page) :
	d_(*(new button_impl(page))) {}

liblec::lecui::widgets::button::~button() { delete& d_; }

liblec::lecui::widgets::specs::button&
liblec::lecui::widgets::button::add(const std::string& name) {
	return d_.page_.d_page_.add_button(name);
}

liblec::lecui::widgets::specs::button&
liblec::lecui::widgets::button::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_button(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
