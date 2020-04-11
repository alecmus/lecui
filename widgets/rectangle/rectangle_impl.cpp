/*
** rectangle_impl.cpp - rectangle widget implementation
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

#include "rectangle_impl.h"

liblec::lecui::widgets_implementation::rectangle::rectangle(const std::string& page,
	const std::string& name) :
	p_brush_fill_(nullptr),
	p_brush_border_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	x_off_set_(0.f),
	y_off_set_(0.f),
	x_off_set_og_(0.f),
	y_off_set_og_(0.f),
	og_off_set_captured_(false) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::rectangle::~rectangle() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::rectangle::name() { return name_; }
std::string liblec::lecui::widgets_implementation::rectangle::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::rectangle::type() {
	return lecui::widgets_implementation::widget_type::rectangle;
}

HRESULT liblec::lecui::widgets_implementation::rectangle::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	specs_old_ = specs_;
	is_static_ = (specs_.on_click == nullptr);

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_fill_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
			&p_brush_border_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
			&p_brush_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
			&p_brush_disabled_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
			&p_brush_selected_);

	resources_created_ = true;
	return hr;
}

void liblec::lecui::widgets_implementation::rectangle::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::rectangle::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	if (specs_old_ != specs_) {
		log("specs changed: " + name_);
		specs_old_ = specs_;
		discard_resources();
	}

	if (!resources_created_)
		create_resources(p_render_target);

	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	if (name_ == "minimal_page_border_rect") {
		// this is a special rectangle useful for the proper functioning of page
		// scroll bars. Capture the initial offset so that changes made by the scrollbars can
		// be ignored in the contains() virtual function override. If this is not done
		// the page virtual "hit" area moves with the rectangles when the scrollbars are moved!!!!!
		if (!og_off_set_captured_) {
			x_off_set_og_ = x_off_set;
			y_off_set_og_ = y_off_set;
			og_off_set_captured_ = true;
		}

		x_off_set_ = x_off_set;
		y_off_set_ = y_off_set;
	}

	if (!render || !visible_)
		return rect_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ?
		p_brush_fill_ : p_brush_disabled_);
	p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
		p_brush_border_ : p_brush_disabled_, specs_.border);

	if (!is_static_ && is_enabled_) {
		if (hit_ || pressed_)
			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ?
				1.75f : 1.f);

		if (selected_)
			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ?
				1.75f : 1.f);
	}

	return rect_;
}

void liblec::lecui::widgets_implementation::rectangle::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

bool liblec::lecui::widgets_implementation::rectangle::contains(const D2D1_POINT_2F& point) {
	// capture the point
	point_ = point;

	D2D1_RECT_F rect = rect_;

	if (name_ == "minimal_page_border_rect") {
		// this is a special rectangle used to manage pages.
		// scrollbar movements move everything, including the minimal page border rect.
		// keep the page virtual hit area over the actual page by ignoring scrollbar movements.
		auto x_change = x_off_set_ - x_off_set_og_;
		auto y_change = y_off_set_ - y_off_set_og_;

		rect.left += x_change;
		rect.right += x_change;
		rect.top += y_change;
		rect.bottom += y_change;
	}

	if (point.x == 0.f && point.y == 0.f)
		return false;

	scale_RECT(rect, dpi_scale_);

	if (point.x >= rect.left && point.x <= rect.right &&
		point.y >= rect.top && point.y <= rect.bottom)
		return true;
	else
		return false;
}

liblec::lecui::widgets::specs::rectangle&
liblec::lecui::widgets_implementation::rectangle::specs() { return specs_; }
