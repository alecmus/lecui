/*
** close_button.cpp - close button widget implementation
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

#include "close_button.h"

namespace liblec {
	namespace lecui {
		widgets_impl::close_button::close_button() :
			p_brush_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr) {
			page_alias_ = "";
			alias_ = "close_button";
		}

		widgets_impl::close_button::~close_button() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::close_button::type() {
			return lecui::widgets_impl::widget_type::close_button;
		}

		HRESULT widgets_impl::close_button::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			is_static_ = false;
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::close_button::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F& widgets_impl::close_button::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			ID2D1SolidColorBrush* p_brush_current_ = is_enabled_ ? p_brush_ : p_brush_disabled_;

			if (!is_static_ && is_enabled_ && hit_)
				p_brush_current_ = p_brush_hot_;

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);

			if (!is_static_ && hit_)
				p_render_target->DrawRectangle(&rect_, p_brush_, pressed_ ? .5f : .0f);

			if (!render || !visible_)
				return rect_;

			// draw close button
			D2D1_RECT_F specs_(rect_);

			float margin_ = (rect_.right - rect_.left) * .30f;

			specs_.left += margin_;
			specs_.top += margin_;
			specs_.right -= margin_;
			specs_.bottom -= margin_;

			p_render_target->DrawLine(
				D2D1_POINT_2F{ specs_.left, specs_.top },
				D2D1_POINT_2F{ specs_.right, specs_.bottom },
				p_brush_current_, 2.f);
			p_render_target->DrawLine(
				D2D1_POINT_2F{ specs_.right, specs_.top },
				D2D1_POINT_2F{ specs_.left, specs_.bottom },
				p_brush_current_, 2.f);

			return rect_;
		}

		void widgets_impl::close_button::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::close_button_specs&
			widgets_impl::close_button::specs() { return specs_; }

		widgets::close_button_specs&
			widgets_impl::close_button::operator()() { return specs(); }
	}
}
