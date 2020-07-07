/*
** widget_impl.cpp - widget_impl implementation
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

#include "widget_impl.h"
#include "../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::widget_impl::widget_impl(containers::page& page, const std::string& alias) :
			page_(page),
			alias_(alias),
			is_static_(true),
			hit_(false),
			pressed_(false),
			rect_({ .0f, .0f, .0f, .0f }),
			visible_(true),
			is_enabled_(true),
			point_({ 0.f, 0.f }),
			selected_(false),
			point_on_press_({ 0.f,0.f }),
			point_on_release_({ 0.f,0.f }),
			resources_created_(false),
			h_cursor_(nullptr) {}

		widgets_impl::widget_impl::~widget_impl() {}

		const std::string& widgets_impl::widget_impl::alias() { return alias_; }

		bool widgets_impl::widget_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, page_.d_page_.get_dpi_scale());

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return contains();
			else
				return false;
		}

		bool widgets_impl::widget_impl::hit(const bool& hit) {
			if (is_static_ || hit == hit_)
				return false;	// Don't refresh ui. Either widget is static or hit status hasn't changed.

			hit_ = hit;

			if (hit_) {
				if (generic_specs().events().mouse_enter)
					generic_specs().events().mouse_enter();
			}
			else {
				if (generic_specs().events().mouse_leave)
					generic_specs().events().mouse_leave();
			}

			return true;
		}

		void widgets_impl::widget_impl::press(const bool& pressed) {
			bool change = pressed_ != pressed;

			pressed_ = pressed;

			if (change) {
				if (pressed_)
					point_on_press_ = point_;
				else
					point_on_release_ = point_;
			}
		}

		bool widgets_impl::widget_impl::pressed() { return pressed_; }
		bool widgets_impl::widget_impl::is_static() { return is_static_; }
		void widgets_impl::widget_impl::show(const bool& show) { visible_ = show; }
		bool widgets_impl::widget_impl::visible() { return visible_; }
		void widgets_impl::widget_impl::enable(const bool& enable) {
			is_enabled_ = enable;
		}
		bool widgets_impl::widget_impl::enabled() { return is_enabled_; }
		const D2D1_RECT_F& widgets_impl::widget_impl::get_rect() { return rect_; }
		bool widgets_impl::widget_impl::selected() { return selected_; }
		bool widgets_impl::widget_impl::hit() { return hit_; }
		HCURSOR widgets_impl::widget_impl::cursor() { return h_cursor_; }
		float widgets_impl::widget_impl::get_dpi_scale() { return page_.d_page_.get_dpi_scale(); }
		form& widgets_impl::widget_impl::get_form() { return page_.d_page_.get_form(); }
		bool widgets_impl::widget_impl::on_mousewheel(float units) { return false; }
		bool widgets_impl::widget_impl::on_keydown(WPARAM wParam) { return false; }
		void widgets_impl::widget_impl::on_selection_change(const bool& selected) {}
		bool widgets_impl::widget_impl::contains() { return true; }

		void widgets_impl::widget_impl::select(const bool& selected) {
			bool change = selected_ != selected;
			selected_ = selected;

			if (change)
				on_selection_change(selected_);
		}

		D2D1_RECT_F
			widgets_impl::widget_impl::position(const rect& rect,
				const widgets::specs::resize_params& on_resize,
				const float& change_in_width, const float& change_in_height) {
			auto x_change = change_in_width * on_resize.perc_x / 100.f;
			auto y_change = change_in_height * on_resize.perc_y / 100.f;
			auto width_change =
				change_in_width * on_resize.perc_width / 100.f;
			auto height_change =
				change_in_height * on_resize.perc_height / 100.f;

			auto left = rect.left + x_change;
			auto top = rect.top + y_change;
			auto right = rect.right + x_change + width_change;
			auto bottom = rect.bottom + y_change + height_change;

			// apply limits on width and height

			// width
			if ((right - left) < on_resize.min_width)
				right = left + on_resize.min_width;

			if (on_resize.max_width > on_resize.min_width)
				if ((right - left) > on_resize.max_width)
					right = left + on_resize.max_width;

			// height
			if ((bottom - top) < on_resize.min_height)
				bottom = top + on_resize.min_height;

			if (on_resize.max_height > on_resize.min_height)
				if ((bottom - top) > on_resize.max_height)
					bottom = top + on_resize.max_height;

			const auto width = right - left;
			const auto height = bottom - top;

			// apply limits on position

			// x

			if (left < on_resize.min_x) {
				left = on_resize.min_x;
				right = left + width;
			}

			if (on_resize.max_x > on_resize.min_x) {
				if (left > on_resize.max_x) {
					left = on_resize.max_x;
					right = left + width;
				}
			}

			// y

			if (top < on_resize.min_y) {
				top = on_resize.min_y;
				bottom = top + height;
			}

			if (on_resize.max_y > on_resize.min_y) {
				if (top > on_resize.max_y) {
					top = on_resize.max_y;
					bottom = top + height;
				}
			}

			return D2D1::RectF(left, top, right, bottom);
		}
	}
}
