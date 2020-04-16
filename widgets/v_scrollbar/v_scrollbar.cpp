/*
** v_scrollbar.h - vertical scroll bar widget implementation
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

#include "v_scrollbar.h"

namespace liblec {
	namespace lecui {
		widgets_impl::v_scrollbar::v_scrollbar(const std::string& page_alias) :
			p_brush_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_hot_pressed_(nullptr),
			p_brush_background_(nullptr),
			rectA_({ 0.f, 0.f, 0.f, 0.f }),
			rectB_({ 0.f, 0.f, 0.f, 0.f }),
			rectC_({ 0.f, 0.f, 0.f, 0.f }),
			rectD_({ 0.f, 0.f, 0.f, 0.f }),
			y_displacement_previous_(0.f),
			y_displacement_(0.f),
			y_off_set_(0.f),
			max_displacement_top_(0.f),
			max_displacement_bottom_(0.f),
			force_translate_(false) {
			page_alias_ = page_alias;
			alias_ = "v_scrollbar";
		}

		widgets_impl::v_scrollbar::~v_scrollbar() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::v_scrollbar::type() {
			return lecui::widgets_impl::widget_type::v_scrollbar;
		}

		HRESULT widgets_impl::v_scrollbar::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			is_static_ = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_scrollbar_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot_pressed),
					&p_brush_hot_pressed_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_background),
					&p_brush_background_);

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::v_scrollbar::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_hot_pressed_);
			safe_release(&p_brush_background_);
		}

		D2D1_RECT_F&
			widgets_impl::v_scrollbar::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			const int precision = 3;	// to prevent false-positives (4 is enough, 3 is a failsafe)
			auto equal = [&](const D2D1_RECT_F& rect_1, const D2D1_RECT_F& rect_2) {
				if (roundoff::tof(rect_1.left, precision) == roundoff::tof(rect_2.left, precision) &&
					roundoff::tof(rect_1.top, precision) == roundoff::tof(rect_2.top, precision) &&
					roundoff::tof(rect_1.right, precision) == roundoff::tof(rect_2.right, precision) &&
					roundoff::tof(rect_1.bottom, precision) == roundoff::tof(rect_2.bottom, precision))
					return true;
				else
					return false;
			};

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			rectC_ = rect_;

			rectD_ = { 0.f, 0.f, 0.f, 0.f };
			position_v_scrollbar(rectA_, rectB_, rectC_, rectD_);

			// adjust y_displacement_ to match what the user is seeing. Ensures consistency.
			auto const ui_displacement = dpi_scale_ * (rectD_.top - rectC_.top);

			if (ui_displacement != y_displacement_) {
				y_displacement_ = ui_displacement;
				force_translate_ = true;
			}

			if (!render)
				return rect_;

			if (!equal(rectC_, rectD_) &&
				!(roundoff::tof((rectD_.bottom - rectD_.top), precision) >=
					roundoff::tof((rectC_.bottom - rectC_.top), precision))) {
				auto corner_radius = smallest((rectD_.bottom - rectD_.top) / 3.f,
					(rectD_.right - rectD_.left) / 3.f);

				// scroll area
				D2D1_ROUNDED_RECT rounded_rectC{ rectC_, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(&rounded_rectC, p_brush_background_);

				// scroll bar
				auto rect_scroll_bar = rectD_;
				const float scroll_bar_margin = 2.f;
				rect_scroll_bar.left += scroll_bar_margin;
				rect_scroll_bar.top += scroll_bar_margin;
				rect_scroll_bar.right -= scroll_bar_margin;
				rect_scroll_bar.bottom -= scroll_bar_margin;
				corner_radius = smallest((rect_scroll_bar.bottom - rect_scroll_bar.top) / 3.f,
					(rect_scroll_bar.right - rect_scroll_bar.left) / 3.f);
				D2D1_ROUNDED_RECT rounded_rectD{ rect_scroll_bar, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(&rounded_rectD,
					pressed_ ? p_brush_hot_pressed_ : (hit_ ? p_brush_hot_ : p_brush_));
				p_render_target->DrawRoundedRectangle(&rounded_rectD, p_brush_border_);

				visible_ = true;
			}
			else
				visible_ = false;

			return rect_;
		}

		void widgets_impl::v_scrollbar::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::v_scrollbar_specs&
			widgets_impl::v_scrollbar::specs() { return specs_; }

		widgets::v_scrollbar_specs&
			widgets_impl::v_scrollbar::operator()() { return specs(); }

		void widgets_impl::v_scrollbar::max_displacement(float& top,
			float& bottom) {
			bottom = rectC_.bottom - rectD_.bottom;
			top = rectC_.top - rectD_.top;
		}

		bool widgets_impl::v_scrollbar::translate_y_displacement(
			const float& displacement, float& y_displacement_translated, bool force) {
			if (pressed_ || force) {
				// calculate the scale factor for amplifying the movement of the scroll bar
				const float height_A = rectA_.bottom - rectA_.top;
				const float height_C = rectC_.bottom - rectC_.top;
				const float scale_factor = height_C != 0.f ? height_A / height_C : 1.f;

				y_displacement_translated = displacement * scale_factor;
				return true;
			}

			return false;
		}

		void widgets_impl::v_scrollbar::setup(const D2D1_RECT_F& rectA,
			const D2D1_RECT_F& rectB) {
			rectA_ = rectA;
			rectB_ = rectB;
		}
	}
}
