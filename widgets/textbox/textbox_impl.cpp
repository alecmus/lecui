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
#include "../timer.h"
#include "../label/label_impl.h"

liblec::lecui::widgets_implementation::textbox::textbox(const std::string& page,
	const std::string& name,
	form& fm,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_caret_(nullptr),
	p_brush_prompt_(nullptr),
	p_brush_border_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	fm_(fm),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr),
	margin_x_(7.5f),
	margin_y_(2.5f),
	caret_blink_timer_name_("caret_blink_timer::textbox"),
	caret_position_(0),
	caret_visible_(true),
	text_off_set_(0.f),
	is_selecting_(false),
	is_selected_(false),
	selection_info_({ 0, 0 }) {
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
	rect_text_.left += margin_x_;
	rect_text_.right -= margin_x_;
	rect_text_.top += margin_y_;
	rect_text_.bottom -= margin_y_;

	// define rectangle for clipping text
	auto rect_text_clip_ = rect_;
	rect_text_clip_.left += (margin_x_ / 3.f);
	rect_text_clip_.right -= (margin_x_ / 3.f);
	rect_text_clip_.top += (margin_y_ / 3.f);
	rect_text_clip_.bottom -= (margin_y_ / 3.f);

	// define text box rect and actual text rect (with possible overflow)
	const auto rect_text_box_ = rect_text_;
	rect_text_ = measure_text(p_directwrite_factory_,
		text_, specs_.font, specs_.font_size, false, true, true, false, rect_text_);

	// measure the text up to the caret position
	const auto text_to_caret = text_.substr(0, caret_position_);
	const auto rect_up_to_caret_ = measure_text(p_directwrite_factory_,
		text_to_caret, specs_.font, specs_.font_size, false, true, true, false, rect_text_);

	// compute offset
	UINT32 hidden_left = 0;
	UINT32 hidden_right = 0;
	const float off_set_right = ((rect_text_.right - rect_text_.left) - (rect_text_box_.right - rect_text_box_.left)) + text_off_set_;
	{
		HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
			(UINT32)text_.length(), p_text_format_, rect_text_.right - rect_text_.left,
			rect_text_.bottom - rect_text_.top, &p_text_layout_);

		// characters hidden to the left of text box
		const D2D1_POINT_2F pt_left = D2D1::Point2F(rect_text_.left - text_off_set_, rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f);
		hidden_left = count_characters(p_text_layout_, text_, rect_text_, pt_left, dpi_scale_);

		// characters hidden to the right of text box
		
		const D2D1_POINT_2F pt_right = D2D1::Point2F(rect_text_box_.left + off_set_right, rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f);
		hidden_right = count_characters(p_text_layout_, text_, rect_text_, pt_right, dpi_scale_);

		safe_release(&p_text_layout_);
	}

	const auto textbox_width = rect_text_box_.right - rect_text_box_.left;
	const auto distance_to_caret = rect_up_to_caret_.right - rect_up_to_caret_.left;
	const auto off_set_left = textbox_width - distance_to_caret;

	if (off_set_left < text_off_set_ || hidden_left == 0) {
		// Either
		// 1. caret has reached far right and text is being added
		// 2. text hasn't filled textbox
		//
		// keep caret to the rightmost but within textbox (pin to the right if end is reached,
		// pushing text to the left).
		text_off_set_ = off_set_left;
	}
	else {
		if (hidden_left >= caret_position_) {
			// caret has reached far left
			// prevent caret from being hidden by off-setting text by up to 40px
			text_off_set_ += 40.f;
			
		}
		else
			if (hidden_right == 0) {
				// keep text pinned to the right as we downsize while there's some hidden on the left
				text_off_set_ -= off_set_right;
			}
			else {
				// do nothing under these circumstances
				// * Text has overflown on either or both sides but caret is in the middle
				//   while user is either adding or removing text
			}
	}

	// this offset cannot be greater than zero or text will be indented!!!
	text_off_set_ = smallest(text_off_set_, 0.f);

	// apply offset to text rect to ensure visibility of caret
	rect_text_.left += text_off_set_;
	rect_text_.right += text_off_set_;
	
	HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
		(UINT32)text_.length(), p_text_format_, rect_text_.right - rect_text_.left,
		rect_text_.bottom - rect_text_.top, &p_text_layout_);

	if (SUCCEEDED(hr)) {
		// clip text
		auto_clip clip(render, p_render_target, rect_text_clip_, 0.f);

		// draw the text layout
		p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
			p_text_layout_, !specs_.text.empty() ?
			p_brush_ : p_brush_prompt_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	if (!is_static_ && is_enabled_ && selected_) {
		if (hit_ && pressed_) {
			reset_selection();

			caret_position_ = get_caret_position(p_text_layout_, specs_.text, rect_text_, point_, dpi_scale_);
			caret_visible_ = true;

			if (point_.x != point_on_press_.x || point_.y != point_on_press_.y) {
				// user is making a selection
				is_selecting_ = true;

				auto selection_start_ = get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_press_, dpi_scale_);
				auto selection_end_ = caret_position_;

				auto rect_selection = get_selection_rect(p_text_layout_, rect_text_, selection_start_, selection_end_);
				p_render_target->FillRectangle(rect_selection, p_brush_selected_);
			}
		}
		else
			if (!pressed_ && is_selecting_) {
				// user is done with the selection
				is_selecting_ = false;

				set_selection(
					get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_press_, dpi_scale_),
					get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_release_, dpi_scale_));
			}
	}

	// draw selection rectangle
	if (!is_static_ && is_enabled_ && is_selected_) {
		auto rect_selection = get_selection_rect(p_text_layout_, rect_text_, selection_info_.start, selection_info_.end);
		p_render_target->FillRectangle(rect_selection, p_brush_selected_);
	}

	// draw caret
	if (!is_static_ && is_enabled_ && selected_ && caret_visible_) {
		const auto caret_rect = get_caret_rect(p_text_layout_, rect_text_, caret_position_);
		p_render_target->FillRectangle(&caret_rect, p_brush_caret_);
	}

	// release the text layout
	safe_release(&p_text_layout_);

	return rect_;
}

