//
// rectangle_impl.cpp - rectangle_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "rectangle_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets::rectangle_impl::page_rect_alias() {
			return std::string("lecui::minimal_page_border_rect");
		}

		widgets::rectangle_impl::rectangle_impl(containers::page& page,
			const std::string& alias,
			widgets::h_scrollbar_impl& h_scrollbar,
			widgets::v_scrollbar_impl& v_scrollbar) :
			widget_impl(page, alias),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_border_hot_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			h_scrollbar_(h_scrollbar),
			v_scrollbar_(v_scrollbar) {}

		widgets::rectangle_impl::~rectangle_impl() { discard_resources(); }

		widgets::widget_type
			widgets::rectangle_impl::type() {
			return lecui::widgets::widget_type::rectangle;
		}

		HRESULT widgets::rectangle_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border()),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border_hot()),
					&p_brush_border_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot()),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled()),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected()),
					&p_brush_selected_);

			resources_created_ = true;
			return hr;
		}

		void widgets::rectangle_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_border_hot_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
		}

		D2D1_RECT_F&
			widgets::rectangle_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect(), specs_.on_resize(), change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			const D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x(), specs_.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ?
				(hit_ ? p_brush_hot_ : p_brush_fill_) : p_brush_disabled_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				(hit_ ? p_brush_border_hot_ : p_brush_border_) : p_brush_disabled_, specs_.border());

			if (!is_static_ && is_enabled_) {
				if (pressed_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, 1.f);

				if (selected_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ?
						1.75f : 1.f);
			}

			return rect_;
		}

		bool widgets::rectangle_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, get_dpi_scale());

			if (alias_ == page_rect_alias()) {
				// this is a special rectangle used to manage pages.
				// scrollbar movements move everything, including the minimal page border rect.
				// keep the page virtual hit area over the actual page by ignoring scrollbar movements.

				rect.left += h_scrollbar_.x_off_set_;
				rect.right += h_scrollbar_.x_off_set_;
				rect.top += v_scrollbar_.y_off_set_;
				rect.bottom += v_scrollbar_.y_off_set_;
			}

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return true;
			else
				return false;
		}

		D2D1_POINT_2F widgets::rectangle_impl::get_scrollbar_offset() {
			return { h_scrollbar_.x_off_set_, v_scrollbar_.y_off_set_ };
		}

		widgets::rectangle::rectangle_specs&
			widgets::rectangle_impl::specs() { return specs_; }

		widgets::rectangle::rectangle_specs&
			widgets::rectangle_impl::operator()() { return specs(); }
	}
}
