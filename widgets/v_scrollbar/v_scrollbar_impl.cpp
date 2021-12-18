//
// v_scrollbar_impl.h - vertical scroll bar widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "v_scrollbar_impl.h"

namespace liblec {
	namespace lecui {
		widgets::v_scrollbar_impl::v_scrollbar_impl(containers::page& page) :
			widget_impl(page, "v_scrollbar"),
			_p_brush(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_hot_pressed(nullptr),
			_p_brush_background(nullptr),
			_rectA({ 0.f, 0.f, 0.f, 0.f }),
			_rectB({ 0.f, 0.f, 0.f, 0.f }),
			_rectC({ 0.f, 0.f, 0.f, 0.f }),
			_rectD({ 0.f, 0.f, 0.f, 0.f }),
			_y_displacement_previous(0.f),
			_y_displacement(0.f),
			_y_off_set(0.f),
			_max_displacement_top(0.f),
			_max_displacement_bottom(0.f),
			_force_translate(false) {}

		widgets::v_scrollbar_impl::~v_scrollbar_impl() { discard_resources(); }

		widgets::widget_type
			widgets::v_scrollbar_impl::type() {
			return lecui::widgets::widget_type::v_scrollbar;
		}

		HRESULT widgets::v_scrollbar_impl::create_resources(
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

		void widgets::v_scrollbar_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_hot_pressed);
			safe_release(&_p_brush_background);
		}

		D2D1_RECT_F&
			widgets::v_scrollbar_impl::render(ID2D1HwndRenderTarget* p_render_target,
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
			position_v_scrollbar(_rectA, _rectB, _rectC, _rectD);

			// adjust _y_displacement to match what the user is seeing. Ensures consistency.
			auto const ui_displacement = get_dpi_scale() * (_rectD.top - _rectC.top);

			if (ui_displacement != _y_displacement) {
				_y_displacement = ui_displacement;
				_force_translate = true;
			}

			if (!render)
				return _rect;

			if (!equal(_rectC, _rectD) &&
				!(round_off::to_float((_rectD.bottom - _rectD.top), precision) >=
					round_off::to_float((_rectC.bottom - _rectC.top), precision))) {
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

		widgets::v_scrollbar_specs&
			widgets::v_scrollbar_impl::specs() { return _specs; }

		widgets::v_scrollbar_specs&
			widgets::v_scrollbar_impl::operator()() { return specs(); }

		void widgets::v_scrollbar_impl::max_displacement(float& top,
			float& bottom) {
			bottom = _rectC.bottom - _rectD.bottom;
			top = _rectC.top - _rectD.top;
		}

		bool widgets::v_scrollbar_impl::translate_y_displacement(
			const float& displacement, float& y_displacement_translated, bool force) {
			if (_pressed || force) {
				// calculate the scale factor for amplifying the movement of the scroll bar
				const float height_A = _rectA.bottom - _rectA.top;
				const float height_C = _rectC.bottom - _rectC.top;
				const float scale_factor = height_C != 0.f ? height_A / height_C : 1.f;

				y_displacement_translated = displacement * scale_factor;
				return true;
			}

			return false;
		}

		void widgets::v_scrollbar_impl::setup(const D2D1_RECT_F& rectA,
			const D2D1_RECT_F& rectB) {
			_rectA = rectA;
			_rectB = rectB;
		}
	}
}
