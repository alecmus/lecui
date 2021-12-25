//
// maximize_button_impl.cpp - maximize button widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "maximize_button_impl.h"

namespace liblec {
	namespace lecui {
		widgets::maximize_button_impl::maximize_button_impl(containers::page& page,
			ID2D1Factory* p_direct2d_factory) :
			widget_impl(page, "maximize_button"),
			_p_direct2d_factory(p_direct2d_factory),
			_hWnd(nullptr),
			_p_brush(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr) {}

		widgets::maximize_button_impl::~maximize_button_impl() { discard_resources(); }

		widgets::widget_type
			widgets::maximize_button_impl::type() {
			return lecui::widgets::widget_type::maximize_button;
		}

		HRESULT widgets::maximize_button_impl::create_resources(
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

		void widgets::maximize_button_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
		}

		D2D1_RECT_F& widgets::maximize_button_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!_resources_created)
				create_resources(p_render_target);

			ID2D1SolidColorBrush* _p_brush_current = _is_enabled ? _p_brush : _p_brush_disabled;

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);

			if (!render || !_visible)
				return _rect;

			// draw background
			if (!_is_static && _hit)
				p_render_target->FillRectangle(&_rect, _pressed ? _p_brush_current : _p_brush_hot);

			// draw maximize button
			D2D1_RECT_F _specs_ref(_rect);

			// make square
			float side = smallest(_specs_ref.right - _specs_ref.left, _specs_ref.bottom - _specs_ref.top);
			D2D1_RECT_F _specs{ 0.f, 0.f, side, side };
			pos_rect(_specs_ref, _specs, 50.f, 50.f);

			float _margin = side * .35f;

			_specs.left += _margin;
			_specs.top += _margin;
			_specs.right -= _margin;
			_specs.bottom -= _margin;

			if (maximized(_hWnd)) {
				HRESULT hr = S_OK;
				ID2D1PathGeometry* p_arc_geometry = nullptr;
				hr = _p_direct2d_factory->CreatePathGeometry(&p_arc_geometry);

				if (SUCCEEDED(hr)) {
					ID2D1GeometrySink* p_sink = nullptr;
					hr = p_arc_geometry->Open(&p_sink);
					if (SUCCEEDED(hr)) {
						p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);

						// horizontal line
						p_sink->BeginFigure(D2D1::Point2F(_specs.left, _specs.top),
							D2D1_FIGURE_BEGIN_HOLLOW);
						p_sink->AddLine(D2D1::Point2F(_specs.right - (_specs.right - _specs.left) / 2.5f, _specs.top));
						p_sink->EndFigure(D2D1_FIGURE_END_OPEN);

						// arc (top right corner)
						p_sink->BeginFigure(D2D1::Point2F(_specs.right - (_specs.right - _specs.left) / 2.5f, _specs.top),
							D2D1_FIGURE_BEGIN_HOLLOW);
						p_sink->AddArc(
							D2D1::ArcSegment(D2D1::Point2F(_specs.right, _specs.top + (_specs.bottom - _specs.top) / 2.5f),
								D2D1::SizeF((_specs.right - _specs.left) / 2.5f, (_specs.bottom - _specs.top) / 2.5f),
								0.f,
								D2D1_SWEEP_DIRECTION_CLOCKWISE,
								D2D1_ARC_SIZE_SMALL)
						);
						p_sink->EndFigure(D2D1_FIGURE_END_OPEN);

						// vertical line
						p_sink->BeginFigure(D2D1::Point2F(_specs.right, _specs.top + (_specs.bottom - _specs.top) / 2.5f),
							D2D1_FIGURE_BEGIN_HOLLOW);
						p_sink->AddLine(D2D1::Point2F(_specs.right, _specs.bottom));
						p_sink->EndFigure(D2D1_FIGURE_END_OPEN);

						hr = p_sink->Close();
						safe_release(&p_sink);
					}

					// draw the geometry
					p_render_target->DrawGeometry(p_arc_geometry, (!_is_static && _hit && _pressed) ? _p_brush_hot : _p_brush_current, 1.f);
					safe_release(&p_arc_geometry);
				}

				// small rounded rectangle
				D2D1_RECT_F _specs_fore(_specs);
				float _margin = (_specs_fore.right - _specs_fore.left) * .25f;

				_specs_fore.right -= _margin;
				_specs_fore.top += _margin;

				const D2D1_ROUNDED_RECT rounded_rect{ _specs_fore,
				(_specs_fore.right - _specs_fore.left) / 5.f, (_specs_fore.bottom - _specs_fore.top) / 5.f };

				p_render_target->DrawRoundedRectangle(&rounded_rect, (!_is_static && _hit && _pressed) ? _p_brush_hot : _p_brush_current, 1.f);
			}
			else {
				// large rounded rectangle
				const D2D1_ROUNDED_RECT rounded_rect{ _specs,
				(_specs.right - _specs.left) / 5.f, (_specs.bottom - _specs.top) / 5.f };

				p_render_target->DrawRoundedRectangle(&rounded_rect, (!_is_static && _hit && _pressed) ? _p_brush_hot : _p_brush_current, 1.f);
			}

			return _rect;
		}

		void widgets::maximize_button_impl::on_click() {
			if (IsWindow(_hWnd)) {
				if (maximized(_hWnd)) {
					_specs.tooltip("Maximize");
					ShowWindow(_hWnd, SW_RESTORE);
				}
				else {
					_specs.tooltip("Restore down");
					ShowWindow(_hWnd, SW_MAXIMIZE);
				}
			}

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::maximize_button_impl::set_hwnd(HWND hWnd) { _hWnd = hWnd; }

		widgets::maximize_button_specs&
			widgets::maximize_button_impl::specs() { return _specs; }

		widgets::maximize_button_specs&
			widgets::maximize_button_impl::operator()() { return specs(); }

		bool widgets::maximize_button_impl::maximized(HWND hwnd) {
			WINDOWPLACEMENT placement;

			if (!::GetWindowPlacement(hwnd, &placement))
				return false;

			return placement.showCmd == SW_MAXIMIZE;
		}
	}
}
