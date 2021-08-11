//
// strength_bar_impl.cpp - strength_bar_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "strength_bar_impl.h"

namespace liblec {
	namespace lecui {
		widgets::strength_bar_impl::strength_bar_impl(containers::page& page,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_direct2d_factory(p_direct2d_factory),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr) {}

		widgets::strength_bar_impl::~strength_bar_impl() { discard_resources(); }

		widgets::widget_type
			widgets::strength_bar_impl::type() {
			return lecui::widgets::widget_type::strength_bar;
		}

		HRESULT widgets::strength_bar_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = true;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr)) {
				for (const auto& [level, color] : _specs.levels()) {
					// add map entry for this level
					_fill_brushes[level] = nullptr;

					// create brush for this level
					hr = p_render_target->CreateSolidColorBrush(convert_color(color),
						&_fill_brushes.at(level));
				}
			}
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

		void widgets::strength_bar_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_fill);

			for (auto& [level, p_brush] : _fill_brushes)
				safe_release(&p_brush);
			
			_fill_brushes.clear();

			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::strength_bar_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// sanity check
			_specs.percentage(smallest(_specs.percentage(), 100.f));
			_specs.percentage(largest(_specs.percentage(), 0.f));

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			// draw unfilled rect
			auto corner_radius = smallest((_rect.bottom - _rect.top) / 3.f,
				(_rect.right - _rect.left) / 3.f);
			D2D1_ROUNDED_RECT unfilled_rect{ _rect, corner_radius, corner_radius };
			p_render_target->DrawRoundedRectangle(unfilled_rect, _p_brush_border, _specs.border());

			// draw filled rect
			auto rect_subject = _rect;
			rect_subject.left += (2.f * _specs.border());
			rect_subject.right -= (2.f * _specs.border());
			rect_subject.top += (2.f * _specs.border());
			rect_subject.bottom -= (2.f * _specs.border());
			corner_radius = smallest((rect_subject.bottom - rect_subject.top) / 3.f,
				(rect_subject.right - rect_subject.left) / 3.f);

			{
				// define rectangle that should contain fill
				auto rect_fill = rect_subject;

				// clip
				auto_clip clip(render, p_render_target, rect_fill, 0.f);

				D2D1_ROUNDED_RECT filled_rect{ rect_subject, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(filled_rect, _p_brush_fill);
			}

			float previous_level = 0.f;

			for (auto& [level, p_brush] : _fill_brushes) {
				float perc = smallest(level, _specs.percentage());

				// define rectangle that should contain fill
				auto rect_fill = rect_subject;
				rect_fill.right = rect_fill.left + (perc * (rect_fill.right - rect_fill.left) / 100.f);

				if (previous_level)
					rect_fill.left = previous_level;

				previous_level = rect_fill.right;

				if (rect_fill.left >= rect_fill.right)
					break;

				// clip
				auto_clip clip(render, p_render_target, rect_fill, 0.f);

				D2D1_ROUNDED_RECT filled_rect{ rect_subject, corner_radius, corner_radius };
				p_render_target->FillRoundedRectangle(filled_rect, p_brush);
			}

			return _rect;
		}

		widgets::strength_bar&
			widgets::strength_bar_impl::specs() { return _specs; }

		widgets::strength_bar&
			widgets::strength_bar_impl::operator()() { return specs(); }
	}
}
