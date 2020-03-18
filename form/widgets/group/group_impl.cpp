/*
** group.cpp - group widget implementation
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

#include "group_impl.h"
#include "../page/page_impl.h"

class liblec::lecui::widgets::group::group::group_impl {
public:
	group_impl(liblec::lecui::widgets::page& page) :
		page_(page) {}
	liblec::lecui::widgets::page& page_;
};

liblec::lecui::widgets::group::group(liblec::lecui::widgets::page& page) :
	d_(*(new group_impl(page))) {}

liblec::lecui::widgets::group::~group() { delete& d_; }

liblec::lecui::widgets::specs::group&
liblec::lecui::widgets::group::add(const std::string& name) { return d_.page_.d_page_.add_group(name); }

liblec::lecui::widgets_implementation::group::group(const std::string& page,
	const std::string& name) :
	p_brush_fill_(nullptr),
	p_brush_border_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::group::~group() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::group::name() { return name_; }
std::string liblec::lecui::widgets_implementation::group::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::group::type() {
	return lecui::widgets_implementation::widget_type::group;
}

HRESULT liblec::lecui::widgets_implementation::group::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = true;

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

	return hr;
}

void liblec::lecui::widgets_implementation::group::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::group::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	rect_ = convert_rect(specs_.rect);

	if (!render || !visible_)
		return rect_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	if (specs_.is_filled)
		p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_fill_ : p_brush_disabled_);
	else
		p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_border_ : p_brush_disabled_, specs_.border);

	if (!is_static_ && is_enabled_ && hit_)
		p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ? 1.5f : 0.8f);

	return rect_;
}

void liblec::lecui::widgets_implementation::group::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::group&
liblec::lecui::widgets_implementation::group::specs() { return specs_; }
