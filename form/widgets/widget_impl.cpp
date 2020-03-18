/*
** widget_impl.cpp - widget_impl implementation
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

#include "widget_impl.h"

liblec::lecui::widgets_implementation::widget::widget() :
	page_(std::string()),
	name_(std::string()),
	dpi_scale_(1.0f),
	is_static_(true),
	hit_(false),
	pressed_(false),
	rect_({ .0f, .0f, .0f, .0f }),
	visible_(true),
	is_enabled_(true),
	point_({ 0.f, 0.f }),
	selected_(false),
	point_on_press_({ 0.f,0.f }),
	point_on_release_({ 0.f,0.f }),
	draw_menu_(false) {
	log("entering widget constructor");

	HDC hdc_screen = GetDC(NULL);
	dpi_scale_ = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
	ReleaseDC(NULL, hdc_screen);

	log("dpi_scale_: " + std::to_string(dpi_scale_));
}

liblec::lecui::widgets_implementation::widget::~widget() { log("exiting widget destructor"); }

bool liblec::lecui::widgets_implementation::widget::contains(const D2D1_POINT_2F& point) {
	// capture the point
	point_ = point;

	if (point.x == 0.f && point.y == 0.f)
		return false;

	D2D1_RECT_F rect = rect_;
	scale_RECT(rect, dpi_scale_);

	if (point.x >= rect.left && point.x <= rect.right &&
		point.y >= rect.top && point.y <= rect.bottom)
		return contains();
	else
		return false;
}

bool liblec::lecui::widgets_implementation::widget::hit(const bool& hit) {
	if (is_static_ || hit == hit_)
		return false;

	hit_ = hit;
	return true;
}

void liblec::lecui::widgets_implementation::widget::press(const bool& pressed) {
	bool change = pressed_ != pressed;

	pressed_ = pressed;

	if (change) {
		if (pressed_)
			point_on_press_ = point_;
		else
			point_on_release_ = point_;
	}
}

bool liblec::lecui::widgets_implementation::widget::pressed() { return pressed_; }
bool liblec::lecui::widgets_implementation::widget::is_static() { return is_static_; }
void liblec::lecui::widgets_implementation::widget::show(const bool& show) { visible_ = show; }
bool liblec::lecui::widgets_implementation::widget::visible() { return visible_; }
void liblec::lecui::widgets_implementation::widget::enable(const bool& enable) { is_enabled_ = enable; }
bool liblec::lecui::widgets_implementation::widget::enabled() { return is_enabled_; }
const D2D1_RECT_F& liblec::lecui::widgets_implementation::widget::get_rect() { return rect_; }
bool liblec::lecui::widgets_implementation::widget::selected() { return selected_; }
bool liblec::lecui::widgets_implementation::widget::menu_visible() { return draw_menu_; }
bool liblec::lecui::widgets_implementation::widget::hit() { return hit_; }
bool liblec::lecui::widgets_implementation::widget::on_menu(ID2D1HwndRenderTarget* p_render_target,
	const D2D1_RECT_F& client_area) { return false; }
void liblec::lecui::widgets_implementation::widget::reset_menu() { draw_menu_ = false; }
bool liblec::lecui::widgets_implementation::widget::on_mousewheel(float units) { return false; }
bool liblec::lecui::widgets_implementation::widget::on_keydown(WPARAM wParam) { return false; }
bool liblec::lecui::widgets_implementation::widget::contains() { return true; }

void liblec::lecui::widgets_implementation::widget::select(const bool& selected) {
	bool change = selected_ != selected;
	selected_ = selected;

	if (change)
		if (!selected_)
			reset_menu();
}

D2D1_RECT_F
liblec::lecui::widgets_implementation::widget::position(const liblec::lecui::rect& rect,
	const liblec::lecui::widgets::specs::on_resize& resize,
	const float& change_in_width, const float& change_in_height) {
	float x_change = change_in_width * static_cast<float>(resize.perc_x) / 100.f;
	float y_change = change_in_height * static_cast<float>(resize.perc_y) / 100.f;
	float width_change =
		change_in_width * static_cast<float>(resize.perc_width) / 100.f;
	float height_change =
		change_in_height * static_cast<float>(resize.perc_height) / 100.f;

	float left = static_cast<float>(rect.left) + x_change;
	float top = static_cast<float>(rect.top) + y_change;
	float right = static_cast<float>(rect.right) + x_change + width_change;
	float bottom = static_cast<float>(rect.bottom) + y_change + height_change;

	// apply limits on width and height

	// width
	if ((right - left) < resize.min_width)
		right = left + resize.min_width;

	if (resize.max_width > resize.min_width)
		if ((right - left) > resize.max_width)
			right = left + resize.max_width;

	// height
	if ((bottom - top) < resize.min_height)
		bottom = top + resize.min_height;

	if (resize.max_height > resize.min_height)
		if ((bottom - top) > resize.max_height)
			bottom = top + resize.max_height;

	const float width = right - left;
	const float height = bottom - top;

	// apply limits on x-position

	// x

	if (left < resize.min_x) {
		left = static_cast<float>(resize.min_x);
		right = left + width;
	}

	if (resize.max_x > resize.min_x) {
		if (left > resize.max_x) {
			left = static_cast<float>(resize.max_x);
			right = left + width;
		}
	}

	// y

	if (top < resize.min_y) {
		top = static_cast<float>(resize.min_y);
		bottom = top + height;
	}

	if (resize.max_y > resize.min_y) {
		if (top > resize.max_y) {
			top = static_cast<float>(resize.max_y);
			bottom = top + height;
		}
	}

	return D2D1::RectF(left, top, right, bottom);
}
