/*
** tab_control_impl.cpp - tab control widget implementation
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

#include "tab_control_impl.h"
#include "../page/page_impl.h"

class liblec::lecui::containers::tab_control::tab_control::tab_control_impl {
public:
	tab_control_impl(liblec::lecui::containers::page& page,
		liblec::lecui::containers::specs::tab_control& specs,
		const std::string& name) :
		page_(page), specs_(specs), name_(name) {}
	liblec::lecui::containers::page& page_;
	liblec::lecui::containers::specs::tab_control& specs_;
	std::string name_;
};

liblec::lecui::containers::tab_control::tab_control(liblec::lecui::containers::page& page,
	const std::string& name) :
	d_(*(new tab_control_impl(page, page.d_page_.add_tab_control(name), name))) {}

liblec::lecui::containers::tab_control::~tab_control() { delete& d_; }

liblec::lecui::containers::specs::tab_control& liblec::lecui::containers::tab_control::specs() {
	return d_.specs_;
}

void liblec::lecui::containers::tab_control::select(const std::string& name) {
	auto& tab_control_ = d_.page_.d_page_.get_tab_control(d_.name_);
	tab_control_.current_tab_ = name;
}

class liblec::lecui::containers::tab::tab::tab_impl {
public:
	tab_impl(liblec::lecui::containers::tab_control& tc) :
		tc_(tc) {}
	liblec::lecui::containers::tab_control& tc_;
};

liblec::lecui::containers::tab::tab(liblec::lecui::containers::tab_control& tc) :
	d_(*(new tab_impl(tc))) {}

liblec::lecui::containers::tab::~tab() { delete& d_; }

liblec::lecui::containers::page& liblec::lecui::containers::tab::add(const std::string& name) {
	auto& tab_control_ = d_.tc_.d_.page_.d_page_.get_tab_control(d_.tc_.d_.name_);

	log("liblec::lecui::containers::tab::add");

	if (tab_control_.p_tabs_.count(name)) {
		log("library usage error liblec::lecui::containers::tab::add");

		/// to-do: find a mechanism that makes sense ...
		return tab_control_.p_tabs_.at(name);
	}

	tab_control_.p_tabs_.emplace(name, name);

	// specify directwrite factory (used internally for text rendering)
	tab_control_.p_tabs_.at(name).d_page_.directwrite_factory(d_.tc_.d_.page_.d_page_.directwrite_factory());

	const long thickness = 10;
	const long margin = 10;
	const float page_tolerance_ = 10.f;
	const float caption_bar_height_ = tab_control_.caption_bar_height();
	liblec::lecui::rect rect_client_area = tab_control_.specs().rect;

	// initialize the page's horizontal scroll bar
	{
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().resize.perc_width = 100;
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().resize.perc_y = 100;

		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.left = 0;
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.right = (rect_client_area.right - rect_client_area.left) - (margin + thickness);
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.bottom = (rect_client_area.bottom - rect_client_area.top) - static_cast<long>(caption_bar_height_ + page_tolerance_);
		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.top =
			tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

		tab_control_.p_tabs_.at(name).d_page_.h_scrollbar().specs().on_click = nullptr;
	}

	// initialize the page's vertical scroll bar
	{
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().resize.perc_height = 100;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().resize.perc_x = 100;

		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.top = 0;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.bottom = (rect_client_area.bottom - rect_client_area.top) - (margin + thickness) - static_cast<long>(caption_bar_height_);
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.right = (rect_client_area.right - rect_client_area.left) - margin;
		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.left =
			tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().rect.right - thickness;

		tab_control_.p_tabs_.at(name).d_page_.v_scrollbar().specs().on_click = nullptr;
	}

	// add an invisible rect to bound the page. This is essential for scroll bars to work appropriately when contents don't reach the page borders
	auto& rectangle = tab_control_.p_tabs_.at(name).d_page_.add_rectangle("minimal_page_border_rect");
	rectangle.color_fill.alpha = 0;

	// make it transparent
	rectangle.color_border = { 255, 0, 0, 0 };
	rectangle.color_hot = { 255, 0, 0, 0 };

	tab_control_.p_tabs_.at(name).d_page_.size({ 0, 0 });

	{
		tab_control_.p_tabs_.at(name).d_page_.width(static_cast<long>(rect_client_area.right - rect_client_area.left));
		tab_control_.p_tabs_.at(name).d_page_.height(static_cast<long>(rect_client_area.bottom - rect_client_area.top));
	}

	tab_control_.p_tabs_.at(name).d_page_.width(tab_control_.p_tabs_.at(name).d_page_.width() - static_cast<long>(2.f * page_tolerance_));
	tab_control_.p_tabs_.at(name).d_page_.height(tab_control_.p_tabs_.at(name).d_page_.height() - static_cast<long>(2.f * page_tolerance_ + caption_bar_height_));

	rectangle.rect.set(0, 0, tab_control_.p_tabs_.at(name).d_page_.width(), tab_control_.p_tabs_.at(name).d_page_.height());

	rectangle.corner_radius_x = 15.f;
	rectangle.corner_radius_y = 15.f;

	rectangle.resize.perc_width = 100;
	rectangle.resize.perc_height = 100;

	// return reference to page so caller can add widgets to it
	return tab_control_.p_tabs_.at(name);
}

liblec::lecui::widgets_implementation::tab_control::tab_control(const std::string& page,
	const std::string& name,
	IDWriteFactory * p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_border_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_tabs_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr),
	margin_(12.f),
	tab_gap_(margin_ / 1.25f),
	tab_height_(25.f),
	bar_height_(2.f),
	rect_tabs_({ 0.f, 0.f, 0.f, 0.f }),
	rect_client_area_({ 0.f, 0.f, 0.f, 0.f }),
	rect_tab_control_({ 0.f, 0.f, 0.f, 0.f }) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::tab_control::~tab_control() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::tab_control::name() { return name_; }
std::string liblec::lecui::widgets_implementation::tab_control::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::tab_control::type() {
	return lecui::widgets_implementation::widget_type::tab_control;
}

HRESULT liblec::lecui::widgets_implementation::tab_control::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = false;

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
			&p_brush_);
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
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_tabs),
			&p_brush_tabs_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
			&p_brush_selected_);
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

void liblec::lecui::widgets_implementation::tab_control::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	safe_release(&p_brush_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_tabs_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::tab_control::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	rect_tab_control_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_tab_control_.left -= x_off_set;
	rect_tab_control_.right -= x_off_set;
	rect_tab_control_.top -= y_off_set;
	rect_tab_control_.bottom -= y_off_set;

	rect_tabs_ = rect_tab_control_;
	rect_tabs_.bottom = rect_tabs_.top + tab_height_ + bar_height_;

	rect_ = rect_tabs_;	// only make the tab area respond to hit testing, even though for scroll bar at form level we need to return the entire region through rect_tab_control_

	rect_client_area_ = rect_tab_control_;
	rect_client_area_.top = rect_tabs_.bottom;

	if (!render || !visible_)
		return rect_tab_control_;

	D2D1_ROUNDED_RECT rounded_rect{ rect_client_area_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	if (specs_.is_filled)
		p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_fill_ : p_brush_disabled_);
	
	p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_border_ : p_brush_disabled_, specs_.border);

	auto measure_string = [](IDWriteFactory* p_directwrite_factory_,
		IDWriteTextFormat* p_text_format_, IDWriteTextLayout* p_text_layout_,
		const std::string& text_, float max_width, float max_height) {
		D2D1_SIZE_F minSize = { 0.f, 0.f };

		// create a text layout
		HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
			(UINT32)text_.length(), p_text_format_, max_width, max_height, &p_text_layout_);

		// measure minimum width required
		if (SUCCEEDED(hr)) {
			DWRITE_TEXT_METRICS textMetrics;
			p_text_layout_->GetMetrics(&textMetrics);
			minSize.width = textMetrics.width;
			minSize.height = textMetrics.height;
		}

		// release the text layout
		safe_release(&p_text_layout_);

		return minSize;
	};

	// draw tabs rectangle
	rounded_rect = { rect_tabs_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_tabs_);
	p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_border_ : p_brush_disabled_, specs_.border);

	// draw the tab text
	D2D1_RECT_F rect_current_tab_ = rect_tabs_;
	rect_current_tab_.bottom -= bar_height_;
	rect_current_tab_.right = rect_current_tab_.left;
	for (const auto& page : p_tabs_) {
		// calculate tab rect
		rect_current_tab_.left = rect_current_tab_.right;
		rect_current_tab_.right = rect_client_area_.right;

		// calculate minimum width
		const auto min_size = measure_string(p_directwrite_factory_, p_text_format_,
			p_text_layout_, page.first, rect_current_tab_.right - rect_current_tab_.left,
			rect_current_tab_.bottom - rect_current_tab_.top);
		
		rect_current_tab_.right = rect_current_tab_.left + min_size.width + 2.f * margin_;

		D2D1_RECT_F rect_text_ = rect_current_tab_;

		if (rect_text_.right > rect_tab_control_.right)
			break;

		// capture current tab
		p_tab_rects_[page.first];	// add if it doesn't yet exist

		{
			D2D1_RECT_F rect = rect_current_tab_;

			// for aesthetics and to allow switching when moving over from one tab to the other
			rect.left += tab_gap_;
			rect.right -= tab_gap_;

			p_tab_rects_.at(page.first) = rect;
		}

		if (!is_static_ && is_enabled_ && hit_) {
			D2D1_RECT_F rect = rect_current_tab_;

			rect.left += tab_gap_;
			rect.right -= tab_gap_;

			scale_RECT(rect, dpi_scale_);

			if (point_.x >= rect.left && point_.x <= rect.right &&
				point_.y >= rect.top && point_.y <= rect.bottom) {
				if (!is_static_ && is_enabled_ && hit_) {
					if (pressed_)
						current_tab_ = page.first;	// it's a tab, don't wait for a click!

					// move text up a little (visual effect)
					rect_text_.bottom -= 5.f;
				}
			}
		}

		// create a text layout
		HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(page.first).c_str(),
			(UINT32)page.first.length(), p_text_format_, rect_text_.right - rect_text_.left,
			rect_text_.bottom - rect_text_.top, &p_text_layout_);

		if (SUCCEEDED(hr)) {
			// draw the text layout
			p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
				p_text_layout_, is_enabled_ ? p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		}

		// release the text layout
		safe_release(&p_text_layout_);
	}

	// draw the selected tab bar
	for (const auto& it : p_tab_rects_) {
		if ((it.second.right + tab_gap_) > rect_tab_control_.right)
			break;

		if (it.first == current_tab_) {
			D2D1_RECT_F rect_bar_ = it.second;
			rect_bar_.top = rect_bar_.bottom;
			rect_bar_.bottom = rect_bar_.bottom += bar_height_;

			p_render_target->FillRectangle(&rect_bar_, p_brush_selected_);
			break;
		}
	}

	return rect_tab_control_;
}

void liblec::lecui::widgets_implementation::tab_control::on_click() {
	for (auto& it : p_tab_rects_) {
		D2D1_RECT_F rect = it.second;
		scale_RECT(rect, dpi_scale_);

		if (point_.x >= rect.left && point_.x <= rect.right &&
			point_.y >= rect.top && point_.y <= rect.bottom)
			current_tab_ = it.first;
	}
}

liblec::lecui::containers::specs::tab_control&
liblec::lecui::widgets_implementation::tab_control::specs() { return specs_; }

const D2D1_RECT_F& liblec::lecui::widgets_implementation::tab_control::client_area() {
	return rect_client_area_;
}

const D2D1_RECT_F& liblec::lecui::widgets_implementation::tab_control::tab_control_area() {
	return rect_tab_control_;
}

float liblec::lecui::widgets_implementation::tab_control::caption_bar_height() {
	return (tab_height_ + bar_height_);
}

bool liblec::lecui::widgets_implementation::tab_control::contains() {
	for (const auto& it : p_tab_rects_) {
		D2D1_RECT_F rect = it.second;
		scale_RECT(rect, dpi_scale_);

		if (point_.x >= rect.left && point_.x <= rect.right &&
			point_.y >= rect.top && point_.y <= rect.bottom)
			return true;
	}

	return false;
}
