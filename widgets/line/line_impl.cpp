//
// line_impl.cpp - line_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "line_impl.h"

namespace liblec {
	namespace lecui {
		widgets::line_impl::line_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr) {}

		widgets::line_impl::~line_impl() { discard_resources(); }

		widgets::widget_type
			widgets::line_impl::type() {
			return lecui::widgets::widget_type::line;
		}

		HRESULT widgets::line_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
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

		void widgets::line_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
		}

		D2D1_RECT_F&
			widgets::line_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			if (!_specs.points().empty()) {
				D2D1_POINT_2F previous = { _rect.left + _specs.points()[0].x, _rect.top + _specs.points()[0].y };
				for (auto& pt : _specs.points()) {
					D2D1_POINT_2F current = { _rect.left + pt.x, _rect.top + pt.y };

					if (!(current.x == previous.x && current.y == previous.y))
						p_render_target->DrawLine(previous, current, _is_enabled ?
							(_hit ? _p_brush_hot :
								(_selected ? _p_brush_selected : _p_brush_fill)) : _p_brush_disabled,
							_specs.thickness());

					previous = current;
				}
			}

			return _rect;
		}

		widgets::line_specs&
			widgets::line_impl::specs() { return _specs; }

		widgets::line_specs&
			widgets::line_impl::operator()() { return specs(); }
	}
}
