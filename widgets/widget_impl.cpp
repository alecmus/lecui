//
// widget_impl.cpp - widget_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "widget_impl.h"
#include "../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets::widget_impl::widget_impl(containers::page& page, const std::string& alias) :
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

		widgets::widget_impl::~widget_impl() {}

		const std::string& widgets::widget_impl::alias() { return alias_; }

		bool widgets::widget_impl::contains(const D2D1_POINT_2F& point) {
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

		bool widgets::widget_impl::hit(const bool& hit) {
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

		void widgets::widget_impl::press(const bool& pressed) {
			bool change = pressed_ != pressed;

			pressed_ = pressed;

			if (change) {
				if (pressed_)
					point_on_press_ = point_;
				else
					point_on_release_ = point_;
			}
		}

		bool widgets::widget_impl::pressed() { return pressed_; }
		bool widgets::widget_impl::is_static() { return is_static_; }
		void widgets::widget_impl::show(const bool& show) { visible_ = show; }
		bool widgets::widget_impl::visible() { return visible_; }
		void widgets::widget_impl::enable(const bool& enable) {
			is_enabled_ = enable;
		}
		bool widgets::widget_impl::enabled() { return is_enabled_; }
		const D2D1_RECT_F& widgets::widget_impl::get_rect() { return rect_; }
		bool widgets::widget_impl::selected() { return selected_; }
		bool widgets::widget_impl::hit() { return hit_; }
		HCURSOR widgets::widget_impl::cursor() { return h_cursor_; }
		float widgets::widget_impl::get_dpi_scale() { return page_.d_page_.get_dpi_scale(); }
		form& widgets::widget_impl::get_form() { return page_.d_page_.get_form(); }

		void widgets::widget_impl::show_tooltip() {
			// make a local copy of the tooltip text
			tooltip_text_ = generic_specs().tooltip;

			// trim leading and trailing whitespace
			trim(tooltip_text_);

			// empty tooltips not allowed
			if (tooltip_text_.empty())
				return;

			// failsafe
			if (p_tooltip_form_.get())
				return;

			lecui::timer_management timer_man_(get_form());

			// failsafe
			if (timer_man_.running("lecui::tooltip::form"))
				return;

			// set timer for delayed display of tooltip for hover effect
			timer_man_.add("lecui::tooltip::form", 500, [&]() {
				lecui::timer_management timer_man_(get_form());

				// stop the timer before we create the tooltip
				timer_man_.stop("lecui::tooltip::form");

				// create tooltip object
				p_tooltip_form_ = std::unique_ptr<tooltip_form>(new tooltip_form(get_form(), tooltip_text_, 5000));

				// display the tooltip
				std::string error;
				if (!p_tooltip_form_->show(error)) {}

				// delete tooltip object
				p_tooltip_form_.reset(nullptr);
				});
		}

		void widgets::widget_impl::hide_tooltip() {
			lecui::timer_management timer_man_(get_form());

			// stop the timer before we close the tooltip
			timer_man_.stop("lecui::tooltip::form");

			// close the tooltip
			// deletion of the tooltip object will be done in the timer handler
			if (p_tooltip_form_ && p_tooltip_form_.get())
				p_tooltip_form_->close();
		}

		void widgets::widget_impl::on_click() {
			if (generic_specs().events().click)
				generic_specs().events().click();

			if (type() != widgets::widget_type::html_editor &&
				type() != widgets::widget_type::text_field &&
				type() != widgets::widget_type::combobox) {
				if (generic_specs().events().action)
					generic_specs().events().action();
			}
		}
		void widgets::widget_impl::on_action() {
			if (generic_specs().events().action)
				generic_specs().events().action();
		}
		bool widgets::widget_impl::on_mousewheel(float units) { return false; }
		bool widgets::widget_impl::on_keydown(WPARAM wParam) { return false; }
		void widgets::widget_impl::on_selection_change(const bool& selected) {}
		bool widgets::widget_impl::contains() { return true; }

		void widgets::widget_impl::select(const bool& selected) {
			bool change = selected_ != selected;
			selected_ = selected;

			if (change)
				on_selection_change(selected_);
		}

		D2D1_RECT_F
			widgets::widget_impl::position(const rect& rect,
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
