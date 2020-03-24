/*
** toggle_impl.cpp - toggle widget implementation
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

#include "toggle_impl.h"
#include "../page/page_impl.h"

class liblec::lecui::widgets::toggle::toggle::toggle_impl {
public:
	toggle_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::toggle::toggle(liblec::lecui::containers::page& page) :
	d_(*(new toggle_impl(page))) {}

liblec::lecui::widgets::toggle::~toggle() { delete& d_; }

liblec::lecui::widgets::specs::toggle& liblec::lecui::widgets::toggle::add(const std::string& name) {
	return d_.page_.d_page_.add_toggle(name);
}

liblec::lecui::widgets_implementation::toggle::toggle(const std::string& page,
	const std::string& name,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_on_(nullptr),
	p_brush_off_(nullptr),
	p_brush_on_hot_(nullptr),
	p_brush_off_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr),
	perc_along_(0.f) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::toggle::~toggle() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::toggle::name() { return name_; }
std::string liblec::lecui::widgets_implementation::toggle::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::toggle::type() {
	return lecui::widgets_implementation::widget_type::toggle;
}

HRESULT liblec::lecui::widgets_implementation::toggle::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = (specs_.on_toggle == nullptr && specs_.on_click == nullptr);

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_on),
			&p_brush_on_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_off),
			&p_brush_off_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(lighten_color(specs_.color_on, 25)),
			&p_brush_on_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(lighten_color(specs_.color_off, 25)),
			&p_brush_off_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
			&p_brush_disabled_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
			&p_brush_selected_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
			&p_brush_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_fill_);
	if (SUCCEEDED(hr)) {
		// Create a DirectWrite text format object.
		hr = p_directwrite_factory_->CreateTextFormat(
			convert_string(specs_.font).c_str(),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			convert_fontsize_to_dip(specs_.font_size),
			L"", //locale
			&p_text_format_
		);
	}
	if (SUCCEEDED(hr)) {
		p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		make_single_line(p_directwrite_factory_, p_text_format_);
	}

	return hr;
}

void liblec::lecui::widgets_implementation::toggle::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	safe_release(&p_brush_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_on_);
	safe_release(&p_brush_off_);
	safe_release(&p_brush_on_hot_);
	safe_release(&p_brush_off_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::toggle::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	const float toggle_height = rect_.bottom - rect_.top;
	const float toggle_width = 2.f * toggle_height;

	rect_toggle_ = rect_;
	rect_toggle_.right = rect_toggle_.left + toggle_width;

	D2D1_RECT_F toggle_background_ = rect_toggle_;
	D2D1_ROUNDED_RECT toggle_background_round_ = { toggle_background_, (toggle_background_.bottom - toggle_background_.top) / 2.f, (toggle_background_.bottom - toggle_background_.top) / 2.f };

	D2D1_RECT_F toggle_foreground_ = rect_toggle_;
	toggle_foreground_.right = toggle_foreground_.right - (toggle_foreground_.right - toggle_foreground_.left) / 2.f;

	perc_along_ = specs_.on ? 100.f : 0.f;

	bool x_change = (point_.x != point_on_press_.x);
	bool y_change = (point_.y != point_on_press_.y);

	if (pressed_ && (x_change || y_change)) {
		D2D1_RECT_F rect = rect_toggle_;
		rect.left += ((toggle_foreground_.right - toggle_foreground_.left) / 2.f);
		rect.right -= ((toggle_foreground_.right - toggle_foreground_.left) / 2.f);

		scale_RECT(rect, dpi_scale_);

		const float mid_point_x = rect.left + ((rect.right - rect.left) / 2.f);

		perc_along_ = 100.f * (point_.x - rect.left) / (rect.right - rect.left);
		perc_along_ = largest(perc_along_, 0.f);
		perc_along_ = smallest(perc_along_, 100.f);
	}

	pos_rect(rect_toggle_, toggle_foreground_, perc_along_, 50.f);

	// to-do: shrink by exactly 90%
	const float shrink_amount = 0.05f * (toggle_foreground_.right - toggle_foreground_.left);
	toggle_foreground_.left += shrink_amount;
	toggle_foreground_.right -= shrink_amount;
	toggle_foreground_.top += shrink_amount;
	toggle_foreground_.bottom -= shrink_amount;

	D2D1_ROUNDED_RECT toggle_foreground_round_ = { toggle_foreground_, (toggle_foreground_.bottom - toggle_foreground_.top) / 2.f, (toggle_foreground_.bottom - toggle_foreground_.top) / 2.f };

	ID2D1SolidColorBrush* p_brush = (perc_along_ >= 50.f) ? p_brush_on_ : p_brush_off_;
	ID2D1SolidColorBrush* p_brush_hot = (perc_along_ >= 50.f) ? p_brush_on_hot_ : p_brush_off_hot_;

	if (render && visible_) {
		p_render_target->FillRoundedRectangle(&toggle_background_round_, !is_enabled_ ? p_brush_disabled_ : hit_ ? p_brush_hot : p_brush);
		p_render_target->FillRoundedRectangle(&toggle_foreground_round_, !is_enabled_ ? p_brush_fill_ : p_brush_fill_);
	}

	rect_text_ = rect_;
	rect_text_.left = rect_toggle_.right + ((rect_toggle_.bottom - rect_toggle_.top) / 3.f);

	// create a text layout
	const std::string& text = specs_.on ? specs_.text : specs_.text_off;
	HRESULT hr = p_directwrite_factory_->CreateTextLayout(
		convert_string(text).c_str(),
		(UINT32)text.length(),
		p_text_format_,
		rect_text_.right - rect_text_.left,
		rect_text_.bottom - rect_text_.top,
		&p_text_layout_);

	if (SUCCEEDED(hr)) {
		DWRITE_TEXT_METRICS textMetrics;
		p_text_layout_->GetMetrics(&textMetrics);

		const auto rect_text_og = rect_text_;
		rect_text_.left += textMetrics.left;
		rect_text_.top += textMetrics.top;
		rect_text_.right = smallest(rect_text_.left + textMetrics.width, rect_text_.right);
		rect_text_.bottom = smallest(rect_text_.top + textMetrics.height, rect_text_.bottom);

		if (render && visible_ && !is_static_ && is_enabled_ && selected_)
			p_render_target->FillRectangle(&rect_text_, p_brush_selected_);

		if (render && visible_)
			p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_og.left, rect_text_og.top },
				p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	rect_.right = rect_text_.right;

	// release the text layout
	safe_release(&p_text_layout_);

	return rect_;
}

void liblec::lecui::widgets_implementation::toggle::on_click() {
	// to-do: how to know if this is a keyboard and not care about either x_change or y_change

	bool x_change = (point_.x != point_on_press_.x);
	bool y_change = (point_.y != point_on_press_.y);

	if (!(x_change || y_change)) {
		specs_.on = !specs_.on;	// A click with no mouse move. Toggle.
		perc_along_ = specs_.on ? 100.f : 0.f;
	}
	else
		specs_.on = perc_along_ >= 50.f;

	if (specs_.on_toggle)
		specs_.on_toggle(specs_.on);

	if (specs_.on_click)
		specs_.on_click();
}

bool liblec::lecui::widgets_implementation::toggle::contains(const D2D1_POINT_2F& point) {
	// capture the point
	point_ = point;

	if (point.x == 0.f && point.y == 0.f)
		return false;

	D2D1_RECT_F rect = rect_;
	scale_RECT(rect, dpi_scale_);

	if (point.x >= rect.left && point.x <= rect.right &&
		point.y >= rect.top && point.y <= rect.bottom)
		return contains();
	else {
		if (pressed_)
			return true;
		else
			return false;
	}
}

bool liblec::lecui::widgets_implementation::toggle::contains() { return true; }

bool liblec::lecui::widgets_implementation::toggle::hit(const bool& hit) {
	if (is_static_ || hit == hit_) {
		if (pressed_)
			return true;
		else
			return false;
	}

	hit_ = hit;
	return true;
}

liblec::lecui::widgets::specs::toggle&
liblec::lecui::widgets_implementation::toggle::specs() { return specs_; }
