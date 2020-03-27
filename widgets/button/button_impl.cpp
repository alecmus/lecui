/*
** button_impl.cpp - button widget implementation
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

#include "button_impl.h"
#include "../../containers/page/page_impl.h"

class liblec::lecui::widgets::button::button::button_impl {
public:
	button_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::button::button(liblec::lecui::containers::page& page) :
	d_(*(new button_impl(page))) {}

liblec::lecui::widgets::button::~button() { delete& d_; }

liblec::lecui::widgets::specs::button&
liblec::lecui::widgets::button::add(const std::string& name) { return d_.page_.d_page_.add_button(name); }

liblec::lecui::widgets_implementation::button::button(const std::string& page,
	const std::string& name,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_border_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::button::~button() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::button::name() { return name_; }
std::string liblec::lecui::widgets_implementation::button::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::button::type() {
	return lecui::widgets_implementation::widget_type::button;
}

HRESULT liblec::lecui::widgets_implementation::button::create_resources(
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

	return hr;
}

void liblec::lecui::widgets_implementation::button::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	safe_release(&p_brush_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::button::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	if (!render || !visible_)
		return rect_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	p_render_target->FillRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
		is_static_ ? p_brush_fill_ :
		hit_ && pressed_ ? p_brush_fill_ :
		hit_ ? p_brush_hot_ :
		p_brush_fill_);

	p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
		p_brush_border_, 0.5f);

	if (!is_static_ && is_enabled_ && selected_)
		p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
			p_brush_selected_, pressed_ ? 1.75f : 1.0f);

	// create a text layout
	HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(specs_.text).c_str(),
		(UINT32)specs_.text.length(), p_text_format_, rect_.right - rect_.left,
		rect_.bottom - rect_.top, &p_text_layout_);

	if (SUCCEEDED(hr)) {
		// draw the text layout
		p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_.left, rect_.top },
			p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	// release the text layout
	safe_release(&p_text_layout_);

	return rect_;
}

void liblec::lecui::widgets_implementation::button::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::button&
liblec::lecui::widgets_implementation::button::specs() { return specs_; }
