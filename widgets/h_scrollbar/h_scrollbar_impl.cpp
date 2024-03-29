//
// h_scrollbar_impl.cpp - horizontal scroll bar widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "h_scrollbar_impl.h"

namespace liblec {
	namespace lecui {
		widgets::h_scrollbar_impl::h_scrollbar_impl(containers::page& page) :
			widget_impl(page, "h_scrollbar"),
			_p_brush(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_hot_pressed(nullptr),
			_p_brush_background(nullptr),
			_rectA({ 0.f, 0.f, 0.f, 0.f }),
			_rectB({ 0.f, 0.f, 0.f, 0.f }),
			_rectC({ 0.f, 0.f, 0.f, 0.f }),
			_rectD({ 0.f, 0.f, 0.f, 0.f }),
			_x_displacement_previous(0.f),
			_x_displacement(0.f),
			_x_off_set(0.f),
			_max_displacement_left(0.f),
			_max_displacement_right(0.f),
			_force_translate(false) {}

		widgets::h_scrollbar_impl::~h_scrollbar_impl() { discard_resources(); }

		widgets::widget_type
			widgets::h_scrollbar_impl::type() {
			return lecui::widgets::widget_type::h_scrollbar;
		}

		HRESULT widgets::h_scrollbar_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_scrollbar_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot_pressed()),
					&_p_brush_hot_pressed);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_background()),
					&_p_brush_background);

			_resources_created = true;
			return hr;
		}

		void widgets::h_scrollbar_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_hot_pressed);
			safe_release(&_p_brush_background);
		}

		D2D1_RECT_F&
			widgets::h_scrollbar_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!_resources_created)
				create_resources(p_render_target);

			const int precision = 3;	// to prevent false-positives (4 is enough, 3 is a failsafe)
			auto equal = [&](const D2D1_RECT_F& rect_1, const D2D1_RECT_F& rect_2) {
				if (round_off::to_float(rect_1.left, precision) == round_off::to_float(rect_2.left, precision) &&
					round_off::to_float(rect_1.top, precision) == round_off::to_float(rect_2.top, precision) &&
					round_off::to_float(rect_1.right, precision) == round_off::to_float(rect_2.right, precision) &&
					round_off::to_float(rect_1.bottom, precision) == round_off::to_float(rect_2.bottom, precision))
					return true;
				else
					return false;
			};

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			_rectC = _rect;

			_rectD = { 0.f, 0.f, 0.f, 0.f };
			position_h_scrollbar(_rectA, _rectB, _rectC, _rectD);

			// adjust _x_displacement to match what the user is seeing. Ensures consistency.
			auto const ui_displacement = get_dpi_scale() * (_rectD.left - _rectC.left);
			
			if (ui_displacement != _x_displacement) {
				_x_displacement = ui_displacement;
				_force_translate = true;
			}

			if (!render)
				return _rect;

			if (!equal(_rectC, _rectD) &&
				!(round_off::to_float((_rectD.right - _rectD.left), precision) >=
					round_off::to_float((_rectC.right - _rectC.left), precision))) {
				auto corner_radius = smallest((_rectD.bottom - _rectD.top) / 3.f,
					(_rectD.right - _rectD.left) / 3.f);

				// scroll area
				D2D1_ROUNDED_RECT rounded_rectC{ _rectC, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(&rounded_rectC, _p_brush_background);

				// scroll bar
				auto rect_scroll_bar = _rectD;
				const float scroll_bar_margin = 2.f;
				rect_scroll_bar.left += scroll_bar_margin;
				rect_scroll_bar.top += scroll_bar_margin;
				rect_scroll_bar.right -= scroll_bar_margin;
				rect_scroll_bar.bottom -= scroll_bar_margin;
				corner_radius = smallest((rect_scroll_bar.bottom - rect_scroll_bar.top) / 3.f,
					(rect_scroll_bar.right - rect_scroll_bar.left) / 3.f);
				D2D1_ROUNDED_RECT rounded_rectD{ rect_scroll_bar, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(&rounded_rectD,
					_pressed ? _p_brush_hot_pressed : (_hit ? _p_brush_hot : _p_brush));
				p_render_target->DrawRoundedRectangle(&rounded_rectD, _p_brush_border);

				_visible = true;
			}
			else
				_visible = false;

			return _rect;
		}

		widgets::h_scrollbar_specs&
			widgets::h_scrollbar_impl::specs() { return _specs; }

		widgets::h_scrollbar_specs&
			widgets::h_scrollbar_impl::operator()() { return specs(); }

		void widgets::h_scrollbar_impl::max_displacement(float& left,
			float& right) {
			right = _rectC.right - _rectD.right;
			left = _rectC.left - _rectD.left;
		}

		bool widgets::h_scrollbar_impl::translate_x_displacement(
			const float& x_displacement, float& x_displacement_translated, bool force) {
			if (_pressed || force) {
				// calculate the scale factor for amplifying the movement of the scroll bar
				const float width_A = _rectA.right - _rectA.left;
				const float width_C = _rectC.right - _rectC.left;
				const float scale_factor = width_C != 0.f ? width_A / width_C : 1.f;

				x_displacement_translated = x_displacement * scale_factor;
				return true;
			}

			return false;
		}

		void widgets::h_scrollbar_impl::reverse_translate_x_displacement(
			const float& x_displacement_translated, float& x_displacement) {
			// calculate the scale factor for amplifying the movement of the scroll bar
			const float width_A = _rectA.right - _rectA.left;
			const float width_C = _rectC.right - _rectC.left;
			const float scale_factor = width_C != 0.f ? width_A / width_C : 1.f;

			x_displacement = x_displacement_translated / scale_factor;
		}

		void widgets::h_scrollbar_impl::setup(const D2D1_RECT_F& rectA,
			const D2D1_RECT_F& rectB) {
			_rectA = rectA;
			_rectB = rectB;
		}
	}
}
