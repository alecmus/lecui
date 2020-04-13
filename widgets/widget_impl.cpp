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

namespace liblec {
	namespace lecui {
		widgets_impl::widget::widget() :
			page_alias_(std::string()),
			alias_(std::string()),
			dpi_scale_(1.f),
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
			draw_menu_(false),
			resources_created_(false),
			h_cursor_(nullptr) {
			// to-do: this seems really wasteful. Find a way of optimizing this and not repeating it for
			// every single widget, without making it static. Perhaps implement at page-level.
			HDC hdc_screen = GetDC(NULL);
			dpi_scale_ = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
			ReleaseDC(NULL, hdc_screen);
		}

		widgets_impl::widget::~widget() {}

		const std::string& widgets_impl::widget::page_alias() { return page_alias_; }
		const std::string& widgets_impl::widget::alias() { return alias_; }

		bool widgets_impl::widget::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, dpi_scale_);

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return contains();
			else
				return false;
		}

		bool widgets_impl::widget::hit(const bool& hit) {
			if (is_static_ || hit == hit_)
				return false;	// Don't refresh ui. Either widget is static or hit status hasn't changed.

			hit_ = hit;
			return true;
		}

		void widgets_impl::widget::press(const bool& pressed) {
			bool change = pressed_ != pressed;

			pressed_ = pressed;

			if (change) {
				if (pressed_)
					point_on_press_ = point_;
				else
					point_on_release_ = point_;
			}
		}

		bool widgets_impl::widget::pressed() { return pressed_; }
		bool widgets_impl::widget::is_static() { return is_static_; }
		void widgets_impl::widget::show(const bool& show) { visible_ = show; }
		bool widgets_impl::widget::visible() { return visible_; }
		void widgets_impl::widget::enable(const bool& enable) {
			is_enabled_ = enable;
		}
		bool widgets_impl::widget::enabled() { return is_enabled_; }
		const D2D1_RECT_F& widgets_impl::widget::get_rect() { return rect_; }
		bool widgets_impl::widget::selected() { return selected_; }
		bool widgets_impl::widget::menu_visible() { return draw_menu_; }
		bool widgets_impl::widget::hit() { return hit_; }
		HCURSOR widgets_impl::widget::cursor() { return h_cursor_; }
		bool widgets_impl::widget::on_menu(
			ID2D1HwndRenderTarget* p_render_target, const D2D1_RECT_F& client_area) {
			return false;
		}
		void widgets_impl::widget::reset_menu() { draw_menu_ = false; }
		bool widgets_impl::widget::on_mousewheel(float units) { return false; }
		bool widgets_impl::widget::on_keydown(WPARAM wParam) { return false; }
		void widgets_impl::widget::on_selection_change(const bool& selected) {}
		bool widgets_impl::widget::contains() { return true; }

		void widgets_impl::widget::select(const bool& selected) {
			bool change = selected_ != selected;
			selected_ = selected;

			if (change) {
				if (!selected_) reset_menu();
				on_selection_change(selected_);
			}
		}

		D2D1_RECT_F
			widgets_impl::widget::position(const rect& rect,
				const widgets::specs::on_resize& resize,
				const float& change_in_width, const float& change_in_height) {
			auto x_change = change_in_width * resize.perc_x / 100.f;
			auto y_change = change_in_height * resize.perc_y / 100.f;
			auto width_change =
				change_in_width * resize.perc_width / 100.f;
			auto height_change =
				change_in_height * resize.perc_height / 100.f;

			auto left = rect.left + x_change;
			auto top = rect.top + y_change;
			auto right = rect.right + x_change + width_change;
			auto bottom = rect.bottom + y_change + height_change;

			// apply limits on width and height

			// width
			if ((right - left) < resize.min_width)
				right = left + resize.min_width;

			if (resize.max_width > resize.min_width)
				if ((right - left) > resize.max_width)
					right = left + resize.max_width;

			// height
			if ((bottom - top) < resize.min_height)
				bottom = top + resize.min_height;

			if (resize.max_height > resize.min_height)
				if ((bottom - top) > resize.max_height)
					bottom = top + resize.max_height;

			const auto width = right - left;
			const auto height = bottom - top;

			// apply limits on position

			// x

			if (left < resize.min_x) {
				left = resize.min_x;
				right = left + width;
			}

			if (resize.max_x > resize.min_x) {
				if (left > resize.max_x) {
					left = resize.max_x;
					right = left + width;
				}
			}

			// y

			if (top < resize.min_y) {
				top = resize.min_y;
				bottom = top + height;
			}

			if (resize.max_y > resize.min_y) {
				if (top > resize.max_y) {
					top = resize.max_y;
					bottom = top + height;
				}
			}

			return D2D1::RectF(left, top, right, bottom);
		}
	}
}
