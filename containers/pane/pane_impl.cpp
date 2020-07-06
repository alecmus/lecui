/*
** pane_impl.cpp - pane_impl implementation
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

#include "pane_impl.h"
#include "../../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets_impl::pane::tree_pane_alias_prefix() {
			return std::string("lecui::containers::tree_pane::");
		}

		std::string widgets_impl::pane::html_pane_alias_prefix() {
			return std::string("lecui::containers::html_pane::");
		}

		std::string widgets_impl::pane::html_controls_pane_alias_prefix() {
			return std::string("lecui::containers::html_controls_pane::");
		}

		widgets_impl::pane::pane(containers::page& page,
			const std::string& alias) :
			widget(page, alias),
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_disabled_(nullptr),
			margin_(12.f),
			rect_client_area_({ 0.f, 0.f, 0.f, 0.f }),
			rect_pane_({ 0.f, 0.f, 0.f, 0.f }) {}

		widgets_impl::pane::~pane() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::pane::type() {
			return lecui::widgets_impl::widget_type::pane;
		}

		HRESULT widgets_impl::pane::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			is_static_ = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::pane::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_disabled_);
		}

		D2D1_RECT_F&
			widgets_impl::pane::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			rect_pane_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_pane_.left -= offset.x;
			rect_pane_.right -= offset.x;
			rect_pane_.top -= offset.y;
			rect_pane_.bottom -= offset.y;

			// no reponse to hit testing, even though for scroll bar at form level
			// we need to return the entire region through rect_pane_
			rect_ = { 0.f, 0.f, 0.f, 0.f };

			rect_client_area_ = rect_pane_;

			if (!render || !visible_)
				return rect_pane_;

			D2D1_ROUNDED_RECT rounded_rect{ rect_client_area_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect,
				is_enabled_ ? p_brush_fill_ : p_brush_disabled_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			return rect_pane_;
		}

		void widgets_impl::pane::on_click() {}

		containers::pane::pane_specs&
			widgets_impl::pane::specs() { return specs_; }

		containers::pane::pane_specs&
			widgets_impl::pane::operator()() { return specs(); }

		const D2D1_RECT_F& widgets_impl::pane::client_area() {
			return rect_client_area_;
		}

		const D2D1_RECT_F& widgets_impl::pane::pane_area() {
			return rect_pane_;
		}

		bool widgets_impl::pane::contains() { return false; }
	}
}
