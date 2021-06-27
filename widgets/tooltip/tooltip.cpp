//
// tooltip.cpp - tooltip implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../../form_common.h"
#include "../tooltip.h"
#include "../label.h"

bool liblec::lecui::widgets::tooltip_form::on_initialize(std::string& error) {
	// make top level form
	ctrls_.make_top_most(true);

	// measure items

	// compute maximum based on working area size and parent form size
	auto rect_wa = dim_.working_area();

	// prevent from overlapping the right border of the parent form
	dimensions dim_parent_(parent_);
	const auto parent_right = dim_parent_.get_position().x + dim_parent_.get_size().width;
	rect_wa.right = smallest(rect_wa.right, parent_right);

	// add padding
	rect_wa.left += margin_;
	rect_wa.right -= margin_;
	rect_wa.top += margin_;
	rect_wa.bottom -= margin_;

	// set maximum size to the computed area
	max_size_.width = smallest(max_size_.width, rect_wa.width());
	max_size_.height = smallest(max_size_.height, rect_wa.height());

	const auto cursor_rect = dim_.cursor_rect();

	// impose maximums
	const lecui::rect max_rect = { margin_, max_size_.width, margin_, max_size_.height };
	float bottom_ = 0.f;
	float right_ = 0.f;

	// compute rect
	rect_ = max_rect;

	rect_ = dim_.measure_label(tooltip_text_, font_, font_size_, false, false, rect_);

	bottom_ = rect_.bottom;
	right_ = rect_.right;

	auto width = right_ + margin_;
	auto height = smallest(max_size_.height, bottom_ + margin_);

	// impose minimums
	width = largest(width, min_size_.width);
	height = largest(height, min_size_.height);

	auto set_size = [&]() {
		dim_.set_size({ width, height });
		dim_.set_minimum({ width, height });
	};

	// default to top left corner of cursor rect
	lecui::point top_left = { cursor_rect.right, cursor_rect.bottom };

	// pin to the bottom if there is enough space, or if the area beneath is
	// larger than that above, else pin above

	set_size();

	// prevent from getting hidden at the bottom
	if ((top_left.y + height) > rect_wa.bottom)
		top_left.y -= (top_left.y + height - rect_wa.bottom);

	// prevent from getting hidden on the right
	if ((top_left.x + width) > rect_wa.right)
		top_left.x -= (top_left.x + width - rect_wa.right);

	// move this form to the cursor position
	move(top_left);

	return true;
}

void liblec::lecui::widgets::tooltip_form::on_start() {
	// set timer to automatically close the tooltip
	timer_man_.add("lecui::tooltip::autoclose", lifetime_, [&]() { close(); });
}

bool liblec::lecui::widgets::tooltip_form::on_layout(std::string& error) {
	// add home page
	auto& home_page = page_man_.add(al_page_home_);

	// label
	lecui::widgets::label_builder label(home_page, "");
	label()
		.text(tooltip_text_).font(font_).font_size(font_size_)
		.rect(rect_).center_v(true);

	page_man_.show(al_page_home_);
	return true;
}

liblec::lecui::widgets::tooltip_form::tooltip_form(form& parent, const std::string& text, unsigned long lifetime) :
	form(tooltip_form_caption(), parent),
	tooltip_text_(text),
	lifetime_(lifetime),
	parent_(parent) {}

liblec::lecui::widgets::tooltip_form::~tooltip_form() {}
