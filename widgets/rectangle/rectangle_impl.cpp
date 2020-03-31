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
#include "../../containers/page/page_impl.h"

class liblec::lecui::widgets::rectangle::rectangle::rectangle_impl {
public:
	rectangle_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::rectangle::rectangle(liblec::lecui::containers::page& page) :
	d_(*(new rectangle_impl(page))) {}

liblec::lecui::widgets::rectangle::~rectangle() { delete& d_; }

liblec::lecui::widgets::specs::rectangle&
liblec::lecui::widgets::rectangle::add(const std::string& name) {
	return d_.page_.d_page_.add_rectangle(name);
}

liblec::lecui::widgets_implementation::rectangle::rectangle(const std::string& page,
	const std::string& name) :
	p_brush_fill_(nullptr),
	p_brush_border_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr) {
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
	if (!resources_created_)
		create_resources(p_render_target);

	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	if (!render || !visible_)
		return rect_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_fill_ : p_brush_disabled_);
	p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_border_ : p_brush_disabled_, specs_.border);

	if (!is_static_ && is_enabled_) {
		if (hit_ || pressed_)
			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ? 1.75f : 1.0f);

		if (selected_)
			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ? 1.75f : 1.0f);
	}

	return rect_;
}

void liblec::lecui::widgets_implementation::rectangle::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::rectangle&
liblec::lecui::widgets_implementation::rectangle::specs() { return specs_; }
