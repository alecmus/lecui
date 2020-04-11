/*
** tab_control.cpp - tab control implementation
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

#include "../tab_control.h"
#include "../../form_impl.h"

class liblec::lecui::containers::tab_control::tab_control::tab_control_impl {
public:
	tab_control_impl(liblec::lecui::containers::page& page,
		liblec::lecui::containers::specs::tab_control& specs,
		const std::string& name) :
		page_(page), specs_(specs), name_(name) {}
	liblec::lecui::containers::page& page_;
	liblec::lecui::containers::specs::tab_control& specs_;
	std::string name_;
};

liblec::lecui::containers::tab_control::tab_control(liblec::lecui::containers::page& page,
	const std::string& name) :
	d_(*(new tab_control_impl(page, page.d_page_.add_tab_control(name), name))) {}

liblec::lecui::containers::tab_control::~tab_control() { delete& d_; }

liblec::lecui::containers::specs::tab_control& liblec::lecui::containers::tab_control::specs() {
	return d_.specs_;
}

liblec::lecui::containers::specs::tab_control&
liblec::lecui::containers::tab_control::specs(form& fm,
	const std::string& name) {
	auto path = name;
	auto idx = path.find("/");

	if (idx != std::string::npos) {
		auto page_name = path.substr(0, idx);
		path = path.substr(idx + 1);
		auto& page = fm.d_.p_pages_.at(page_name);
		// find the widget
		auto results = fm.d_.find_widget(page, path);
		return results.page.d_page_.get_tab_control(results.widget.name()).specs();
	}

	throw std::exception("Invalid path");
}

void liblec::lecui::containers::tab_control::select(const std::string& name) {
	auto& tab_control_ = d_.page_.d_page_.get_tab_control(d_.name_);
	tab_control_.current_tab_ = name;
}

class liblec::lecui::containers::tab::tab::tab_impl {
public:
	tab_impl(liblec::lecui::containers::tab_control& tc) :
		tc_(tc) {}
	liblec::lecui::containers::tab_control& tc_;
};

liblec::lecui::containers::tab::tab(liblec::lecui::containers::tab_control& tc) :
	d_(*(new tab_impl(tc))) {}

liblec::lecui::containers::tab::~tab() { delete& d_; }

liblec::lecui::containers::page& liblec::lecui::containers::tab::add(const std::string& name) {
	auto& tab_control_ = d_.tc_.d_.page_.d_page_.get_tab_control(d_.tc_.d_.name_);

	log("liblec::lecui::containers::tab::add");

	if (tab_control_.p_tabs_.count(name)) {
		log("library usage error liblec::lecui::containers::tab::add");

		/// to-do: find a mechanism that makes sense ...
		return tab_control_.p_tabs_.at(name);
	}

	tab_control_.p_tabs_.try_emplace(name, d_.tc_.d_.page_.d_page_.fm_, name);

	// specify direct2d factory (used internally for geometries and stuff)
	tab_control_.p_tabs_.at(name).d_page_.direct2d_factory(
		d_.tc_.d_.page_.d_page_.direct2d_factory());

	// specify directwrite factory (used internally for text rendering)
	tab_control_.p_tabs_.at(name).d_page_.directwrite_factory(
		d_.tc_.d_.page_.d_page_.directwrite_factory());

	// specify iwic imaging factory (used internally for image rendering)
	tab_control_.p_tabs_.at(name).d_page_.iwic_factory(d_.tc_.d_.page_.d_page_.iwic_factory());

	const float thickness = 10.f;
	const float margin = 10.f;
	const float page_tolerance_ = 10.f;
	const float caption_bar_height_ = tab_control_.caption_bar_height();
	liblec::lecui::rect rect_client_area = tab_control_.specs().rect;

	// initialize the page's horizontal scroll bar
	{
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().resize.perc_width = 100.f;
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().resize.perc_y = 100.f;

		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.left = 0.f;
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.right =
			(rect_client_area.right - rect_client_area.left) - (margin + thickness);
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.bottom =
			(rect_client_area.bottom - rect_client_area.top) -
			(caption_bar_height_ + page_tolerance_);
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.top =
			tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().on_click = nullptr;
	}

	// initialize the page's vertical scroll bar
	{
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().resize.perc_height = 100.f;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().resize.perc_x = 100.f;

		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.top = 0.f;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.bottom =
			(rect_client_area.bottom - rect_client_area.top) -
			(margin + thickness) - caption_bar_height_;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.right =
			(rect_client_area.right - rect_client_area.left) - margin;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.left =
			tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.right - thickness;

		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().on_click = nullptr;
	}

	// add an invisible rect to bound the page. This is essential for scroll bars
	// to work appropriately when contents don't reach the page borders
	auto& rectangle =
		tab_control_.p_tabs_.at(name).d_page_.add_rectangle("minimal_page_border_rect");
	rectangle.color_fill.alpha = 0;

	// make it transparent
	rectangle.color_border = { 255, 0, 0, 0 };
	rectangle.color_hot = { 255, 0, 0, 0 };

	tab_control_.p_tabs_.at(name).d_page_.size({ 0.f, 0.f });
	tab_control_.p_tabs_.at(name).d_page_.width(rect_client_area.right - rect_client_area.left);
	tab_control_.p_tabs_.at(name).d_page_.height(rect_client_area.bottom - rect_client_area.top);

	tab_control_.p_tabs_.at(name).d_page_.width(tab_control_.p_tabs_.at(name).d_page_.width() -
	(2.f * page_tolerance_));
	tab_control_.p_tabs_.at(name).d_page_.height(tab_control_.p_tabs_.at(name).d_page_.height() -
	(2.f * page_tolerance_ + caption_bar_height_));

	rectangle.rect.set(0.f, 0.f, tab_control_.p_tabs_.at(name).d_page_.width(),
		tab_control_.p_tabs_.at(name).d_page_.height());

	rectangle.corner_radius_x = 15.f;
	rectangle.corner_radius_y = 15.f;

	rectangle.resize.perc_width = 100.f;
	rectangle.resize.perc_height = 100.f;

	// return reference to page so caller can add widgets to it
	return tab_control_.p_tabs_.at(name);
}

liblec::lecui::containers::page&
liblec::lecui::containers::tab::get(form& fm, const std::string& name) {
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
