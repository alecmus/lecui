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

bool liblec::lecui::widgets::specs::group::operator==(const group& param) {
	return
		// generic specs
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border);
}

bool liblec::lecui::widgets::specs::group::operator!=(const group& param) {
	return !operator==(param);
}

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

liblec::lecui::widgets::specs::group&
liblec::lecui::widgets::group::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_group(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
