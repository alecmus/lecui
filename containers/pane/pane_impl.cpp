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
#include "../../containers/page/page_impl.h"

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
