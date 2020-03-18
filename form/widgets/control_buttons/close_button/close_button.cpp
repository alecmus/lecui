/*
** close_button.cpp - close button widget implementation
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

#include "close_button.h"

liblec::lecui::widgets_implementation::close_button::close_button() :
	p_brush_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr) {
	page_ = "";
	name_ = "close_button";
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::close_button::~close_button() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::close_button::name() { return name_; }
std::string liblec::lecui::widgets_implementation::close_button::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::close_button::type() {
	return lecui::widgets_implementation::widget_type::close_button;
}

HRESULT liblec::lecui::widgets_implementation::close_button::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = false;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
			&p_brush_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
			&p_brush_disabled_);

	return hr;
}

void liblec::lecui::widgets_implementation::close_button::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	safe_release(&p_brush_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::close_button::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	ID2D1SolidColorBrush* p_brush_current_ = is_enabled_ ? p_brush_ : p_brush_disabled_;

	if (!is_static_ && is_enabled_ && hit_)
		p_brush_current_ = p_brush_hot_;

	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);

	if (!is_static_ && hit_)
		p_render_target->DrawRectangle(&rect_, p_brush_, pressed_ ? 0.5f : 0.0f);

	if (!render || !visible_)
		return rect_;

	// draw close button
	D2D1_RECT_F specs_(rect_);

	float margin_ = (rect_.right - rect_.left) * 0.30f;

	specs_.left += margin_;
	specs_.top += margin_;
	specs_.right -= margin_;
	specs_.bottom -= margin_;

	p_render_target->DrawLine(
		D2D1_POINT_2F{ specs_.left, specs_.top },
		D2D1_POINT_2F{ specs_.right, specs_.bottom },
		p_brush_current_, 2.0f);
	p_render_target->DrawLine(
		D2D1_POINT_2F{ specs_.right, specs_.top },
		D2D1_POINT_2F{ specs_.left, specs_.bottom },
		p_brush_current_, 2.0f);

	return rect_;
}

void liblec::lecui::widgets_implementation::close_button::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::close_button&
liblec::lecui::widgets_implementation::close_button::specs() { return specs_; }
