//
// progress_indicator_impl.cpp - progress_indicator_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "progress_indicator_impl.h"

namespace liblec {
	namespace lecui {
		widgets::progress_indicator_impl::progress_indicator_impl(containers::page& page,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_empty(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_direct2d_factory(p_direct2d_factory),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr) {}

		widgets::progress_indicator_impl::~progress_indicator_impl() { discard_resources(); }

		widgets::widget_type
			widgets::progress_indicator_impl::type() {
			return lecui::widgets::widget_type::progress_indicator;
		}

		HRESULT widgets::progress_indicator_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = true;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_empty()),
					&_p_brush_empty);
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

		void widgets::progress_indicator_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_empty);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::progress_indicator_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			auto _rect_ellipse = _rect;
			_rect_ellipse.left += (_specs.line_thickness_fill() * 2.f);
			_rect_ellipse.top += (_specs.line_thickness_fill() * 2.f);
			_rect_ellipse.right -= (_specs.line_thickness_fill() * 2.f);
			_rect_ellipse.bottom -= (_specs.line_thickness_fill() * 2.f);

			// draw unfilled circle
			const auto radius_x = (_rect_ellipse.right - _rect_ellipse.left) / 2.f;
			const auto radius_y = (_rect_ellipse.bottom - _rect_ellipse.top) / 2.f;
			D2D1_ELLIPSE ellipse{ D2D1::Point2F(_rect_ellipse.left + radius_x, _rect_ellipse.top + radius_y),
				radius_x,
				radius_y
			};

			p_render_target->DrawEllipse(ellipse, _p_brush_empty, _specs.line_thickness_empty());

			// draw filled arc
			const auto start_point = D2D1::Point2F(_rect_ellipse.left + radius_x, _rect_ellipse.top);

			const double pi = 3.1415926535897932384626433832795;

			float percentage = _specs.percentage();
			percentage = smallest(percentage, 99.9999f);	// so arc is drawn full
			percentage = largest(percentage, 0.f);			// failsafe

			const float angle = static_cast<float>((2 * pi) * percentage / 100.0);
			const float x_adjust = static_cast<float>(radius_x * cos((pi / 2) - angle));
			const float y_adjust = static_cast<float>(radius_y * (1.0 - sin((pi / 2) - angle)));

			const auto end_point = D2D1::Point2F(start_point.x + x_adjust, start_point.y + y_adjust);
			{
				HRESULT hr = S_OK;
				ID2D1PathGeometry* p_arc_geometry = nullptr;
				hr = _p_direct2d_factory->CreatePathGeometry(&p_arc_geometry);

				if (SUCCEEDED(hr)) {
					ID2D1GeometrySink* p_sink = nullptr;
					hr = p_arc_geometry->Open(&p_sink);
					if (SUCCEEDED(hr)) {
						p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);
						p_sink->BeginFigure(start_point,
							D2D1_FIGURE_BEGIN_HOLLOW);
						p_sink->AddArc(
							D2D1::ArcSegment(end_point,
								D2D1::SizeF(radius_x, radius_y),
								0.f,
								D2D1_SWEEP_DIRECTION_CLOCKWISE,
								percentage > 50.f ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL)
							);
						p_sink->EndFigure(D2D1_FIGURE_END_OPEN);

						hr = p_sink->Close();
						safe_release(&p_sink);
					}

					// draw the geometry
					p_render_target->DrawGeometry(p_arc_geometry, _p_brush_fill, _specs.line_thickness_fill());
					safe_release(&p_arc_geometry);
				}
			}

			// draw start line
			p_render_target->DrawLine(D2D1::Point2F(start_point.x, start_point.y - _specs.line_thickness_fill() / 2.f),
				D2D1::Point2F(start_point.x, start_point.y + _specs.line_thickness_fill() * 2.f),
				_p_brush_fill, _specs.line_thickness_fill() / 2.f);

			// draw end dot
			D2D1_ELLIPSE dot{ end_point,
				_specs.line_thickness_fill() * 2.f,
				_specs.line_thickness_fill() * 2.f };

			p_render_target->FillEllipse(dot, _p_brush_fill);

			// create a text layout
			std::string text = round_off::to_string(_specs.percentage(), _specs.precision()) + "%";
			HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(text).c_str(),
				(UINT32)text.length(), _p_text_format, _rect_ellipse.right - _rect_ellipse.left,
				_rect_ellipse.bottom - _rect_ellipse.top, &_p_text_layout);

			if (SUCCEEDED(hr)) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_ellipse.left, _rect_ellipse.top },
					_p_text_layout, _is_enabled ?
					_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&_p_text_layout);

			return _rect;
		}

		widgets::progress_indicator_specs&
			widgets::progress_indicator_impl::specs() { return _specs; }

		widgets::progress_indicator_specs&
			widgets::progress_indicator_impl::operator()() { return specs(); }
	}
}
