/*
** toggle.cpp - toggle implementation
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

#include "../toggle.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::toggle::operator==(const toggle& param) {
	return
		// generic specs
		widget::operator==(param) &&

		// widget specific specs
		(text_off == param.text_off) &&
		(color_on == param.color_on) &&
		(color_off == param.color_off) &&
		(on == param.on);
}

bool liblec::lecui::widgets::specs::toggle::operator!=(const toggle& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::toggle::toggle::toggle_impl {
public:
	toggle_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::toggle::toggle(liblec::lecui::containers::page& page) :
	d_(*(new toggle_impl(page))) {}

liblec::lecui::widgets::toggle::~toggle() { delete& d_; }

liblec::lecui::widgets::specs::toggle&
liblec::lecui::widgets::toggle::add(const std::string& name) {
	return d_.page_.d_page_.add_toggle(name);
}

liblec::lecui::widgets::specs::toggle&
liblec::lecui::widgets::toggle::specs(form& fm, const std::string& name) {
	// parse widget path
	std::vector<std::string> path;
	std::string widget_name;
	fm.d_.parse_widget_path(name, path, widget_name);

	// find the page
	auto& page = fm.d_.find_page(fm.d_.p_pages_, path);

	// find the widget
	return page.d_page_.get_toggle(widget_name).specs();
}
