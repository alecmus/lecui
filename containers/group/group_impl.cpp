//
// group_impl.cpp - group_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "group_impl.h"

namespace liblec {
	namespace lecui {
		widgets::group_impl::group_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr) {}

		widgets::group_impl::~group_impl() { discard_resources(); }

		widgets::widget_type
			widgets::group_impl::type() {
			return lecui::widgets::widget_type::group;
		}

		HRESULT widgets::group_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = true;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);

			_resources_created = true;
			return hr;
		}

		void widgets::group_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
		}

		D2D1_RECT_F& widgets::group_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect = convert_rect(_specs.rect());

			if (!render || !_visible)
				return _rect;

			D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_fill : _p_brush_disabled);
			p_render_target->DrawRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_border : _p_brush_disabled, _specs.border());

			if (!_is_static && _is_enabled && _hit)
				p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_hot, _pressed ?
					1.5f : 0.8f);

			return _rect;
		}

		containers::group&
			widgets::group_impl::specs() { return _specs; }

		containers::group&
			widgets::group_impl::operator()() { return specs(); }
	}
}
