/*
** group_impl.cpp - group_impl implementation
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

#include "group_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::group_impl::group_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr) {}

		widgets_impl::group_impl::~group_impl() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::group_impl::type() {
			return lecui::widgets_impl::widget_type::group;
		}

		HRESULT widgets_impl::group_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = true;

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

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::group_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F& widgets_impl::group_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = convert_rect(specs_.rect);

			if (!render || !visible_)
				return rect_;

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_fill_ : p_brush_disabled_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			if (!is_static_ && is_enabled_ && hit_)
				p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ?
					1.5f : 0.8f);

			return rect_;
		}

		void widgets_impl::group_impl::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		containers::group::group_specs&
			widgets_impl::group_impl::specs() { return specs_; }

		containers::group::group_specs&
			widgets_impl::group_impl::operator()() { return specs(); }
	}
}
