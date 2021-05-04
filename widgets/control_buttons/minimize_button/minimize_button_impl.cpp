/*
** minimize_button_impl.cpp - minimize button widget implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "minimize_button_impl.h"

namespace liblec {
	namespace lecui {
		widgets::minimize_button_impl::minimize_button_impl(containers::page& page) :
			widget_impl(page, "minimize_button"),
			hWnd_(nullptr),
			p_brush_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr) {}

		widgets::minimize_button_impl::~minimize_button_impl() { discard_resources(); }

		widgets::widget_type
			widgets::minimize_button_impl::type() {
			return lecui::widgets::widget_type::minimize_button;
		}

		HRESULT widgets::minimize_button_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
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

		void widgets::minimize_button_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F& widgets::minimize_button_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			ID2D1SolidColorBrush* p_brush_current_ = is_enabled_ ? p_brush_ : p_brush_disabled_;

			if (!is_static_ && is_enabled_ && hit_)
				p_brush_current_ = p_brush_hot_;

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);

			if (!render || !visible_)
				return rect_;

			if (!is_static_ && hit_)
				p_render_target->DrawRectangle(&rect_, p_brush_, pressed_ ? .5f : .0f);

			// draw minimize button
			D2D1_RECT_F specs_(rect_);

			float margin_ = (rect_.right - rect_.left) * .30f;

			specs_.left += margin_;
			specs_.top += margin_;
			specs_.right -= margin_;
			specs_.bottom -= margin_;

			p_render_target->DrawLine(
				D2D1_POINT_2F{ specs_.left, specs_.bottom - 1.f },
				D2D1_POINT_2F{ specs_.right, specs_.bottom - 1.f },
				p_brush_current_, 2.f);

			return rect_;
		}

		void widgets::minimize_button_impl::on_click() {
			if (IsWindow(hWnd_))
				ShowWindow(hWnd_, SW_MINIMIZE);

			if (specs_.events().action)
				specs_.events().action();
		}

		void widgets::minimize_button_impl::set_hwnd(HWND hWnd) { hWnd_ = hWnd; }

		widgets::minimize_button_specs&
			widgets::minimize_button_impl::specs() { return specs_; }

		widgets::minimize_button_specs&
			widgets::minimize_button_impl::operator()() { return specs(); }
	}
}
