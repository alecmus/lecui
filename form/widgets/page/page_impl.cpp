/*
** page_impl.cpp - page widget implementation
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

#include "page_impl.h"

liblec::lecui::widgets::page::page_impl::page_impl(const std::string& name) :
	p_directwrite_factory_(nullptr),
	name_(name),
	h_scrollbar_(name),
	v_scrollbar_(name) {
	log("constructor: " + name_);
	widgets_.emplace(h_scrollbar_.name(), h_scrollbar_);
	widgets_.emplace(v_scrollbar_.name(), v_scrollbar_);
}

liblec::lecui::widgets::page::page_impl::~page_impl() { log("destructor:  " + name_); }
std::string liblec::lecui::widgets::page::page_impl::name() { return name_; }

void
liblec::lecui::widgets::page::page_impl::directwrite_factory(IDWriteFactory* p_directwrite_factory) {
	p_directwrite_factory_ = p_directwrite_factory;
}

IDWriteFactory* liblec::lecui::widgets::page::page_impl::directwrite_factory() {
	return p_directwrite_factory_;
}

void liblec::lecui::widgets::page::page_impl::size(const liblec::lecui::size& size) { size_ = size; }
const liblec::lecui::size& liblec::lecui::widgets::page::page_impl::size() { return size_; }
void liblec::lecui::widgets::page::page_impl::width(const long& width) { size_.width = width; }
const long liblec::lecui::widgets::page::page_impl::width() { return size_.width; }
void liblec::lecui::widgets::page::page_impl::height(const long& height) { size_.height = height; }
const long liblec::lecui::widgets::page::page_impl::height() { return size_.height; }

liblec::lecui::widgets::specs::rectangle&
liblec::lecui::widgets::page::page_impl::add_rectangle(const std::string& name) {
	rectangles_.try_emplace(name, name_, name);
	widgets_.emplace(name, rectangles_.at(name));
	widgets_order_.emplace_back(name);
	return rectangles_.at(name).specs();
}

liblec::lecui::widgets::specs::label&
liblec::lecui::widgets::page::page_impl::add_label(const std::string& name) {
	labels_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, labels_.at(name));
	widgets_order_.emplace_back(name);
	return labels_.at(name).specs();
}

liblec::lecui::widgets::specs::group&
liblec::lecui::widgets::page::page_impl::add_group(const std::string& name) {
	groups_.try_emplace(name, name_, name);
	widgets_.emplace(name, groups_.at(name));
	widgets_order_.emplace_back(name);
	return groups_.at(name).specs();
}

liblec::lecui::widgets::specs::tab_control&
liblec::lecui::widgets::page::page_impl::add_tab_control(const std::string& name) {
	tab_controls_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, tab_controls_.at(name));
	widgets_order_.emplace_back(name);
	return tab_controls_.at(name).specs();
}

liblec::lecui::widgets::specs::button&
liblec::lecui::widgets::page::page_impl::add_button(const std::string& name) {
	buttons_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, buttons_.at(name));
	widgets_order_.emplace_back(name);
	return buttons_.at(name).specs();
}

liblec::lecui::widgets::specs::toggle&
liblec::lecui::widgets::page::page_impl::add_toggle(const std::string& name) {
	toggles_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, toggles_.at(name));
	widgets_order_.emplace_back(name);
	return toggles_.at(name).specs();
}

liblec::lecui::widgets::specs::combo&
liblec::lecui::widgets::page::page_impl::add_combo(const std::string& name) {
	combos_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, combos_.at(name));
	widgets_order_.emplace_back(name);
	return combos_.at(name).specs();
}

liblec::lecui::widgets::specs::list&
liblec::lecui::widgets::page::page_impl::add_list(const std::string& name) {
	lists_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, lists_.at(name));
	widgets_order_.emplace_back(name);
	return lists_.at(name).specs();
}

std::map<std::string, liblec::lecui::widgets_implementation::widget&>&
liblec::lecui::widgets::page::page_impl::widgets() { return widgets_; }

const std::vector<std::string>&
liblec::lecui::widgets::page::page_impl::widgets_order() { return widgets_order_; }

liblec::lecui::widgets_implementation::h_scrollbar&
liblec::lecui::widgets::page::page_impl::h_scrollbar() { return h_scrollbar_; }
liblec::lecui::widgets_implementation::v_scrollbar&
liblec::lecui::widgets::page::page_impl::v_scrollbar() { return v_scrollbar_; }

liblec::lecui::widgets_implementation::group&
liblec::lecui::widgets::page::page_impl::get_group(const std::string& name) { return groups_.at(name); }

liblec::lecui::widgets_implementation::tab_control&
liblec::lecui::widgets::page::page_impl::get_tab_control(const std::string& name) { return tab_controls_.at(name); }

liblec::lecui::widgets_implementation::toggle&
liblec::lecui::widgets::page::page_impl::get_toggle(const std::string& name) { return toggles_.at(name); }

liblec::lecui::widgets_implementation::combo&
liblec::lecui::widgets::page::page_impl::get_combo(const std::string& name) { return combos_.at(name); }

liblec::lecui::widgets_implementation::list&
liblec::lecui::widgets::page::page_impl::get_list(const std::string& name) { return lists_.at(name); }

liblec::lecui::widgets::page::page(const std::string& name) :
	d_page_(*new liblec::lecui::widgets::page::page_impl(name)) {}

liblec::lecui::widgets::page::page() :
	liblec::lecui::widgets::page(std::string()) {}

liblec::lecui::widgets::page::~page() { delete& d_page_; }

inline liblec::lecui::size liblec::lecui::widgets::page::size() { return d_page_.size(); }
