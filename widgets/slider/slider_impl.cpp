//
// slider_impl.cpp - slider_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "slider_impl.h"
#include "../label/label_impl.h"

namespace liblec {
	namespace lecui {
		widgets::slider_impl::slider_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_knob(nullptr),
			_p_brush_knob_hot(nullptr),
			_p_brush_knob_border(nullptr),
			_p_brush_tick(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_perc_along(0.f) {}

		widgets::slider_impl::~slider_impl() { discard_resources(); }

		widgets::widget_type
			widgets::slider_impl::type() {
			return lecui::widgets::widget_type::slider;
		}

		HRESULT widgets::slider_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().slider == nullptr && _specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_knob()),
					&_p_brush_knob);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_knob_hot()),
					&_p_brush_knob_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_knob_border()),
					&_p_brush_knob_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_tick()),
					&_p_brush_tick);
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

		void widgets::slider_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_knob);
			safe_release(&_p_brush_knob_hot);
			safe_release(&_p_brush_knob_border);
			safe_release(&_p_brush_tick);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::slider_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			// function for snapping value to ticks
			auto do_snap = [&](float& value) {
				std::vector<float> snap_to;
				const float major_div = _specs.major_tick_unit();
				const float minor_div = _specs.major_tick_unit() / (_specs.minor_tick_count() + 1);
				for (float p = _specs.range().minimum; p <= _specs.range().maximum; p += _specs.major_tick_unit()) {
					// add major div to snap vector
					snap_to.push_back(p);

					// add minor divs to snap vector
					for (float v = p + minor_div; v < smallest(p + major_div, _specs.range().maximum); v += minor_div)
						snap_to.push_back(v);
				}

				float snap_value = _specs.range().minimum;

				for (const auto& v : snap_to) {
					auto lower = v;
					auto higher = v + minor_div;
					if (value >= lower && value <= higher) {
						if (value < lower + (minor_div / 2.f))
							snap_value = v;
						else
							snap_value = v + minor_div;
					}
				}

				value = snap_value;
				value = largest(value, _specs.range().minimum);
				value = smallest(value, _specs.range().maximum);
			};

			if (abs(_rect.right - _rect.left) > abs(_rect.bottom - _rect.top)) {
				// orient horizontally

				// starting and ending points of slider
				const D2D1_POINT_2F start = { _rect.left + _specs.knob_radius(), _rect.top + _specs.knob_radius() };
				const D2D1_POINT_2F end = { _rect.right - _specs.knob_radius(), _rect.top + _specs.knob_radius() };

				// draw slide
				const D2D1_RECT_F slide_rect = {
					start.x - (_specs.slide_thickness() / 2.f),
					start.y - (_specs.slide_thickness() / 2.f),
					end.x + (_specs.slide_thickness() / 2.f),
					end.y + (_specs.slide_thickness() / 2.f)
				};

				const D2D1_ROUNDED_RECT slide_rect_rounded = { slide_rect, _specs.slide_thickness() / 2.f, _specs.slide_thickness() / 2.f };
				p_render_target->FillRoundedRectangle(&slide_rect_rounded, _p_brush_fill);
				p_render_target->DrawRoundedRectangle(&slide_rect_rounded, _p_brush_border, _specs.border());

				if (_pressed) {
					// compute new value
					D2D1_RECT_F rect = slide_rect;
					rect.left = start.x;
					rect.right = end.x;

					scale_RECT(rect, get_dpi_scale());

					_perc_along = 100.f * (_point.x - rect.left) / (rect.right - rect.left);
					_perc_along = largest(_perc_along, 0.f);
					_perc_along = smallest(_perc_along, 100.f);

					// calculate value
					_specs.value((_specs.range().maximum - _specs.range().minimum) * _perc_along / 100.f);
				}

				// snap the value to ticks
				if (_specs.snap_to_ticks())
					do_snap(_specs.value());

				// draw ticks. to-do: remove magic numbers
				const float y = slide_rect.bottom + 5.f;
				const float y_major_tick = y + 6.f;
				const float y_minor_tick = y + 2.5f;
				const float major_div = (_specs.major_tick_unit() / (_specs.range().maximum - _specs.range().minimum)) * (end.x - start.x);
				const float minor_div = major_div / (_specs.minor_tick_count() + 1);

				auto value = 0.f;
				const long major_ticks = static_cast<long>(floor((_specs.range().maximum - _specs.range().minimum) / _specs.major_tick_unit()));
				const long minor_ticks = major_ticks * _specs.minor_tick_count();

				long minor_tick_count = 0;
				for (long n_major = 0; n_major < major_ticks + 1; n_major++) {
					const float x_major = start.x + major_div * n_major;

					float lower_margin = y;
					if (_specs.show_tick_marks()) {
						lower_margin = y_major_tick;

						// draw major ticks
						p_render_target->DrawLine({ x_major, y }, { x_major, y_major_tick }, _p_brush_tick, 0.5f);

						for (long n_minor = 1; minor_tick_count < minor_ticks && n_minor < _specs.minor_tick_count() + 1; n_minor++) {
							const float x_minor = x_major + minor_div * n_minor;

							// draw minor ticks
							p_render_target->DrawLine({ x_minor, y }, { x_minor, y_minor_tick }, _p_brush_tick, 0.5f);
							minor_tick_count++;
						}
					}

					// draw tick label
					if (_specs.show_tick_labels()) {
						D2D1_RECT_F max_rect = { 0.f, 0.f, major_div, 20.f };

						const auto label = round_off::to_string(value, 0);
						value += _specs.major_tick_unit();

						auto label_rect = widgets::measure_label(_p_directwrite_factory, label,
							_specs.font(), _specs.font_size(), true, true, max_rect);

						D2D1_RECT_F optimal_rect = {
							x_major - (label_rect.right - label_rect.left) / 2.f,
							lower_margin + .5f,
							x_major + (label_rect.right - label_rect.left) / 2.f,
							lower_margin + .5f + (label_rect.bottom - label_rect.top)
						};

						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(label).c_str(),
							(UINT32)label.length(), _p_text_format, optimal_rect.right - optimal_rect.left,
							optimal_rect.bottom - optimal_rect.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ optimal_rect.left, optimal_rect.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&_p_text_layout);
					}
				}

				// draw knob
				const D2D1_POINT_2F position = {
					start.x + (_specs.value() / (_specs.range().maximum - _specs.range().minimum)) * (end.x - start.x),
					start.y };

				const D2D1_RECT_F knob_rect = {
					position.x - _specs.knob_radius(),
					position.y - _specs.knob_radius(),
					position.x + _specs.knob_radius(),
					position.y + _specs.knob_radius(),
				};

				// draw knob
				const D2D1_ROUNDED_RECT _knob_rect_round = { knob_rect, _specs.knob_radius(), _specs.knob_radius() };
				p_render_target->FillRoundedRectangle(&_knob_rect_round, (!_is_static && _is_enabled && _hit) ? _p_brush_knob_hot : _p_brush_knob);
				p_render_target->DrawRoundedRectangle(&_knob_rect_round, _selected ? _p_brush_selected : _p_brush_knob_border, _specs.border());
			}
			else {
				// orient vertically

				// starting and ending points of slider
				const D2D1_POINT_2F start = { _rect.left + _specs.knob_radius(), _rect.bottom - _specs.knob_radius() };
				const D2D1_POINT_2F end = { _rect.left + _specs.knob_radius(), _rect.top + _specs.knob_radius() };

				// draw slide
				const D2D1_RECT_F slide_rect = {
					start.x - (_specs.slide_thickness() / 2.f),
					start.y + (_specs.slide_thickness() / 2.f),
					end.x + (_specs.slide_thickness() / 2.f),
					end.y - (_specs.slide_thickness() / 2.f)
				};

				const D2D1_ROUNDED_RECT slide_rect_rounded = { slide_rect, _specs.slide_thickness() / 2.f, _specs.slide_thickness() / 2.f };
				p_render_target->FillRoundedRectangle(&slide_rect_rounded, _p_brush_fill);
				p_render_target->DrawRoundedRectangle(&slide_rect_rounded, _p_brush_border, _specs.border());

				if (_pressed) {
					// compute new value
					D2D1_RECT_F rect = slide_rect;
					rect.bottom = start.y;
					rect.top = end.y;

					scale_RECT(rect, get_dpi_scale());

					_perc_along = 100.f * (rect.bottom - _point.y) / (rect.bottom - rect.top);
					_perc_along = largest(_perc_along, 0.f);
					_perc_along = smallest(_perc_along, 100.f);

					// calculate value
					_specs.value() = (_specs.range().maximum - _specs.range().minimum) * _perc_along / 100.f;
				}

				// snap the value to ticks
				if (_specs.snap_to_ticks())
					do_snap(_specs.value());

				// draw ticks. to-do: remove magic numbers
				const float x = slide_rect.right + 5.f;
				const float x_major_tick = x + 6.f;
				const float x_minor_tick = x + 2.5f;
				const float major_div = (_specs.major_tick_unit() / (_specs.range().maximum - _specs.range().minimum)) * (start.y - end.y);
				const float minor_div = major_div / (_specs.minor_tick_count() + 1);

				auto value = 0.f;
				const long major_ticks = static_cast<long>(floor((_specs.range().maximum - _specs.range().minimum) / _specs.major_tick_unit()));
				const long minor_ticks = major_ticks * _specs.minor_tick_count();

				long minor_tick_count = 0;
				for (long n_major = 0; n_major < major_ticks + 1; n_major++) {
					const float y_major = start.y - major_div * n_major;

					float lower_margin = x;
					if (_specs.show_tick_marks()) {
						lower_margin = x_major_tick;

						// draw major ticks
						p_render_target->DrawLine({ x, y_major }, { x_major_tick, y_major }, _p_brush_tick, 0.5f);

						for (long n_minor = 1; minor_tick_count < minor_ticks && n_minor < _specs.minor_tick_count() + 1; n_minor++) {
							const float y_minor = y_major - minor_div * n_minor;

							// draw minor ticks
							p_render_target->DrawLine({ x, y_minor }, { x_minor_tick, y_minor }, _p_brush_tick, 0.5f);
							minor_tick_count++;
						}
					}

					// draw tick label
					if (_specs.show_tick_labels()) {
						D2D1_RECT_F max_rect = { 0.f, 0.f, _rect.right - x_minor_tick - .5f, major_div };

						const auto label = round_off::to_string(value, 0);
						value += _specs.major_tick_unit();

						auto label_rect = widgets::measure_label(_p_directwrite_factory, label,
							_specs.font(), _specs.font_size(), true, true, max_rect);

						D2D1_RECT_F optimal_rect = {
							lower_margin + .5f,
							y_major - (label_rect.bottom - label_rect.top) / 2.f,
							lower_margin + .5f + (label_rect.right - label_rect.left),
							y_major + (label_rect.bottom - label_rect.top) / 2.f
						};

						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(label).c_str(),
							(UINT32)label.length(), _p_text_format, optimal_rect.right - optimal_rect.left,
							optimal_rect.bottom - optimal_rect.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ optimal_rect.left, optimal_rect.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&_p_text_layout);
					}
				}

				// draw knob
				const D2D1_POINT_2F position = { start.x,
					start.y - (_specs.value() / (_specs.range().maximum - _specs.range().minimum)) * (start.y - end.y)};

				const D2D1_RECT_F knob_rect = {
					position.x - _specs.knob_radius(),
					position.y - _specs.knob_radius(),
					position.x + _specs.knob_radius(),
					position.y + _specs.knob_radius(),
				};

				// draw knob
				const D2D1_ROUNDED_RECT _knob_rect_round = { knob_rect, _specs.knob_radius(), _specs.knob_radius() };
				p_render_target->FillRoundedRectangle(&_knob_rect_round, (!_is_static && _is_enabled && _hit) ? _p_brush_knob_hot : _p_brush_knob);
				p_render_target->DrawRoundedRectangle(&_knob_rect_round, _selected ? _p_brush_selected : _p_brush_knob_border, _specs.border());
			}

			return _rect;
		}

		void widgets::slider_impl::on_click() {
			if (_specs.events().slider)
				_specs.events().slider(_specs.value());

			if (_specs.events().click)
				_specs.events().click();

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::slider_impl::on_action() {
			if (_specs.events().slider)
				_specs.events().slider(_specs.value());

			if (_specs.events().action)
				_specs.events().action();
		}

		void widgets::slider_impl::on_right_click() {
			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		bool widgets::slider_impl::contains(const D2D1_POINT_2F& point) {
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

		bool widgets::slider_impl::contains() { return true; }

		bool widgets::slider_impl::hit(const bool& hit) {
			if (_is_static || hit == _hit) {
				if (_pressed)
					return true;
				else
					return false;
			}

			_hit = hit;
			return true;
		}

		widgets::slider&
			widgets::slider_impl::specs() { return _specs; }

		widgets::slider&
			widgets::slider_impl::operator()() { return specs(); }
	}
}
