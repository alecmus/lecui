/*
** maximize_button_impl.cpp - maximize button widget implementation
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

#include "maximize_button_impl.h"

namespace liblec {
	namespace lecui {
		widgets::maximize_button_impl::maximize_button_impl(containers::page& page) :
			widget_impl(page, "maximize_button"),
			hWnd_(nullptr),
			p_brush_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr) {}

		widgets::maximize_button_impl::~maximize_button_impl() { discard_resources(); }

		widgets::widget_type
			widgets::maximize_button_impl::type() {
			return lecui::widgets::widget_type::maximize_button;
		}

		HRESULT widgets::maximize_button_impl::create_resources(
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

		void widgets::maximize_button_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F& widgets::maximize_button_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			// draw maximize button
			D2D1_RECT_F specs_(rect_);

			float margin_ = (rect_.right - rect_.left) * .30f;

			specs_.left += margin_;
			specs_.top += margin_;
			specs_.right -= margin_;
			specs_.bottom -= margin_;

			if (maximized(hWnd_)) {
				p_render_target->DrawLine(
					D2D1_POINT_2F{ specs_.left, specs_.top },
					D2D1_POINT_2F{ specs_.right, specs_.top },
					p_brush_current_, 1.f);
				p_render_target->DrawLine(
					D2D1_POINT_2F{ specs_.right, specs_.top },
					D2D1_POINT_2F{ specs_.right, specs_.bottom },
					p_brush_current_, 1.f);

				D2D1_RECT_F specs_fore_(specs_);
				float margin_ = (specs_fore_.right - specs_fore_.left) * .25f;

				specs_fore_.right -= margin_;
				specs_fore_.top += margin_;

				p_render_target->DrawRectangle(&specs_fore_, p_brush_current_, 1.f);

				p_render_target->DrawLine(
					D2D1_POINT_2F{ specs_fore_.left, specs_fore_.top + 1.f },
					D2D1_POINT_2F{ specs_fore_.right, specs_fore_.top + 1.f },
					p_brush_current_, 2.f);
			}
			else {
				p_render_target->DrawRectangle(&specs_, p_brush_current_, 1.f);

				p_render_target->DrawLine(
					D2D1_POINT_2F{ specs_.left, specs_.top + 1.f },
					D2D1_POINT_2F{ specs_.right, specs_.top + 1.f },
					p_brush_current_, 2.f);
			}

			return rect_;
		}

		void widgets::maximize_button_impl::on_click() {
			if (IsWindow(hWnd_)) {
				if (maximized(hWnd_))
					ShowWindow(hWnd_, SW_RESTORE);
				else
					ShowWindow(hWnd_, SW_MAXIMIZE);
			}
		}

		void widgets::maximize_button_impl::set_hwnd(HWND hWnd) { hWnd_ = hWnd; }

		widgets::maximize_button_specs&
			widgets::maximize_button_impl::specs() { return specs_; }

		widgets::maximize_button_specs&
			widgets::maximize_button_impl::operator()() { return specs(); }

		bool widgets::maximize_button_impl::maximized(HWND hwnd) {
			WINDOWPLACEMENT placement;

			if (!::GetWindowPlacement(hwnd, &placement))
				return false;

			return placement.showCmd == SW_MAXIMIZE;
		}
	}
}
