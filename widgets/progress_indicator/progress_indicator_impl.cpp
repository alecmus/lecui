/*
** progress_indicator_impl.cpp - progress_indicator_impl implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "progress_indicator_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::progress_indicator::progress_indicator(const std::string& page_alias,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_empty_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_direct2d_factory_(p_direct2d_factory),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::progress_indicator::~progress_indicator() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::progress_indicator::type() {
			return lecui::widgets_impl::widget_type::progress_indicator;
		}

		HRESULT widgets_impl::progress_indicator::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = true;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_empty),
					&p_brush_empty_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
					&p_brush_);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory_->CreateTextFormat(
					convert_string(specs_.font).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(specs_.font_size),
					L"", //locale
					&p_text_format_
					);
			}
			if (SUCCEEDED(hr)) {
				p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::progress_indicator::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_empty_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::progress_indicator::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			// sanity check
			specs_.percentage = smallest(specs_.percentage, 100.f);
			specs_.percentage = largest(specs_.percentage, 0.f);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			auto rect_ellipse_ = rect_;
			rect_ellipse_.left += (specs_.line_thickness_fill * 2.f);
			rect_ellipse_.top += (specs_.line_thickness_fill * 2.f);
			rect_ellipse_.right -= (specs_.line_thickness_fill * 2.f);
			rect_ellipse_.bottom -= (specs_.line_thickness_fill * 2.f);

			// draw unfilled circle
			const auto radius_x = (rect_ellipse_.right - rect_ellipse_.left) / 2.f;
			const auto radius_y = (rect_ellipse_.bottom - rect_ellipse_.top) / 2.f;
			D2D1_ELLIPSE ellipse{ D2D1::Point2F(rect_ellipse_.left + radius_x, rect_ellipse_.top + radius_y),
				radius_x,
				radius_y
			};

			p_render_target->DrawEllipse(ellipse, p_brush_empty_, specs_.line_thickness_empty);

			// draw filled arc
			const auto start_point = D2D1::Point2F(rect_ellipse_.left + radius_x, rect_ellipse_.top);

			const double pi = 3.1415926535897932384626433832795;

			float percentage = specs_.percentage;
			percentage = smallest(percentage, 99.9999f);	// so arc is drawn full
			percentage = largest(percentage, 0.f);			// failsafe

			const float angle = static_cast<float>((2 * pi) * percentage / 100.0);
			const float x_adjust = static_cast<float>(radius_x * cos((pi / 2) - angle));
			const float y_adjust = static_cast<float>(radius_y * (1.0 - sin((pi / 2) - angle)));

			const auto end_point = D2D1::Point2F(start_point.x + x_adjust, start_point.y + y_adjust);
			{
				HRESULT hr = S_OK;
				ID2D1PathGeometry* p_arc_geometry = nullptr;
				hr = p_direct2d_factory_->CreatePathGeometry(&p_arc_geometry);

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
					p_render_target->DrawGeometry(p_arc_geometry, p_brush_fill_, specs_.line_thickness_fill);
					safe_release(&p_arc_geometry);
				}
			}

			// draw start line
			p_render_target->DrawLine(D2D1::Point2F(start_point.x, start_point.y - specs_.line_thickness_fill / 2.f),
				D2D1::Point2F(start_point.x, start_point.y + specs_.line_thickness_fill * 2.f),
				p_brush_fill_, specs_.line_thickness_fill / 2.f);

			// draw end dot
			D2D1_ELLIPSE dot{ end_point,
				specs_.line_thickness_fill * 2.f,
				specs_.line_thickness_fill * 2.f };

			p_render_target->FillEllipse(dot, p_brush_fill_);

			// create a text layout
			std::string text = roundoff::tostr<char>(specs_.percentage, specs_.precision) + "%";
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text).c_str(),
				(UINT32)text.length(), p_text_format_, rect_ellipse_.right - rect_ellipse_.left,
				rect_ellipse_.bottom - rect_ellipse_.top, &p_text_layout_);

			if (SUCCEEDED(hr)) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_ellipse_.left, rect_ellipse_.top },
					p_text_layout_, is_enabled_ ?
					p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&p_text_layout_);

			return rect_;
		}

		void widgets_impl::progress_indicator::on_click() {
			if (specs_.on_click)
				specs_.on_click();
		}

		widgets::progress_indicator::progress_indicator_specs&
			widgets_impl::progress_indicator::specs() { return specs_; }
	}
}