void liblec::lecui::widgets_implementation::textbox::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

void liblec::lecui::widgets_implementation::textbox::on_selection_change(const bool& selected) {
	if (selected) {
		// start blink timer
		log("starting caret blink timer: " + name_);
		widgets::timer(fm_).add(caret_blink_timer_name_, 500,
			[&]() {
				caret_visible_ = !caret_visible_;
				fm_.update();
			});
	}
	else {
		// stop blink timer
		log("stopping caret blink timer: " + name_);
		widgets::timer(fm_).stop(caret_blink_timer_name_);

		// stop selection
		reset_selection();
	}
}

liblec::lecui::widgets::specs::textbox&
liblec::lecui::widgets_implementation::textbox::specs() { return specs_; }

void liblec::lecui::widgets_implementation::textbox::insert_character(const char& c) {
	try {
		if (is_selected_) {
			if (selection_info_.start > selection_info_.end)
				swap(selection_info_.start, selection_info_.end);

			caret_position_ = selection_info_.start;
			specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
			reset_selection();
		}

		std::string s;
		s += c;
		specs_.text.insert(caret_position_, s);
		caret_position_++;
		caret_visible_ = true;
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_backspace() {
	try {
		if (is_selected_) {
			if (selection_info_.start > selection_info_.end)
				swap(selection_info_.start, selection_info_.end);

			caret_position_ = selection_info_.start;
			specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
			reset_selection();
		}
		else {
			specs_.text.erase(caret_position_ - 1, 1);
			caret_position_--;
			caret_visible_ = true;
		}
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_delete() {
	try {
		if (is_selected_) {
			if (selection_info_.start > selection_info_.end)
				swap(selection_info_.start, selection_info_.end);

			caret_position_ = selection_info_.start;
			specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
			reset_selection();
		}
		else {
			specs_.text.erase(caret_position_, 1);
			caret_visible_ = true;
		}
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_left() {
	try {
		if (is_selected_) {
			if (selection_info_.start > selection_info_.end)
				swap(selection_info_.start, selection_info_.end);

			caret_position_ = selection_info_.start + 1;
			reset_selection();
		}

		if (caret_position_ > 0)
			caret_position_--;
		caret_visible_ = true;
	}
	catch (const std::exception& e) { log(e.what()); }
}

void liblec::lecui::widgets_implementation::textbox::key_right() {
	try {
		if (is_selected_) {
			if (selection_info_.start > selection_info_.end)
				swap(selection_info_.start, selection_info_.end);

			caret_position_ = selection_info_.end - 1;
			reset_selection();
		}

		if (caret_position_ < specs_.text.length())
			caret_position_++;
		caret_visible_ = true;
	}
	catch (const std::exception& e) { log(e.what()); }
}

UINT32
liblec::lecui::widgets_implementation::textbox::count_characters(IDWriteTextLayout* p_text_layout, const std::string& text,
	const D2D1_RECT_F& rect_text, const D2D1_POINT_2F& point,
	const float& dpi_scale) {
	BOOL is_trailing;
	BOOL is_inside;
	DWRITE_HIT_TEST_METRICS hitTestMetrics;

	// check position of click
	p_text_layout->HitTestPoint(point.x - rect_text.left, point.y - rect_text.top,
		&is_trailing, &is_inside, &hitTestMetrics);

	auto characters = hitTestMetrics.textPosition;
	if (is_trailing && (characters < text.length())) characters++;
	return characters;
}

UINT32
liblec::lecui::widgets_implementation::textbox::get_caret_position(IDWriteTextLayout* p_text_layout, const std::string& text,
	const D2D1_RECT_F& rect_text, const D2D1_POINT_2F& point,
	const float& dpi_scale) {
	auto rect_hit = rect_text;

	scale_RECT(rect_hit, dpi_scale);

	auto p_x = point.x - rect_hit.left;
	auto p_y = point.y - rect_hit.top;

	p_x /= dpi_scale;
	p_y /= dpi_scale;

	BOOL is_trailing;
	BOOL is_inside;
	DWRITE_HIT_TEST_METRICS hitTestMetrics;

	// check position of click
	p_text_layout->HitTestPoint(p_x, p_y,
		&is_trailing, &is_inside, &hitTestMetrics);

	// change caret position
	auto caret_position = hitTestMetrics.textPosition;

	if (is_trailing && (caret_position < text.length()))
		caret_position++;

	return caret_position;
}

D2D1_RECT_F
liblec::lecui::widgets_implementation::textbox::get_selection_rect(
	IDWriteTextLayout* p_text_layout, const D2D1_RECT_F& rect_text,
	const UINT32& selection_start, const UINT32& selection_end) {
	DWRITE_HIT_TEST_METRICS hit_metrics_start, hit_metrics_end;

	float p_x, p_y;
	bool isTrailingHit = false;
	p_text_layout->HitTestTextPosition(selection_start,
		isTrailingHit, &p_x, &p_y, &hit_metrics_start);

	p_text_layout->HitTestTextPosition(selection_end,
		isTrailingHit, &p_x, &p_y, &hit_metrics_end);

	return D2D1::RectF(rect_text.left + hit_metrics_start.left,
		rect_text.top, rect_text.left + hit_metrics_end.left, rect_text.bottom);
}

float liblec::lecui::widgets_implementation::textbox::get_caret_width() {
	// respect user settings
	DWORD caret_width = 1;
	SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caret_width, 0);
	return static_cast<float>(caret_width);
}

D2D1_RECT_F
liblec::lecui::widgets_implementation::textbox::get_caret_rect(IDWriteTextLayout* p_text_layout,
	const D2D1_RECT_F& rect_text, const UINT32& caret_position) {
	DWRITE_HIT_TEST_METRICS hit_metrics;
	float p_x, p_y;

	bool is_trailing_hit = false;
	p_text_layout->HitTestTextPosition(caret_position,
		is_trailing_hit, &p_x, &p_y, &hit_metrics);

	const float half_caret_width = get_caret_width() / 2.f;

	return D2D1::RectF(rect_text.left + hit_metrics.left - half_caret_width,
		rect_text.top + hit_metrics.top,
		rect_text.left + hit_metrics.left + half_caret_width,
		rect_text.top + hit_metrics.top + hit_metrics.height);
}
