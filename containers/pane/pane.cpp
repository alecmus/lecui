/*
** pane.cpp - pane implementation
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

#include "../pane.h"
#include "../../form_impl.h"

class liblec::lecui::containers::pane::pane::pane_impl {
public:
	pane_impl(liblec::lecui::containers::page& page,
		liblec::lecui::containers::specs::pane& specs,
		const std::string& name) :
		page_(page), specs_(specs), name_(name) {}
	liblec::lecui::containers::page& page_;
	liblec::lecui::containers::specs::pane& specs_;
	std::string name_;
};

liblec::lecui::containers::pane::pane(liblec::lecui::containers::page& page,
	const std::string& name) :
	d_(*(new pane_impl(page, page.d_page_.add_pane(name), name))) {}

liblec::lecui::containers::pane::~pane() { delete& d_; }

liblec::lecui::containers::specs::pane& liblec::lecui::containers::pane::specs() {
	return d_.specs_;
}

liblec::lecui::containers::specs::pane&
liblec::lecui::containers::pane::specs(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_pane(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}

liblec::lecui::containers::page& liblec::lecui::containers::pane::get() {
	auto& pane_ = d_.page_.d_page_.get_pane(d_.name_);

	log("liblec::lecui::containers::pane::add");

	const std::string name = "pane";

	if (pane_.p_panes_.count(name)) {
		log("library usage error liblec::lecui::containers::pane::add");

		/// to-do: find a mechanism that makes sense ...
		return pane_.p_panes_.at(name);
	}

	pane_.p_panes_.try_emplace(name, d_.page_.d_page_.fm_, name);
	pane_.current_pane_ = name;

	// specify direct2d factory (used internally for geometries and stuff)
	pane_.p_panes_.at(name).d_page_.direct2d_factory(d_.page_.d_page_.direct2d_factory());

	// specify directwrite factory (used internally for text rendering)
	pane_.p_panes_.at(name).d_page_.directwrite_factory(d_.page_.d_page_.directwrite_factory());

	// specify iwic imaging factory (used internally for image rendering)
	pane_.p_panes_.at(name).d_page_.iwic_factory(d_.page_.d_page_.iwic_factory());

	const float thickness = 10.f;
	const float margin = 10.f;
	const float page_tolerance_ = 10.f;
	liblec::lecui::rect rect_client_area = pane_.specs().rect;

	// initialize the page's horizontal scroll bar
	{
		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().resize.perc_width = 100.f;
		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().resize.perc_y = 100.f;

		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().rect.left = 0.f;
		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().rect.right =
			(rect_client_area.right - rect_client_area.left) - (margin + thickness);
		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().rect.bottom =
			(rect_client_area.bottom - rect_client_area.top) - page_tolerance_;
		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().rect.top =
			pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

		pane_.p_panes_.at(name).d_page_.h_scrollbar().specs().on_click = nullptr;
	}

	// initialize the page's vertical scroll bar
	{
		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().resize.perc_height = 100.f;
		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().resize.perc_x = 100.f;

		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().rect.top = 0;
		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().rect.bottom =
			(rect_client_area.bottom - rect_client_area.top) - (margin + thickness);
		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().rect.right =
			(rect_client_area.right - rect_client_area.left) - margin;
		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().rect.left =
			pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().rect.right - thickness;

		pane_.p_panes_.at(name).d_page_.v_scrollbar().specs().on_click = nullptr;
	}

	// add an invisible rect to bound the page. This is essential for scroll bars
	// to work appropriately when contents don't reach the page borders
	auto& rectangle = pane_.p_panes_.at(name).d_page_.add_rectangle("minimal_page_border_rect");
	rectangle.color_fill.alpha = 0;

	// make it transparent
	rectangle.color_border = { 255, 0, 0, 0 };
	rectangle.color_hot = { 255, 0, 0, 0 };

	pane_.p_panes_.at(name).d_page_.size({ 0, 0 });
	pane_.p_panes_.at(name).d_page_.width(rect_client_area.right - rect_client_area.left);
	pane_.p_panes_.at(name).d_page_.height(rect_client_area.bottom - rect_client_area.top);

	pane_.p_panes_.at(name).d_page_.width(pane_.p_panes_.at(name).d_page_.width() -
	(2.f * page_tolerance_));
	pane_.p_panes_.at(name).d_page_.height(pane_.p_panes_.at(name).d_page_.height() -
	(2.f * page_tolerance_));

	rectangle.rect.set(0.f, 0.f, pane_.p_panes_.at(name).d_page_.width(),
		pane_.p_panes_.at(name).d_page_.height());

	rectangle.corner_radius_x = 15.f;
	rectangle.corner_radius_y = 15.f;

	rectangle.resize.perc_width = 100.f;
	rectangle.resize.perc_height = 100.f;

	// return reference to page so caller can add widgets to it
	return pane_.p_panes_.at(name);
}

liblec::lecui::containers::page&
liblec::lecui::containers::pane::get(form& fm, const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx == std::string::npos)
		return fm.d_.p_pages_.at(path);
	else {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the page
		return fm.d_.find_page(page, path);
	}
}
