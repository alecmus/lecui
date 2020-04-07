/*
** textbox_impl.cpp - textbox_impl implementation
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

#include "textbox_impl.h"

liblec::lecui::widgets_implementation::textbox::textbox(const std::string& page,
	const std::string& name,
	ID2D1Factory* p_direct2d_factory,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_caret_(nullptr),
	p_brush_prompt_(nullptr),
	p_brush_border_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_direct2d_factory_(p_direct2d_factory),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr),
	caret_position_(0) {
	page_ = page;
	name_ = name;
	h_cursor_ = LoadCursor(NULL, IDC_IBEAM);
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::textbox::~textbox() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::textbox::name() { return name_; }
std::string liblec::lecui::widgets_implementation::textbox::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::textbox::type() {
	return lecui::widgets_implementation::widget_type::textbox;
}

HRESULT liblec::lecui::widgets_implementation::textbox::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	specs_old_ = specs_;
	is_static_ = false;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_fill_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
			&p_brush_border_);
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
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_caret),
			&p_brush_caret_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_prompt),
			&p_brush_prompt_);
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

	resources_created_ = true;
	return hr;
}

void liblec::lecui::widgets_implementation::textbox::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_);
	safe_release(&p_brush_caret_);
	safe_release(&p_brush_prompt_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::textbox::render(ID2D1HwndRenderTarget* p_render_target,
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

	if (!render || !visible_)
		return rect_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_fill_ :
		p_brush_disabled_);

	p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_border_, .5f);

	if (!is_static_ && is_enabled_ && selected_)
		p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, 1.75f);

	// create a text layout
	std::string text_ = !specs_.text.empty() ?
		specs_.text : selected_ ?
		std::string() : specs_.prompt;

	auto rect_text_ = rect_;
	rect_text_.left += 7.5f;
	rect_text_.right -= 7.5f;
	rect_text_.top += 2.5f;
	rect_text_.bottom -= 2.5f;
	
	HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
		(UINT32)text_.length(), p_text_format_, rect_text_.right - rect_text_.left,
		rect_text_.bottom - rect_text_.top, &p_text_layout_);

	if (SUCCEEDED(hr)) {
		// draw the text layout
		p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
			p_text_layout_, !specs_.text.empty() ?
			p_brush_ : p_brush_prompt_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	// draw caret
	if (!is_static_ && is_enabled_ && selected_) {
		DWRITE_HIT_TEST_METRICS hitTestMetrics;
		float caretX, caretY;
		bool isTrailingHit = false; // Use the leading character edge for simplicity here.
		// Map text position index to caret coordinate and hit-test rectangle.
		p_text_layout_->HitTestTextPosition(
			caret_position_,
			isTrailingHit,
			OUT & caretX,
			OUT & caretY,
			OUT & hitTestMetrics
			);

		// Respect user settings.
		DWORD caretWidth = 1;
		SystemParametersInfo(SPI_GETCARETWIDTH, 0, OUT & caretWidth, 0);
		const float halfCaretWidth = caretWidth / 2.f;

		// Draw a thin rectangle.
		float layoutOriginX = 0.f;
		float layoutOriginY = 0.f;
		
		D2D1_RECT_F caretRect = D2D1::RectF(rect_text_.left + hitTestMetrics.left - halfCaretWidth,
			rect_text_.top + hitTestMetrics.top,
			rect_text_.left + hitTestMetrics.left + halfCaretWidth,
			rect_text_.top + hitTestMetrics.top + hitTestMetrics.height);

		p_render_target->FillRectangle(&caretRect, p_brush_caret_);
	}

	// release the text layout
	safe_release(&p_text_layout_);

	return rect_;
}

void liblec::lecui::widgets_implementation::textbox::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::textbox&
liblec::lecui::widgets_implementation::textbox::specs() { return specs_; }

void liblec::lecui::widgets_implementation::textbox::insert_character(const char& c) {
	std::string s;
	s += c;
	specs_.text.insert(caret_position_, s);
	caret_position_++;
}

void liblec::lecui::widgets_implementation::textbox::key_backspace() {
	try {
		specs_.text.erase(caret_position_ - 1, 1);
		caret_position_--;
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_delete() {
	try {
		specs_.text.erase(caret_position_, 1);
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_left() {
	try {
		if (caret_position_ > 0)
			caret_position_--;
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_right() {
	try {
		if (caret_position_ < specs_.text.length())
			caret_position_++;
	}
	catch (const std::exception& e) { log(e.what()); }
}
