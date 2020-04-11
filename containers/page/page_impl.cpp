/*
** page_impl.cpp - page_impl implementation
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

liblec::lecui::containers::page::page_impl::page_impl(form& fm, const std::string& name) :
	fm_(fm),
	p_directwrite_factory_(nullptr),
	p_iwic_factory_(nullptr),
	name_(name),
	h_scrollbar_(name),
	v_scrollbar_(name) {
	log("constructor: " + name_);
	widgets_.emplace(h_scrollbar_.name(), h_scrollbar_);
	widgets_.emplace(v_scrollbar_.name(), v_scrollbar_);
}

liblec::lecui::containers::page::page_impl::~page_impl() { log("destructor:  " + name_); }
std::string liblec::lecui::containers::page::page_impl::name() { return name_; }

void liblec::lecui::containers::page::page_impl::direct2d_factory(ID2D1Factory* p_direct2d_factory) {
	p_direct2d_factory_ = p_direct2d_factory;
}

ID2D1Factory* liblec::lecui::containers::page::page_impl::direct2d_factory() {
	return p_direct2d_factory_;
}

void
liblec::lecui::containers::page::page_impl::directwrite_factory(IDWriteFactory* p_directwrite_factory) {
	p_directwrite_factory_ = p_directwrite_factory;
}

IDWriteFactory* liblec::lecui::containers::page::page_impl::directwrite_factory() {
	return p_directwrite_factory_;
}

void
liblec::lecui::containers::page::page_impl::iwic_factory(IWICImagingFactory* p_iwic_factory) {
	p_iwic_factory_ = p_iwic_factory;
}

IWICImagingFactory* liblec::lecui::containers::page::page_impl::iwic_factory() {
	return p_iwic_factory_;
}

void liblec::lecui::containers::page::page_impl::size(const liblec::lecui::size& size) { size_ = size; }
const liblec::lecui::size& liblec::lecui::containers::page::page_impl::size() { return size_; }
void liblec::lecui::containers::page::page_impl::width(const float& width) { size_.width = width; }
const float liblec::lecui::containers::page::page_impl::width() { return size_.width; }
void liblec::lecui::containers::page::page_impl::height(const float& height) { size_.height = height; }
const float liblec::lecui::containers::page::page_impl::height() { return size_.height; }

bool liblec::lecui::containers::page::page_impl::contains(const D2D1_POINT_2F& point) {
	try {
		// check if minimal page border rect contains the point
		auto& rect = rectangles_.at("minimal_page_border_rect");
		return rect.contains(point);
	}
	catch (const std::exception&) {}
	return false;
}

liblec::lecui::containers::specs::tab_control&
liblec::lecui::containers::page::page_impl::add_tab_control(const std::string& name) {
	tab_controls_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, tab_controls_.at(name));
	widgets_order_.emplace_back(name);
	return tab_controls_.at(name).specs();
}

liblec::lecui::containers::specs::pane&
liblec::lecui::containers::page::page_impl::add_pane(const std::string& name) {
	panes_.try_emplace(name, name_, name);
	widgets_.emplace(name, panes_.at(name));
	widgets_order_.emplace_back(name);
	return panes_.at(name).specs();
}

liblec::lecui::widgets::specs::rectangle&
liblec::lecui::containers::page::page_impl::add_rectangle(const std::string& name) {
	rectangles_.try_emplace(name, name_, name);
	widgets_.emplace(name, rectangles_.at(name));
	widgets_order_.emplace_back(name);
	return rectangles_.at(name).specs();
}

liblec::lecui::widgets::specs::label&
liblec::lecui::containers::page::page_impl::add_label(const std::string& name) {
	labels_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, labels_.at(name));
	widgets_order_.emplace_back(name);
	return labels_.at(name).specs();
}

liblec::lecui::widgets::specs::group&
liblec::lecui::containers::page::page_impl::add_group(const std::string& name) {
	groups_.try_emplace(name, name_, name);
	widgets_.emplace(name, groups_.at(name));
	widgets_order_.emplace_back(name);
	return groups_.at(name).specs();
}

liblec::lecui::widgets::specs::button&
liblec::lecui::containers::page::page_impl::add_button(const std::string& name) {
	buttons_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, buttons_.at(name));
	widgets_order_.emplace_back(name);
	return buttons_.at(name).specs();
}

liblec::lecui::widgets::specs::toggle&
liblec::lecui::containers::page::page_impl::add_toggle(const std::string& name) {
	toggles_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, toggles_.at(name));
	widgets_order_.emplace_back(name);
	return toggles_.at(name).specs();
}

liblec::lecui::widgets::specs::combo&
liblec::lecui::containers::page::page_impl::add_combo(const std::string& name) {
	combos_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, combos_.at(name));
	widgets_order_.emplace_back(name);
	return combos_.at(name).specs();
}

liblec::lecui::widgets::specs::list&
liblec::lecui::containers::page::page_impl::add_list(const std::string& name) {
	lists_.try_emplace(name, name_, name, p_directwrite_factory_);
	widgets_.emplace(name, lists_.at(name));
	widgets_order_.emplace_back(name);
	return lists_.at(name).specs();
}

liblec::lecui::widgets::specs::custom&
liblec::lecui::containers::page::page_impl::add_custom(const std::string& name) {
	customs_.try_emplace(name, name_, name, p_directwrite_factory_, p_iwic_factory_);
	widgets_.emplace(name, customs_.at(name));
	widgets_order_.emplace_back(name);
	return customs_.at(name).specs();
}

liblec::lecui::widgets::specs::image&
liblec::lecui::containers::page::page_impl::add_image(const std::string& name) {
	images_.try_emplace(name, name_, name, p_iwic_factory_);
	widgets_.emplace(name, images_.at(name));
	widgets_order_.emplace_back(name);
	return images_.at(name).specs();
}

liblec::lecui::widgets::specs::progress_indicator&
liblec::lecui::containers::page::page_impl::add_progress_indicator(const std::string& name) {
	progress_indicators_.try_emplace(name, name_, name, p_direct2d_factory_, p_directwrite_factory_);
	widgets_.emplace(name, progress_indicators_.at(name));
	widgets_order_.emplace_back(name);
	return progress_indicators_.at(name).specs();
}

liblec::lecui::widgets::specs::progress_bar&
liblec::lecui::containers::page::page_impl::add_progress_bar(const std::string& name) {
	progress_bars_.try_emplace(name, name_, name, p_direct2d_factory_, p_directwrite_factory_);
	widgets_.emplace(name, progress_bars_.at(name));
	widgets_order_.emplace_back(name);
	return progress_bars_.at(name).specs();
}

liblec::lecui::widgets::specs::checkbox&
liblec::lecui::containers::page::page_impl::add_checkbox(const std::string& name) {
	checkboxes_.try_emplace(name, name_, name, p_direct2d_factory_, p_directwrite_factory_);
	widgets_.emplace(name, checkboxes_.at(name));
	widgets_order_.emplace_back(name);
	return checkboxes_.at(name).specs();
}

liblec::lecui::widgets::specs::textbox&
liblec::lecui::containers::page::page_impl::add_textbox(const std::string& name) {
	textboxes_.try_emplace(name, name_, name, fm_, p_directwrite_factory_);
	widgets_.emplace(name, textboxes_.at(name));
	widgets_order_.emplace_back(name);
	return textboxes_.at(name).specs();
}

std::map<std::string, liblec::lecui::widgets_implementation::widget&>&
liblec::lecui::containers::page::page_impl::widgets() { return widgets_; }

const std::vector<std::string>&
liblec::lecui::containers::page::page_impl::widgets_order() { return widgets_order_; }

liblec::lecui::widgets_implementation::h_scrollbar&
liblec::lecui::containers::page::page_impl::h_scrollbar() { return h_scrollbar_; }
liblec::lecui::widgets_implementation::v_scrollbar&
liblec::lecui::containers::page::page_impl::v_scrollbar() { return v_scrollbar_; }

liblec::lecui::widgets_implementation::tab_control&
liblec::lecui::containers::page::page_impl::get_tab_control(const std::string& name) { return tab_controls_.at(name); }

liblec::lecui::widgets_implementation::pane&
liblec::lecui::containers::page::page_impl::get_pane(const std::string& name) { return panes_.at(name); }

liblec::lecui::widgets_implementation::rectangle&
liblec::lecui::containers::page::page_impl::get_rectangle(const std::string& name) { return rectangles_.at(name); }

liblec::lecui::widgets_implementation::button&
liblec::lecui::containers::page::page_impl::get_button(const std::string& name) { return buttons_.at(name); }

liblec::lecui::widgets_implementation::label&
liblec::lecui::containers::page::page_impl::get_label(const std::string& name) { return labels_.at(name); }

liblec::lecui::widgets_implementation::group&
liblec::lecui::containers::page::page_impl::get_group(const std::string& name) { return groups_.at(name); }

liblec::lecui::widgets_implementation::toggle&
liblec::lecui::containers::page::page_impl::get_toggle(const std::string& name) { return toggles_.at(name); }

liblec::lecui::widgets_implementation::combo&
liblec::lecui::containers::page::page_impl::get_combo(const std::string& name) { return combos_.at(name); }

liblec::lecui::widgets_implementation::list&
liblec::lecui::containers::page::page_impl::get_list(const std::string& name) { return lists_.at(name); }

liblec::lecui::widgets_implementation::custom&
liblec::lecui::containers::page::page_impl::get_custom(const std::string& name) { return customs_.at(name); }

liblec::lecui::widgets_implementation::image&
liblec::lecui::containers::page::page_impl::get_image(const std::string& name) { return images_.at(name); }

liblec::lecui::widgets_implementation::progress_indicator&
liblec::lecui::containers::page::page_impl::get_progress_indicator(const std::string& name) { return progress_indicators_.at(name); }

liblec::lecui::widgets_implementation::progress_bar&
liblec::lecui::containers::page::page_impl::get_progress_bar(const std::string& name) { return progress_bars_.at(name); }

liblec::lecui::widgets_implementation::checkbox&
liblec::lecui::containers::page::page_impl::get_checkbox(const std::string& name) { return checkboxes_.at(name); }

liblec::lecui::widgets_implementation::textbox&
liblec::lecui::containers::page::page_impl::get_textbox(const std::string& name) { return textboxes_.at(name); }

bool
liblec::lecui::containers::page::page_impl::close_widget(const std::string& name,
	widgets_implementation::widget_type type,
	std::string& error) {
	// to-do: prevent erasure from widgets_ only to throw later in the switch because the data
	// was wrong to begin with!!!!!
	try {
		///
		/// close widget by
		/// 1. removing it from widgets_
		/// 2. removing it from the page it's in, e.g. tab_controls_ for tab controls
		/// 3. removing it from widgets_order_

		// step 1
		widgets_.erase(name);

		// step 2
		switch (type) {
		case widgets_implementation::widget_type::rectangle:
			rectangles_.erase(name);
			break;
		case widgets_implementation::widget_type::label:
			labels_.erase(name);
			break;
		case widgets_implementation::widget_type::group:
			groups_.erase(name);
			break;
		case widgets_implementation::widget_type::tab_control:
			tab_controls_.erase(name);
			break;
		case widgets_implementation::widget_type::button:
			buttons_.erase(name);
			break;
		case widgets_implementation::widget_type::toggle:
			toggles_.erase(name);
			break;
		case widgets_implementation::widget_type::combo:
			combos_.erase(name);
			break;
		case widgets_implementation::widget_type::list:
			lists_.erase(name);
			break;
		case widgets_implementation::widget_type::custom:
			customs_.erase(name);
			break;
		case widgets_implementation::widget_type::pane:
			panes_.erase(name);
			break;
		case widgets_implementation::widget_type::image:
			images_.erase(name);
			break;
		case widgets_implementation::widget_type::progress_indicator:
			progress_indicators_.erase(name);
			break;
		case widgets_implementation::widget_type::progress_bar:
			progress_bars_.erase(name);
			break;
		case widgets_implementation::widget_type::checkbox:
			checkboxes_.erase(name);
			break;
		case widgets_implementation::widget_type::textbox:
			textboxes_.erase(name);
			break;
		case widgets_implementation::widget_type::close_button:
		case widgets_implementation::widget_type::maximize_button:
		case widgets_implementation::widget_type::minimize_button:
		case widgets_implementation::widget_type::h_scrollbar:
		case widgets_implementation::widget_type::v_scrollbar:
		default:
			break;
		}

		// step 3
		std::vector<std::string> widgets_order;
		for (auto it : widgets_order_)
			if (it != name)
				widgets_order.push_back(it);
		widgets_order_ = widgets_order;

		return true;
	}
	catch (const std::exception& e) {
		error = e.what();
		return false;
	}
}
