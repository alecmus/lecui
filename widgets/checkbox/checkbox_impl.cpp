//
// checkbox_impl.cpp - checkbox_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "checkbox_impl.h"

namespace liblec {
	namespace lecui {
		widgets::checkbox_impl::checkbox_impl(containers::page& page,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_check(nullptr),
			_p_brush_border(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_direct2d_factory(p_direct2d_factory),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr) {}

		widgets::checkbox_impl::~checkbox_impl() { discard_resources(); }

		widgets::widget_type
			widgets::checkbox_impl::type() {
			return lecui::widgets::widget_type::checkbox;
		}

		HRESULT widgets::checkbox_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().check == nullptr && _specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_check()),
					&_p_brush_check);
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
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::checkbox_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_check);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::checkbox_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			auto _rect_checkbox = _rect;
			const auto side = smallest(_rect_checkbox.right - _rect_checkbox.left,
				_rect_checkbox.bottom - _rect_checkbox.top);
			_rect_checkbox.right = _rect_checkbox.left + side;
			_rect_checkbox.bottom = _rect_checkbox.top + side;
			pos_rect(_rect, _rect_checkbox, 0.f, 50.f);

			// draw rectangles
			D2D1_ROUNDED_RECT rounded_rect{ _rect_checkbox,
				side / 4.f, side / 4.f };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_static ? _p_brush_fill :
				_hit && _pressed ? _p_brush_fill :
				_hit ? _p_brush_hot :
				_p_brush_fill);

			p_render_target->DrawRoundedRectangle(&rounded_rect, !_is_enabled ? _p_brush_disabled :
				_p_brush_border, _specs.border());

			if (!_is_static && _is_enabled && _selected)
				p_render_target->DrawRoundedRectangle(&rounded_rect, !_is_enabled ? _p_brush_disabled :
					_p_brush_selected, _pressed ? _specs.border() * 3.5f : _specs.border() * 2.f);

			// draw checkbox contents
			std::string text;
			switch (_specs.status()) {
			case widgets::checkbox::checkbox_status::checked: {
				text = _specs.text();
				HRESULT hr = S_OK;
				ID2D1PathGeometry* p_checkbox_geometry = nullptr;
				hr = _p_direct2d_factory->CreatePathGeometry(&p_checkbox_geometry);

				if (SUCCEEDED(hr)) {
					ID2D1GeometrySink* p_sink = nullptr;
					hr = p_checkbox_geometry->Open(&p_sink);
					if (SUCCEEDED(hr)) {
						p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);
						p_sink->BeginFigure(
							D2D1::Point2F(_rect_checkbox.left + (.1f * side), _rect_checkbox.top + (.5f * side)),
							D2D1_FIGURE_BEGIN_FILLED
							);
						D2D1_POINT_2F points[] = {
						   D2D1::Point2F(_rect_checkbox.left + (.4f * side), _rect_checkbox.top + (.7f * side)),
						   D2D1::Point2F(_rect_checkbox.left + (.9f * side), _rect_checkbox.top + (.1f * side)),
						   D2D1::Point2F(_rect_checkbox.left + (.4f * side), _rect_checkbox.top + (.9f * side)),
						};
						p_sink->AddLines(points, ARRAYSIZE(points));
						p_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
						hr = p_sink->Close();
						safe_release(&p_sink);
					}

					// draw the geometry
					p_render_target->FillGeometry(p_checkbox_geometry, _p_brush_check);
					safe_release(&p_checkbox_geometry);
				}
			} break;
			case widgets::checkbox::checkbox_status::unchecked:
				text = _specs.text_unchecked();
				break;
			case widgets::checkbox::checkbox_status::indeterminate: {
				text = _specs.text_indeterminate();
				// draw a horizontal line to show indeterminate state
				auto rect_indeterminate = _rect_checkbox;
				rect_indeterminate.left += (.2f * side);
				rect_indeterminate.right -= (.2f * side);
				rect_indeterminate.top += (.44f * side);
				rect_indeterminate.bottom -= (.44f * side);

				p_render_target->FillRectangle(rect_indeterminate, _p_brush_check);
			} break;
			default:
				break;
			}

			auto _rect_text = _rect;
			_rect_text.left = _rect_checkbox.right + (side / 3.f);

			// create a text layout
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

				if (render && _visible)
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_og.left, rect_text_og.top },
						_p_text_layout, _p_brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&_p_text_layout);

			_rect = _rect_checkbox;
			_rect.right = _rect_text.right;

			return _rect;
		}

		void widgets::checkbox_impl::on_click() {
			switch (_specs.status()) {
			case widgets::checkbox::checkbox_status::unchecked:
				_specs.status(widgets::checkbox::checkbox_status::checked);
				break;
			case widgets::checkbox::checkbox_status::checked:
			case widgets::checkbox::checkbox_status::indeterminate:
			default:
				_specs.status(widgets::checkbox::checkbox_status::unchecked);
				break;
			}

			if (_specs.events().check)
				_specs.events().check(_specs.status());

			if (_specs.events().click)
				_specs.events().click();

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::checkbox_impl::on_action() {
			switch (_specs.status()) {
			case widgets::checkbox::checkbox_status::unchecked:
				_specs.status(widgets::checkbox::checkbox_status::checked);
				break;
			case widgets::checkbox::checkbox_status::checked:
			case widgets::checkbox::checkbox_status::indeterminate:
			default:
				_specs.status(widgets::checkbox::checkbox_status::unchecked);
				break;
			}

			if (_specs.events().check)
				_specs.events().check(_specs.status());

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::checkbox_impl::on_right_click() {
			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		widgets::checkbox&
			widgets::checkbox_impl::specs() { return _specs; }

		widgets::checkbox&
			widgets::checkbox_impl::operator()() { return specs(); }
	}
}
