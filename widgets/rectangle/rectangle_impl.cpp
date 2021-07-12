//
// rectangle_impl.cpp - rectangle_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "rectangle_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets::rectangle_impl::page_rect_alias() {
			return std::string("lecui::minimal_page_border_rect");
		}

		widgets::rectangle_impl::rectangle_impl(containers::page& page,
			const std::string& alias,
			widgets::h_scrollbar_impl& h_scrollbar,
			widgets::v_scrollbar_impl& v_scrollbar) :
			widget_impl(page, alias),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_border_hot(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_h_scrollbar(h_scrollbar),
			_v_scrollbar(v_scrollbar) {}

		widgets::rectangle_impl::~rectangle_impl() { discard_resources(); }

		widgets::widget_type
			widgets::rectangle_impl::type() {
			return lecui::widgets::widget_type::rectangle;
		}

		HRESULT widgets::rectangle_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border_hot()),
					&_p_brush_border_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);

			_resources_created = true;
			return hr;
		}

		void widgets::rectangle_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_border_hot);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
		}

		D2D1_RECT_F&
			widgets::rectangle_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			const D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_enabled ?
				(_hit ? _p_brush_hot : _p_brush_fill) : _p_brush_disabled);
			p_render_target->DrawRoundedRectangle(&rounded_rect, _is_enabled ?
				(_hit ? _p_brush_border_hot : _p_brush_border) : _p_brush_disabled, _specs.border());

			if (!_is_static && _is_enabled) {
				if (_pressed)
					p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_hot, 1.f);

				if (_selected)
					p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_selected, _pressed ?
						1.75f : 1.f);
			}

			return _rect;
		}

		bool widgets::rectangle_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			_point = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = _rect;
			scale_RECT(rect, get_dpi_scale());

			if (_alias == page_rect_alias()) {
				// this is a special rectangle used to manage pages.
				// scrollbar movements move everything, including the minimal page border rect.
				// keep the page virtual hit area over the actual page by ignoring scrollbar movements.

				rect.left += _h_scrollbar._x_off_set;
				rect.right += _h_scrollbar._x_off_set;
				rect.top += _v_scrollbar._y_off_set;
				rect.bottom += _v_scrollbar._y_off_set;
			}

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return true;
			else
				return false;
		}

		D2D1_POINT_2F widgets::rectangle_impl::get_scrollbar_offset() {
			return { _h_scrollbar._x_off_set, _v_scrollbar._y_off_set };
		}

		widgets::rectangle_specs&
			widgets::rectangle_impl::specs() { return _specs; }

		widgets::rectangle_specs&
			widgets::rectangle_impl::operator()() { return specs(); }
	}
}
