/*
** list.cpp - list implementation
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

#include "../list.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::list::operator==(const list& param) {
	return
		// generic specs
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border) &&
		(color_dropdown_hot == param.color_dropdown_hot) &&
		(color_menu == param.color_menu) &&
		(color_grid == param.color_grid) &&
		(color_text_header == param.color_text_header) &&
		(color_fill_header == param.color_fill_header) &&
		(color_fill_alternate == param.color_fill_alternate) &&
		(color_row_hot == param.color_row_hot) &&
		(color_row_selected == param.color_row_selected) &&

		// ignore the following for the stated reasons
		// 1. columns: to-do: find a way of checking this equality
		// 2. table: performance issue (polling) when table is large!!!!!
		true;
}

bool liblec::lecui::widgets::specs::list::operator!=(const list& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::list::list::list_impl {
public:
	list_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::list::list(liblec::lecui::containers::page& page) :
	d_(*(new list_impl(page))) {}

liblec::lecui::widgets::list::~list() { delete& d_; }

liblec::lecui::widgets::specs::list&
liblec::lecui::widgets::list::add(const std::string& name) {
	return d_.page_.d_page_.add_list(name);
}

liblec::lecui::widgets::specs::list&
liblec::lecui::widgets::list::specs(form& fm, const std::string& name) {
	// parse widget path
	std::vector<std::string> path;
	std::string widget_name;
	fm.d_.parse_widget_path(name, path, widget_name);

	// find the page
	auto& page = fm.d_.find_page(fm.d_.p_pages_, path);

	// find the widget
	return page.d_page_.get_list(widget_name).specs();
}
