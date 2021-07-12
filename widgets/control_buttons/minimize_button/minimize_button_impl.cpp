//
// minimize_button_impl.cpp - minimize button widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "minimize_button_impl.h"

namespace liblec {
	namespace lecui {
		widgets::minimize_button_impl::minimize_button_impl(containers::page& page) :
			widget_impl(page, "minimize_button"),
			_hWnd(nullptr),
			_p_brush(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr) {}

		widgets::minimize_button_impl::~minimize_button_impl() { discard_resources(); }

		widgets::widget_type
			widgets::minimize_button_impl::type() {
			return lecui::widgets::widget_type::minimize_button;
		}

		HRESULT widgets::minimize_button_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);

			_resources_created = true;
			return hr;
		}

		void widgets::minimize_button_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
		}

		D2D1_RECT_F& widgets::minimize_button_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!_resources_created)
				create_resources(p_render_target);

			ID2D1SolidColorBrush* _p_brush_current = _is_enabled ? _p_brush : _p_brush_disabled;

			if (!_is_static && _is_enabled && _hit)
				_p_brush_current = _p_brush_hot;

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);

			if (!render || !_visible)
				return _rect;

			if (!_is_static && _hit)
				p_render_target->DrawRectangle(&_rect, _p_brush, _pressed ? .5f : .0f);

			// draw minimize button
			D2D1_RECT_F _specs(_rect);

			float _margin = (_rect.right - _rect.left) * .30f;

			_specs.left += _margin;
			_specs.top += _margin;
			_specs.right -= _margin;
			_specs.bottom -= _margin;

			p_render_target->DrawLine(
				D2D1_POINT_2F{ _specs.left, _specs.bottom - 1.f },
				D2D1_POINT_2F{ _specs.right, _specs.bottom - 1.f },
				_p_brush_current, 2.f);

			return _rect;
		}

		void widgets::minimize_button_impl::on_click() {
			if (IsWindow(_hWnd))
				ShowWindow(_hWnd, SW_MINIMIZE);

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::minimize_button_impl::set_hwnd(HWND hWnd) { _hWnd = hWnd; }

		widgets::minimize_button_specs&
			widgets::minimize_button_impl::specs() { return _specs; }

		widgets::minimize_button_specs&
			widgets::minimize_button_impl::operator()() { return specs(); }
	}
}
