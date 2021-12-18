//
// toggle_impl.cpp - toggle_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "toggle_impl.h"

namespace liblec {
	namespace lecui {
		widgets::toggle_impl::toggle_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_on(nullptr),
			_p_brush_off(nullptr),
			_p_brush_on_hot(nullptr),
			_p_brush_off_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_perc_along(0.f) {}

		widgets::toggle_impl::~toggle_impl() { discard_resources(); }

		widgets::widget_type
			widgets::toggle_impl::type() {
			return lecui::widgets::widget_type::toggle;
		}

		HRESULT widgets::toggle_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().toggle == nullptr && _specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_on()),
					&_p_brush_on);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_off()),
					&_p_brush_off);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(
					convert_color(lighten_color(_specs.color_on(), 25)), &_p_brush_on_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(
					convert_color(lighten_color(_specs.color_off(), 25)), &_p_brush_off_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = _p_directwrite_factory->CreateTextFormat(
					convert_string(_specs.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(_specs.font_size()),
					L"", //locale
					&_p_text_format
					);
			}
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::toggle_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_on);
			safe_release(&_p_brush_off);
			safe_release(&_p_brush_on_hot);
			safe_release(&_p_brush_off_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::toggle_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			const float toggle_height = smallest(_rect.bottom - _rect.top, _rect.right - _rect.left);
			const float toggle_width = 2.f * toggle_height;

			_rect_toggle = _rect;
			_rect_toggle.right = _rect_toggle.left + toggle_width;
			_rect_toggle.bottom = _rect_toggle.top + toggle_height;
			pos_rect(_rect, _rect_toggle, 0.f, 50.f);

			D2D1_RECT_F _toggle_background = _rect_toggle;
			D2D1_ROUNDED_RECT _toggle_background_round = { _toggle_background,
				(_toggle_background.bottom - _toggle_background.top) / 2.f,
				(_toggle_background.bottom - _toggle_background.top) / 2.f };

			D2D1_RECT_F _toggle_foreground = _rect_toggle;
			_toggle_foreground.right = _toggle_foreground.right -
				(_toggle_foreground.right - _toggle_foreground.left) / 2.f;

			_perc_along = _specs.on() ? 100.f : 0.f;

			bool x_change = (_point.x != _point_on_press.x);
			bool y_change = (_point.y != _point_on_press.y);

			if (_pressed && (x_change || y_change)) {
				D2D1_RECT_F rect = _rect_toggle;
				rect.left += ((_toggle_foreground.right - _toggle_foreground.left) / 2.f);
				rect.right -= ((_toggle_foreground.right - _toggle_foreground.left) / 2.f);

				scale_RECT(rect, get_dpi_scale());

				const float mid_point_x = rect.left + ((rect.right - rect.left) / 2.f);

				_perc_along = 100.f * (_point.x - rect.left) / (rect.right - rect.left);
				_perc_along = largest(_perc_along, 0.f);
				_perc_along = smallest(_perc_along, 100.f);
			}

			pos_rect(_rect_toggle, _toggle_foreground, _perc_along, 50.f);

			// to-do: shrink by exactly 90%
			const float shrink_amount = .05f * (_toggle_foreground.right - _toggle_foreground.left);
			_toggle_foreground.left += shrink_amount;
			_toggle_foreground.right -= shrink_amount;
			_toggle_foreground.top += shrink_amount;
			_toggle_foreground.bottom -= shrink_amount;

			D2D1_ROUNDED_RECT _toggle_foreground_round = { _toggle_foreground,
				(_toggle_foreground.bottom - _toggle_foreground.top) / 2.f,
				(_toggle_foreground.bottom - _toggle_foreground.top) / 2.f };

			ID2D1SolidColorBrush* p_brush = (_perc_along >= 50.f) ? _p_brush_on : _p_brush_off;
			ID2D1SolidColorBrush* p_brush_hot = (_perc_along >= 50.f) ?
				_p_brush_on_hot : _p_brush_off_hot;

			if (render && _visible) {
				p_render_target->FillRoundedRectangle(&_toggle_background_round, !_is_enabled ?
					_p_brush_disabled : _hit ? p_brush_hot : p_brush);
				p_render_target->FillRoundedRectangle(&_toggle_foreground_round, !_is_enabled ?
					_p_brush_fill : _p_brush_fill);
			}

			auto _rect_text = _rect;
			_rect_text.left = _rect_toggle.right + ((_rect_toggle.bottom - _rect_toggle.top) / 3.f);

			// create a text layout
			const std::string& text = _specs.on() ? _specs.text() : _specs.text_off();
			HRESULT hr = _p_directwrite_factory->CreateTextLayout(
				convert_string(text).c_str(),
				(UINT32)text.length(),
				_p_text_format,
				_rect_text.right - _rect_text.left,
				_rect_text.bottom - _rect_text.top,
				&_p_text_layout);

			if (SUCCEEDED(hr)) {
				DWRITE_TEXT_METRICS textMetrics;
				_p_text_layout->GetMetrics(&textMetrics);

				const auto rect_text_og = _rect_text;
				_rect_text.left += textMetrics.left;
				_rect_text.top += textMetrics.top;
				_rect_text.right = smallest(_rect_text.left + textMetrics.width, _rect_text.right);
				_rect_text.bottom = smallest(_rect_text.top + textMetrics.height, _rect_text.bottom);

				if (render && _visible && !_is_static && _is_enabled && _selected)
					p_render_target->FillRectangle(&_rect_text, _p_brush_selected);

				if (render && _visible)
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_og.left, rect_text_og.top },
						_p_text_layout, _p_brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			_rect.right = _rect_text.right;

			// release the text layout
			safe_release(&_p_text_layout);

			return _rect;
		}

		void widgets::toggle_impl::on_click() {
			bool x_change = (_point.x != _point_on_press.x);
			bool y_change = (_point.y != _point_on_press.y);

			if (!(x_change || y_change)) {
				_specs.on() = !_specs.on();	// A click with no mouse move. Toggle.
				_perc_along = _specs.on() ? 100.f : 0.f;
			}
			else
				_specs.on() = _perc_along >= 50.f;

			if (_specs.events().toggle)
				_specs.events().toggle(_specs.on());

			if (_specs.events().click)
				_specs.events().click();
		}

		void widgets::toggle_impl::on_action() {
			_specs.on(!_specs.on());	// Toggle.

			if (_specs.events().toggle)
				_specs.events().toggle(_specs.on());

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::toggle_impl::on_right_click() {
			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		bool widgets::toggle_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			_point = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = _rect;
			scale_RECT(rect, get_dpi_scale());

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return contains();
			else {
				if (_pressed)
					return true;
				else
					return false;
			}
		}

		bool widgets::toggle_impl::contains() { return true; }

		bool widgets::toggle_impl::hit(const bool& hit) {
			if (_is_static || hit == _hit) {
				if (_pressed)
					return true;
				else
					return false;
			}

			_hit = hit;
			return true;
		}

		widgets::toggle&
			widgets::toggle_impl::specs() { return _specs; }

		widgets::toggle&
			widgets::toggle_impl::operator()() { return specs(); }
	}
}
