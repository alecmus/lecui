/*
** rectangle_impl.cpp - rectangle_impl implementation
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

#include "rectangle_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets_impl::rectangle::page_rect_alias() {
			return std::string("lecui::minimal_page_border_rect");
		}

		widgets_impl::rectangle::rectangle(const std::string& page_alias,
			const std::string& alias,
			widgets_impl::h_scrollbar& h_scrollbar,
			widgets_impl::v_scrollbar& v_scrollbar) :
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			h_scrollbar_(h_scrollbar),
			v_scrollbar_(v_scrollbar) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::rectangle::~rectangle() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::rectangle::type() {
			return lecui::widgets_impl::widget_type::rectangle;
		}

		HRESULT widgets_impl::rectangle::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

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

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::rectangle::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
		}

		D2D1_RECT_F&
			widgets_impl::rectangle::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			const D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_fill_ : p_brush_disabled_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			if (!is_static_ && is_enabled_) {
				if (hit_ || pressed_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ?
						1.75f : 1.f);

				if (selected_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ?
						1.75f : 1.f);
			}

			return rect_;
		}

		void widgets_impl::rectangle::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		bool widgets_impl::rectangle::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, dpi_scale_);

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

		widgets::rectangle::rectangle_specs&
			widgets_impl::rectangle::specs() { return specs_; }

		widgets::rectangle::rectangle_specs&
			widgets_impl::rectangle::operator()() { return specs(); }
	}
}
