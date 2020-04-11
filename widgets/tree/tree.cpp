/*
** tree.cpp - tree implementation
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

#include "../tree.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::tree::operator==(const tree& param) {
	// generic specs
	return
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border);
}

bool liblec::lecui::widgets::specs::tree::operator!=(const tree& param) {
	return !operator==(param);
}


class liblec::lecui::widgets::tree::tree::tree_impl {
public:
	tree_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::tree::tree(liblec::lecui::containers::page& page) :
	d_(*(new tree_impl(page))) {}

liblec::lecui::widgets::tree::~tree() { delete& d_; }

liblec::lecui::widgets::specs::tree&
liblec::lecui::widgets::tree::add(const std::string& name) {
	// note: this placement is only temporary. the next time page::show() is called
	// this tree widget will be moved into a special pane
	// this is important so we don't have to manually handle scroll bar issues in tree_impl.
	// all we need to do for scroll bars is dynamically change specs_.rect as the tree changes
	// and the pane will do all the scrolling for us
	return d_.page_.d_page_.add_tree(name);
}

liblec::lecui::widgets::specs::tree&
liblec::lecui::widgets::tree::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_tree(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}
