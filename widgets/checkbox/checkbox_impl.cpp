/*
** checkbox_impl.cpp - checkbox_impl implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "checkbox_impl.h"

namespace liblec {
	namespace lecui {
		widgets::checkbox_impl::checkbox_impl(containers::page& page,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			p_brush_(nullptr),
			p_brush_check_(nullptr),
			p_brush_border_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_direct2d_factory_(p_direct2d_factory),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr) {}

		widgets::checkbox_impl::~checkbox_impl() { discard_resources(); }

		widgets::widget_type
			widgets::checkbox_impl::type() {
			return lecui::widgets::widget_type::checkbox;
		}

		HRESULT widgets::checkbox_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().check == nullptr && specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_check),
					&p_brush_check_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
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
				p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
				make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets::checkbox_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_check_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets::checkbox_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			auto rect_checkbox_ = rect_;
			const auto side = smallest(rect_checkbox_.right - rect_checkbox_.left,
				rect_checkbox_.bottom - rect_checkbox_.top);
			rect_checkbox_.right = rect_checkbox_.left + side;
			rect_checkbox_.bottom = rect_checkbox_.top + side;
			pos_rect(rect_, rect_checkbox_, 0.f, 50.f);

			// draw rectangles
			D2D1_ROUNDED_RECT rounded_rect{ rect_checkbox_,
				side / 4.f, side / 4.f };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_static_ ? p_brush_fill_ :
				hit_ && pressed_ ? p_brush_fill_ :
				hit_ ? p_brush_hot_ :
				p_brush_fill_);

			p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
				p_brush_border_, specs_.border);

			if (!is_static_ && is_enabled_ && selected_)
				p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
					p_brush_selected_, pressed_ ? specs_.border * 3.5f : specs_.border * 2.f);

			// draw checkbox contents
			std::string text;
			switch (specs_.status) {
			case widgets::checkbox::checkbox_specs::checkbox_status::checked: {
				text = specs_.text;
				HRESULT hr = S_OK;
				ID2D1PathGeometry* p_checkbox_geometry = nullptr;
				hr = p_direct2d_factory_->CreatePathGeometry(&p_checkbox_geometry);

				if (SUCCEEDED(hr)) {
					ID2D1GeometrySink* p_sink = nullptr;
					hr = p_checkbox_geometry->Open(&p_sink);
					if (SUCCEEDED(hr)) {
						p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);
						p_sink->BeginFigure(
							D2D1::Point2F(rect_checkbox_.left + (.1f * side), rect_checkbox_.top + (.5f * side)),
							D2D1_FIGURE_BEGIN_FILLED
							);
						D2D1_POINT_2F points[] = {
						   D2D1::Point2F(rect_checkbox_.left + (.4f * side), rect_checkbox_.top + (.7f * side)),
						   D2D1::Point2F(rect_checkbox_.left + (.9f * side), rect_checkbox_.top + (.1f * side)),
						   D2D1::Point2F(rect_checkbox_.left + (.4f * side), rect_checkbox_.top + (.9f * side)),
						};
						p_sink->AddLines(points, ARRAYSIZE(points));
						p_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
						hr = p_sink->Close();
						safe_release(&p_sink);
					}

					// draw the geometry
					p_render_target->FillGeometry(p_checkbox_geometry, p_brush_check_);
					safe_release(&p_checkbox_geometry);
				}
			} break;
			case widgets::checkbox::checkbox_specs::checkbox_status::unchecked:
				text = specs_.text_unchecked;
				break;
			case widgets::checkbox::checkbox_specs::checkbox_status::indeterminate: {
				text = specs_.text_indeterminate;
				// draw a horizontal line to show indeterminate state
				auto rect_indeterminate = rect_checkbox_;
				rect_indeterminate.left += (.2f * side);
				rect_indeterminate.right -= (.2f * side);
				rect_indeterminate.top += (.44f * side);
				rect_indeterminate.bottom -= (.44f * side);

				p_render_target->FillRectangle(rect_indeterminate, p_brush_check_);
			} break;
			default:
				break;
			}

			auto rect_text_ = rect_;
			rect_text_.left = rect_checkbox_.right + (side / 3.f);

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(
				convert_string(text).c_str(),
				(UINT32)text.length(),
				p_text_format_,
				rect_text_.right - rect_text_.left,
				rect_text_.bottom - rect_text_.top,
				&p_text_layout_);

			if (SUCCEEDED(hr)) {
				DWRITE_TEXT_METRICS textMetrics;
				p_text_layout_->GetMetrics(&textMetrics);

				const auto rect_text_og = rect_text_;
				rect_text_.left += textMetrics.left;
				rect_text_.top += textMetrics.top;
				rect_text_.right = smallest(rect_text_.left + textMetrics.width, rect_text_.right);
				rect_text_.bottom = smallest(rect_text_.top + textMetrics.height, rect_text_.bottom);

				if (render && visible_)
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_og.left, rect_text_og.top },
						p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&p_text_layout_);

			rect_ = rect_checkbox_;
			rect_.right = rect_text_.right;

			return rect_;
		}

		void widgets::checkbox_impl::on_click() {
			switch (specs_.status) {
			case widgets::checkbox::checkbox_specs::checkbox_status::unchecked:
				specs_.status = widgets::checkbox::checkbox_specs::checkbox_status::checked;
				break;
			case widgets::checkbox::checkbox_specs::checkbox_status::checked:
			case widgets::checkbox::checkbox_specs::checkbox_status::indeterminate:
			default:
				specs_.status = widgets::checkbox::checkbox_specs::checkbox_status::unchecked;
				break;
			}

			if (specs_.events().check)
				specs_.events().check(specs_.status);

			if (specs_.events().click)
				specs_.events().click();

			if (specs_.events().action)
				specs_.events().action();
		}

		void widgets::checkbox_impl::on_action() {
			switch (specs_.status) {
			case widgets::checkbox::checkbox_specs::checkbox_status::unchecked:
				specs_.status = widgets::checkbox::checkbox_specs::checkbox_status::checked;
				break;
			case widgets::checkbox::checkbox_specs::checkbox_status::checked:
			case widgets::checkbox::checkbox_specs::checkbox_status::indeterminate:
			default:
				specs_.status = widgets::checkbox::checkbox_specs::checkbox_status::unchecked;
				break;
			}

			if (specs_.events().check)
				specs_.events().check(specs_.status);

			if (specs_.events().action)
				specs_.events().action();
		}

		widgets::checkbox::checkbox_specs&
			widgets::checkbox_impl::specs() { return specs_; }

		widgets::checkbox::checkbox_specs&
			widgets::checkbox_impl::operator()() { return specs(); }
	}
}
