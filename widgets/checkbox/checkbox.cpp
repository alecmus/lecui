/*
** checkbox.cpp - checkbox implementation
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

#include "../checkbox.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::checkbox::operator==(const checkbox& param) {
	// generic specs
	return
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border) &&
		(color_check == param.color_check);
}

bool liblec::lecui::widgets::specs::checkbox::operator!=(const checkbox& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::checkbox::checkbox::checkbox_impl {
public:
	checkbox_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::checkbox::checkbox(liblec::lecui::containers::page& page) :
	d_(*(new checkbox_impl(page))) {}

liblec::lecui::widgets::checkbox::~checkbox() { delete& d_; }

liblec::lecui::widgets::specs::checkbox&
liblec::lecui::widgets::checkbox::add(const std::string& name) {
	return d_.page_.d_page_.add_checkbox(name);
}

liblec::lecui::widgets::specs::checkbox&
liblec::lecui::widgets::checkbox::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_checkbox(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
