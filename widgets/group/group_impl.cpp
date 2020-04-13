/*
** group.cpp - group_impl implementation
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
		widgets_impl::group::group(const std::string& page_alias,
			const std::string& alias) :
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::group::~group() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::group::type() {
			return lecui::widgets_impl::widget_type::group;
		}

		HRESULT widgets_impl::group::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
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

		void widgets_impl::group::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F& widgets_impl::group::render(ID2D1HwndRenderTarget* p_render_target,
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

		void widgets_impl::group::on_click() {
			if (specs_.on_click)
				specs_.on_click();
		}

		widgets::specs::group&
			widgets_impl::group::specs() { return specs_; }
	}
}
