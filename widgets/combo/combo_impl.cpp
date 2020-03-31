/*
** combo_impl.cpp - combo widget implementation
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

#include "combo_impl.h"
#include "../../containers/page/page_impl.h"

class liblec::lecui::widgets::combo::combo::combo_impl {
public:
	combo_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::combo::combo(liblec::lecui::containers::page& page) :
	d_(*(new combo_impl(page))) {}

liblec::lecui::widgets::combo::~combo() { delete& d_; }

liblec::lecui::widgets::specs::combo& liblec::lecui::widgets::combo::add(const std::string& name) {
	return d_.page_.d_page_.add_combo(name);
}

liblec::lecui::widgets_implementation::combo::combo(const std::string& page,
	const std::string& name,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_fill_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_brush_border_(nullptr),
	p_brush_dropdown_hot_(nullptr),
	p_brush_menu_(nullptr),
	p_brush_menu_hot_(nullptr),
	p_brush_menu_selected_(nullptr),
	p_text_format_(nullptr),
	p_directwrite_factory_(p_directwrite_factory),
	rect_dropdown_({ 0.f, 0.f, 0.f, 0.f }),
	rect_text_({ 0.f, 0.f, 0.f, 0.f }),
	p_text_layout_(nullptr),
	scrollbar_thickness_(10),

	color_scrollbar_({ 20, 80, 140, 150 }),
	color_scrollbar_hot_({ 20, 80, 140, 200 }),
	color_scrollbar_hot_pressed_({ 20, 80, 140, 255 }),
	color_scrollbar_background_({ 20, 80, 140, 50 }),

	p_brush_scrollbar_(nullptr),
	p_brush_scrollbar_hot_(nullptr),
	p_brush_scrollbar_hot_pressed_(nullptr),
	p_brush_scrollbar_background_(nullptr),

	scrollbar_hit_(false),
	scrollbar_pressed_(false),
	move_by_(0),
	displacement_(0.f),
	displacement_previous_(0.f) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::combo::~combo() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::combo::name() { return name_; }
std::string liblec::lecui::widgets_implementation::combo::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::combo::type() {
	return lecui::widgets_implementation::widget_type::combo;
}

HRESULT liblec::lecui::widgets_implementation::combo::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = (specs_.on_click == nullptr && specs_.on_selection == nullptr);

	HRESULT hr = S_OK;

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
			&p_brush_fill_);
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
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
			&p_brush_border_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown_hot),
			&p_brush_dropdown_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_menu),
			&p_brush_menu_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_menu_hot),
			&p_brush_menu_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_menu_selected),
			&p_brush_menu_selected_);

	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_),
			&p_brush_scrollbar_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_hot_),
			&p_brush_scrollbar_hot_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_hot_pressed_),
			&p_brush_scrollbar_hot_pressed_);
	if (SUCCEEDED(hr))
		hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_background_),
			&p_brush_scrollbar_background_);

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

void liblec::lecui::widgets_implementation::combo::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_);
	safe_release(&p_brush_fill_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_brush_border_);
	safe_release(&p_brush_dropdown_hot_);
	safe_release(&p_brush_menu_);
	safe_release(&p_brush_menu_hot_);
	safe_release(&p_brush_menu_selected_);
	safe_release(&p_text_format_);

	safe_release(&p_brush_scrollbar_);
	safe_release(&p_brush_scrollbar_hot_);
	safe_release(&p_brush_scrollbar_hot_pressed_);
	safe_release(&p_brush_scrollbar_background_);

}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::combo::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	if (!resources_created_)
		create_resources(p_render_target);

	rect_combo_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_combo_.left -= x_off_set;
	rect_combo_.right -= x_off_set;
	rect_combo_.top -= y_off_set;
	rect_combo_.bottom -= y_off_set;

	specs_.text.clear();

	for (auto& item : specs_.items) {
		if (item == specs_.selected) {
			specs_.text = item;
			break;
		}
	}

	D2D1_ROUNDED_RECT rounded_rect{ rect_combo_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	// draw background
	if (render && visible_)
		p_render_target->FillRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
			p_brush_fill_);

	// draw dropdown rect
	rect_dropdown_ = rect_combo_;

	// width 80% of height
	rect_dropdown_.left = rect_dropdown_.right - .8f * (rect_dropdown_.bottom - rect_dropdown_.top);

	D2D1_ROUNDED_RECT dropdown_rounded_rect{rect_dropdown_,
		specs_.corner_radius_x, specs_.corner_radius_y };

	// draw dropdown rectangle
	if (render && visible_ && hit_)
		p_render_target->FillRoundedRectangle(&dropdown_rounded_rect, !is_enabled_ ? p_brush_disabled_ :
			p_brush_border_);

	// define dropdown confines
	D2D1_POINT_2F top_left, top_right, bottom;

	// width 40%
	top_left.x = rect_dropdown_.left + .4f * (rect_dropdown_.right - rect_dropdown_.left) / 2.f;
	top_right.x = rect_dropdown_.right - .4f * (rect_dropdown_.right - rect_dropdown_.left) / 2.f;
	
	// height 25%
	top_left.y = rect_dropdown_.top + .75f * (rect_dropdown_.bottom - rect_dropdown_.top) / 2.f;
	top_right.y = top_left.y;

	bottom.x = (top_left.x + top_right.x) / 2.f;
	bottom.y = rect_dropdown_.bottom - .75f * (rect_dropdown_.bottom - rect_dropdown_.top) / 2.f;

	if (render && visible_) {
		// draw dropdown
		p_render_target->DrawLine(top_left, bottom, hit_ ? p_brush_dropdown_hot_ : p_brush_border_);
		p_render_target->DrawLine(top_right, bottom, hit_ ? p_brush_dropdown_hot_ : p_brush_border_);

		// draw combo border
		p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
			p_brush_border_);
	}

	rect_text_ = rect_combo_;
	rect_text_.left += ((rect_combo_.bottom - rect_combo_.top) / 4.f);
	rect_text_.right = rect_dropdown_.left - ((rect_combo_.bottom - rect_combo_.top) / 4.f);

	// create a text layout
	HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(specs_.text).c_str(),
		(UINT32)specs_.text.length(), p_text_format_, rect_text_.right - rect_text_.left,
		rect_text_.bottom - rect_text_.top, &p_text_layout_);

	if (SUCCEEDED(hr) && render && visible_) {
		// draw the text layout
		p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
			p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	if (render && visible_ && !is_static_ && is_enabled_ && selected_)
		show_selected(p_render_target, p_brush_selected_, rect_combo_, pressed_);

	// release the text layout
	safe_release(&p_text_layout_);

	if (draw_menu_) {
		rect_ = rect_combo_;

		if (menu_below_)
			rect_.bottom = rect_menu_.bottom;
		else
			rect_.top = rect_menu_.top;

		return rect_;
	}
	else {
		rect_ = rect_combo_;
		return rect_;
	}
}

void liblec::lecui::widgets_implementation::combo::on_click() {
	// check if the dropdown has been clicked
	D2D1_RECT_F rect = rect_dropdown_;
	scale_RECT(rect, dpi_scale_);

	if (!scrollbar_pressed_ &&
		point_.x >= rect.left && point_.x <= rect.right &&
		point_.y >= rect.top && point_.y <= rect.bottom) {
		// dropdown has been clicked
		draw_menu_ = !draw_menu_;

		if (!draw_menu_)
			reset_menu();

		return;
	}

	// check if any of the items have been clicked
	bool selection_made = false;
	std::string selected_previous = specs_.selected;
	for (auto &it : hot_spots_) {
		auto rect = it.second;
		scale_RECT(rect, dpi_scale_);

		if (point_.x >= rect.left && point_.x <= rect.right &&
			point_.y >= rect.top && point_.y <= rect.bottom) {
			// change the selection
			selection_made = true;
			specs_.selected = it.first;
		}
	}

	if (selection_made) {
		reset_menu();

		if (selected_previous != specs_.selected) {
			if (specs_.on_selection)
				specs_.on_selection(specs_.selected);

			if (specs_.on_click)
				specs_.on_click();
		}
	}
}

bool liblec::lecui::widgets_implementation::combo::on_menu(ID2D1HwndRenderTarget* p_render_target,
	const D2D1_RECT_F& client_area) {
	if (draw_menu_) {
		menu_below_ = (rect_dropdown_.top - client_area.top) < (client_area.bottom - rect_dropdown_.bottom);

		// define menu space
		D2D1_RECT_F rect_menu_space = rect_combo_;

		if (menu_below_) {
			rect_menu_space.top = rect_combo_.bottom;
			rect_menu_space.bottom = client_area.bottom;
		}
		else {
			rect_menu_space.top = client_area.top;
			rect_menu_space.bottom = rect_combo_.top;
		}

		// calculate how much height we need for items
		const std::string test_string = "Abg";
		D2D1_RECT_F rect_item;
		rect_item.left = rect_text_.left;
		rect_item.right = rect_text_.right;
		
		if (menu_below_) {
			rect_item.top = rect_menu_space.top;
			rect_item.bottom = rect_item.top + (rect_text_.bottom - rect_text_.top);
		}
		else {
			rect_item.bottom = rect_menu_space.bottom;
			rect_item.top = rect_item.bottom - (rect_text_.bottom - rect_text_.top);
		}

		// create a text layout
		HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(test_string).c_str(),
			(UINT32)test_string.length(), p_text_format_, rect_item.right - rect_item.left,
			rect_item.bottom - rect_item.top, &p_text_layout_);

		DWRITE_TEXT_METRICS textMetrics;
		p_text_layout_->GetMetrics(&textMetrics);

		// release the text layout
		safe_release(&p_text_layout_);

		if (menu_below_)
			rect_item.bottom = rect_item.top + (1.1f * textMetrics.height);
		else
			rect_item.top = rect_item.bottom - (1.1f * textMetrics.height);

		const auto spacing = rect_item.bottom - rect_item.top;

		// check out how many items can fit (using truncation)
		const auto max_items = static_cast<unsigned long>((rect_menu_space.bottom - rect_menu_space.top) / spacing);

		bool scroll_bar = false;
		unsigned long starting_point = 0;
		unsigned long end_point = 0;

		if (specs_.items.size() < max_items) {
			// shrink menu space to match the items available
			if (menu_below_)
				rect_menu_space.bottom = rect_menu_space.top + (specs_.items.size() * spacing);
			else
				rect_menu_space.top = rect_menu_space.bottom - (specs_.items.size() * spacing);

			p_render_target->FillRectangle(rect_menu_space, p_brush_menu_);

			starting_point = 0;
			end_point = static_cast<unsigned long>(specs_.items.size());
		}
		else {
			scroll_bar = true;

			// factor in scroll bar
			rect_menu_space.right -= (1.2f * scrollbar_thickness_);

			// shrink menu space to match the maximum permissible items
			if (menu_below_)
				rect_menu_space.bottom = rect_menu_space.top + (max_items * spacing);
			else
				rect_menu_space.top = rect_menu_space.bottom - (max_items * spacing);

			p_render_target->FillRectangle(rect_menu_space, p_brush_menu_);

			// identify the index of the selected item
			unsigned long idx = 0;

			for (size_t i = 0; i < specs_.items.size(); i++) {
				if (specs_.items[i] == specs_.selected) {
					idx = static_cast<unsigned long>(i);
					break;
				}
			}

			// figure out starting point
			if ((specs_.items.size() - idx) > max_items)
				starting_point = idx;
			else
				starting_point = idx - (max_items - (static_cast<unsigned long>(specs_.items.size()) - idx));

			end_point = starting_point + max_items;
		}

		float y_displacement_translated = 0.f;

		if (scroll_bar) {
			// do the hard work

			// figure out scroll bar area
			auto rectC_ = rect_menu_space;
			rectC_.right = rect_combo_.right;
			rectC_.left = rectC_.right - scrollbar_thickness_;

			// figure out rect that would contain all the items
			auto rectA_ = rectC_;
			rectA_.top -= (spacing * starting_point);
			rectA_.bottom += (spacing * (specs_.items.size() - end_point));

			auto rectB_ = rectC_;

			// do the important math
			auto rectD_ = rectC_;
			position_v_scrollbar(rectA_, rectB_, rectC_, rectD_);

			{
				auto rect = rectC_;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					scrollbar_hit_ = true;
				else
					scrollbar_hit_ = false;
			}

			{
				const auto before = scrollbar_pressed_;

				auto rect = rectC_;
				scale_RECT(rect, dpi_scale_);

				if (pressed_ &&
					point_on_press_.x >= rect.left && point_on_press_.x <= rect.right &&
					point_on_press_.y >= rect.top && point_on_press_.y <= rect.bottom)
					scrollbar_pressed_ = true;
				else
					scrollbar_pressed_ = false;

				const auto after = scrollbar_pressed_;

				if (before != after && !scrollbar_pressed_)	// scroll bar released
					displacement_previous_ = displacement_;
			}

			// calculate the scale factor for amplifying the movement of the scroll bar
			const float height_A = rectA_.bottom - rectA_.top;
			const float height_C = rectC_.bottom - rectC_.top;
			const float scale_factor = height_C != 0.f ? height_A / height_C : 1.f;

			if (scrollbar_pressed_) {
				displacement_ = (point_.y - point_on_press_.y) / dpi_scale_;
				displacement_ += displacement_previous_;

				// limit displacement
				const float displacement_limit_top = rectC_.top - rectD_.top;
				const float displacement_limit_bottom = rectC_.bottom - rectD_.bottom;

				displacement_ = largest(displacement_, displacement_limit_top);
				displacement_ = smallest(displacement_, displacement_limit_bottom);

				y_displacement_translated = displacement_ * scale_factor;

				// move the scroll bar
				rectD_.top += displacement_;
				rectD_.bottom += displacement_;
			}
			else {
				// move the scroll bar
				rectD_.top += displacement_;
				rectD_.bottom += displacement_;
			}

			const auto corner_radius = smallest((rectD_.right - rectD_.left) / 3.f, (rectD_.bottom - rectD_.top) / 3.f);

			// scroll area
			D2D1_ROUNDED_RECT rounded_rectC{ rectC_, corner_radius, corner_radius };
			p_render_target->FillRoundedRectangle(&rounded_rectC, p_brush_scrollbar_background_);

			// scroll bar
			D2D1_ROUNDED_RECT rounded_rectD{ rectD_, corner_radius, corner_radius };
			p_render_target->FillRoundedRectangle(&rounded_rectD,
				scrollbar_pressed_ ? p_brush_scrollbar_hot_pressed_ : (scrollbar_hit_ ? p_brush_scrollbar_hot_ : p_brush_scrollbar_));
		}

		// do the magic ... use y_displacement_translated and translate everything
		
		// figure out how many items to move by
		if (scrollbar_pressed_) {
			float correction = y_displacement_translated > 0 ? .5f : -.5f;
			move_by_ = static_cast<unsigned long>(correction + (y_displacement_translated / spacing));
		}

		starting_point += move_by_;
		end_point += move_by_;

		hot_spots_.clear();

		// draw all items that fit (with selected item as far up as possible)
		for (unsigned long i = starting_point; i < end_point; i++) {
			rect_item.top = rect_menu_space.top + ((i - starting_point) * spacing);
			rect_item.bottom = rect_item.top + spacing;

			D2D1_RECT_F rect_hotspot = rect_item;
			rect_hotspot.top += 2;
			rect_hotspot.bottom -= 2;

			// capture hotspot
			hot_spots_[specs_.items[i]] = rect_hotspot;

			bool selected = specs_.items[i] == specs_.selected;

			if (selected) {
				auto rect = rect_item;
				rect.left = rect_menu_space.left;
				rect.right = rect_menu_space.right;
				p_render_target->FillRectangle(rect, p_brush_menu_selected_);
			}
			else {
				// check if an item has been hit
				auto rect = rect_hotspot;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom) {
					auto rect_highlight = rect_item;
					rect_highlight.left = rect_menu_space.left;
					rect_highlight.right = rect_menu_space.right;
					p_render_target->FillRectangle(rect_highlight, p_brush_menu_hot_);
				}
			}

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(
				convert_string(specs_.items[i]).c_str(),
				(UINT32)specs_.items[i].length(),
				p_text_format_,
				rect_item.right - rect_item.left,
				rect_item.bottom - rect_item.top,
				&p_text_layout_);

			if (SUCCEEDED(hr)) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_item.left, rect_item.top },
					p_text_layout_, selected ? p_brush_dropdown_hot_ : p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&p_text_layout_);
		}

		rect_menu_ = rect_menu_space;
		rect_menu_.right = rect_combo_.right;

		return true;
	}

	return false;
}

void liblec::lecui::widgets_implementation::combo::reset_menu() {
	// reset parameters
	draw_menu_ = false;
	move_by_ = 0;
	displacement_ = 0.f;
	displacement_previous_ = 0.f;
	hot_spots_.clear();
}

bool liblec::lecui::widgets_implementation::combo::hit(const bool& hit) {
	if (is_static_ || hit == hit_) {
		if (hit || pressed_)
			return true;
		else
			return false;
	}

	hit_ = hit;
	return true;
}

liblec::lecui::widgets::specs::combo&
liblec::lecui::widgets_implementation::combo::specs() { return specs_; }
