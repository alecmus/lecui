/*
** custom.cpp - custom widget implementation
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

#include "../custom.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::custom::operator==(const custom& param) {
	return
		// generic specs
		widget::operator==(param)&&

		// widget specific specs
		true;
}

bool liblec::lecui::widgets::specs::custom::operator!=(const custom& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::custom::custom::custom_impl {
public:
	custom_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::custom::custom(liblec::lecui::containers::page& page) :
	d_(*(new custom_impl(page))) {}

liblec::lecui::widgets::custom::~custom() { delete& d_; }

liblec::lecui::widgets::specs::custom&
liblec::lecui::widgets::custom::add(const std::string& name) {
	return d_.page_.d_page_.add_custom(name);
}

liblec::lecui::widgets::specs::custom&
liblec::lecui::widgets::custom::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_custom(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
