/*
** pane_impl.cpp - pane_impl implementation
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

#include "pane_impl.h"
#include "../page/page_impl.h"

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

liblec::lecui::containers::page& liblec::lecui::containers::pane::get() {
	auto& pane_ = d_.page_.d_page_.get_pane(d_.name_);

	log("liblec::lecui::containers::pane::add");

	const std::string name = "pane";

	if (pane_.p_panes_.count(name)) {
		log("library usage error liblec::lecui::containers::pane::add");

		/// to-do: find a mechanism that makes sense ...
		return pane_.p_panes_.at(name);
	}

	pane_.p_panes_.emplace(name, name);
	pane_.current_pane_ = name;

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

liblec::lecui::widgets_implementation::pane::pane(const std::string& page,
	const std::string& name) :
	p_brush_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_border_(nullptr),
	p_brush_disabled_(nullptr),
	margin_(12.f),
	rect_client_area_({ 0.f, 0.f, 0.f, 0.f }),
	rect_pane_({ 0.f, 0.f, 0.f, 0.f }) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::pane::~pane() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::pane::name() { return name_; }
std::string liblec::lecui::widgets_implementation::pane::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::pane::type() {
	return lecui::widgets_implementation::widget_type::pane;
}

HRESULT liblec::lecui::widgets_implementation::pane::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = false;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
			&p_brush_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_fill_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
			&p_brush_border_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
			&p_brush_disabled_);

	resources_created_ = true;
	return hr;
}

void liblec::lecui::widgets_implementation::pane::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_disabled_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::pane::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height,
	float x_off_set, float y_off_set, const bool& render) {
	if (!resources_created_)
		create_resources(p_render_target);

	rect_pane_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_pane_.left -= x_off_set;
	rect_pane_.right -= x_off_set;
	rect_pane_.top -= y_off_set;
	rect_pane_.bottom -= y_off_set;

	// no reponse to hit testing, even though for scroll bar at form level
	// we need to return the entire region through rect_pane_
	rect_ = { 0.f, 0.f, 0.f, 0.f };

	rect_client_area_ = rect_pane_;

	if (!render || !visible_)
		return rect_pane_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_client_area_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	if (specs_.is_filled)
		p_render_target->FillRoundedRectangle(&rounded_rect,
			is_enabled_ ? p_brush_fill_ : p_brush_disabled_);
	
	p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
		p_brush_border_ : p_brush_disabled_, specs_.border);

	return rect_pane_;
}

void liblec::lecui::widgets_implementation::pane::on_click() {}

liblec::lecui::containers::specs::pane&
liblec::lecui::widgets_implementation::pane::specs() { return specs_; }

const D2D1_RECT_F& liblec::lecui::widgets_implementation::pane::client_area() {
	return rect_client_area_;
}

const D2D1_RECT_F& liblec::lecui::widgets_implementation::pane::pane_area() {
	return rect_pane_;
}

bool liblec::lecui::widgets_implementation::pane::contains() { return false; }
