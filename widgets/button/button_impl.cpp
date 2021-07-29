//
// button_impl.cpp - button_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "button_impl.h"

namespace liblec {
	namespace lecui {
		widgets::button_impl::button_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_border(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr) {}

		widgets::button_impl::~button_impl() { discard_resources(); }

		widgets::widget_type
			widgets::button_impl::type() {
			return lecui::widgets::widget_type::button;
		}

		HRESULT widgets::button_impl::create_resources(
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
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
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::button_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::button_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_static ? _p_brush_fill :
				_hit && _pressed ? _p_brush_fill :
				_hit ? _p_brush_hot :
				_p_brush_fill);

			p_render_target->DrawRoundedRectangle(&rounded_rect, !_is_enabled ? _p_brush_disabled :
				_p_brush_border, .5f);

			if (!_is_static && _is_enabled && _selected)
				p_render_target->DrawRoundedRectangle(&rounded_rect, !_is_enabled ? _p_brush_disabled :
					_p_brush_selected, _pressed ? 1.75f : 1.f);

			// create a text layout
			HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_specs.text()).c_str(),
				(UINT32)_specs.text().length(), _p_text_format, _rect.right - _rect.left,
				_rect.bottom - _rect.top, &_p_text_layout);

			if (SUCCEEDED(hr)) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect.left, _rect.top },
					_p_text_layout, _is_enabled ?
					_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&_p_text_layout);

			return _rect;
		}

		void widgets::button_impl::on_click() {
			if (_specs.events().click)
				_specs.events().click();

			if (_specs.events().action)
				_specs.events().action();
		}

		widgets::button&
			widgets::button_impl::specs() { return _specs; }

		widgets::button&
			widgets::button_impl::operator()() { return specs(); }
	}
}
