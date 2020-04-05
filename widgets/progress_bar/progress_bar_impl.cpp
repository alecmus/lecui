/*
** progress_bar_impl.cpp - progress_bar_impl implementation
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

#include "progress_bar_impl.h"

liblec::lecui::widgets_implementation::progress_bar::progress_bar(const std::string& page,
	const std::string& name,
	ID2D1Factory* p_direct2d_factory,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_border_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_direct2d_factory_(p_direct2d_factory),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::progress_bar::~progress_bar() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::progress_bar::name() { return name_; }
std::string liblec::lecui::widgets_implementation::progress_bar::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::progress_bar::type() {
	return lecui::widgets_implementation::widget_type::progress_bar;
}

HRESULT liblec::lecui::widgets_implementation::progress_bar::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	specs_old_ = specs_;
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
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
			&p_brush_selected_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
			&p_brush_);
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
		p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		make_single_line(p_directwrite_factory_, p_text_format_);
	}

	resources_created_ = true;
	return hr;
}

void liblec::lecui::widgets_implementation::progress_bar::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::progress_bar::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	if (specs_old_ != specs_) {
		log("specs changed: " + name_);
		specs_old_ = specs_;
		discard_resources();
	}

	if (!resources_created_)
		create_resources(p_render_target);

	// sanity check
	specs_.percentage = smallest(specs_.percentage, 100.f);
	specs_.percentage = largest(specs_.percentage, 0.f);

	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	if (!render || !visible_)
		return rect_;

	// draw unfilled rect
	auto corner_radius = smallest((rect_.bottom - rect_.top) / 3.f,
		(rect_.right - rect_.left) / 3.f);
	D2D1_ROUNDED_RECT unfilled_rect{ rect_, corner_radius, corner_radius };
	p_render_target->DrawRoundedRectangle(unfilled_rect, p_brush_border_, specs_.border);

	// draw filled rect
	auto rect_subject = rect_;
	rect_subject.left += (2.f * specs_.border);
	rect_subject.right -= (2.f * specs_.border);
	rect_subject.top += (2.f * specs_.border);
	rect_subject.bottom -= (2.f * specs_.border);
	corner_radius = smallest((rect_subject.bottom - rect_subject.top) / 3.f,
		(rect_subject.right - rect_subject.left) / 3.f);

	{
		// define rectangle that should contain fill
		auto rect_fill = rect_subject;
		rect_fill.right = rect_fill.left + (specs_.percentage * (rect_fill.right - rect_fill.left) / 100.f);

		// clip
		auto_clip clip(render, p_render_target, rect_fill, 0.f );

		D2D1_ROUNDED_RECT filled_rect{ rect_subject, corner_radius, corner_radius };
		p_render_target->FillRoundedRectangle(filled_rect, p_brush_fill_);
	}

	return rect_;
}

void liblec::lecui::widgets_implementation::progress_bar::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::progress_bar&
liblec::lecui::widgets_implementation::progress_bar::specs() { return specs_; }
