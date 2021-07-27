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
	_ctrls.make_top_most(true);

	// measure items

	// compute maximum based on working area size and parent form size
	auto rect_wa = _dim.working_area();

	// prevent from overlapping the right border of the parent form
	dimensions _dim_parent(_parent);
	const auto parent_right = _dim_parent.get_position().get_x() + _dim_parent.get_size().get_width();
	rect_wa.right(smallest(rect_wa.right(), parent_right));

	// add padding
	rect_wa.left() += _margin;
	rect_wa.right() -= _margin;
	rect_wa.top() += _margin;
	rect_wa.bottom() -= _margin;

	// set maximum size to the computed area
	_max_size.width(smallest(_max_size.get_width(), rect_wa.width()));
	_max_size.height(smallest(_max_size.get_height(), rect_wa.height()));

	const auto cursor_rect = _dim.cursor_rect();

	// impose maximums
	const lecui::rect max_rect = rect()
		.left(_margin)
		.right(_max_size.get_width())
		.top(_margin)
		.bottom(_max_size.get_height());

	float _bottom = 0.f;
	float _right = 0.f;

	// compute rect
	_rect = max_rect;

	_rect = _dim.measure_label(_tooltip_text, _font, _font_size, false, false, _rect);

	_bottom = _rect.bottom();
	_right = _rect.right();

	auto width = _right + _margin;
	auto height = smallest(_max_size.get_height(), _bottom + _margin);

	// impose minimums
	width = largest(width, _min_size.get_width());
	height = largest(height, _min_size.get_height());

	auto set_size = [&]() {
		_dim.set_size({ width, height });
		_dim.set_minimum({ width, height });
	};

	// default to top left corner of cursor rect
	lecui::point top_left = { cursor_rect.get_right(), cursor_rect.get_bottom() };

	// pin to the bottom if there is enough space, or if the area beneath is
	// larger than that above, else pin above

	set_size();

	// prevent from getting hidden at the bottom
	if ((top_left.get_y() + height) > rect_wa.bottom())
		top_left.y() -= (top_left.get_y() + height - rect_wa.bottom());

	// prevent from getting hidden on the right
	if ((top_left.get_x() + width) > rect_wa.right())
		top_left.x() -= (top_left.get_x() + width - rect_wa.right());

	// move this form to the cursor position
	move(top_left);

	return true;
}

void liblec::lecui::widgets::tooltip_form::on_start() {
	// set timer to automatically close the tooltip
	_timer_man.add("lecui::tooltip::autoclose", _lifetime, [&]() { close(); });
}

bool liblec::lecui::widgets::tooltip_form::on_layout(std::string& error) {
	// add home page
	auto& home_page = _page_man.add(_al_page_home);

	// label
	lecui::widgets::label_builder label(home_page, "");
	label()
		.text(_tooltip_text).font(_font).font_size(_font_size)
		.rect(_rect).center_v(true);

	_page_man.show(_al_page_home);
	return true;
}

liblec::lecui::widgets::tooltip_form::tooltip_form(form& parent, const std::string& text, unsigned long lifetime) :
	form(tooltip_form_caption(), parent),
	_tooltip_text(text),
	_lifetime(lifetime),
	_parent(parent) {}

liblec::lecui::widgets::tooltip_form::~tooltip_form() {}
