/*
** button_impl.cpp - button_impl implementation
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

#include "button_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::button::button(const std::string& page_alias,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_border_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::button::~button() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::button::type() {
			return lecui::widgets_impl::widget_type::button;
		}

		HRESULT widgets_impl::button::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
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
				p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::button::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::button::render(ID2D1HwndRenderTarget* p_render_target,
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

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_static_ ? p_brush_fill_ :
				hit_ && pressed_ ? p_brush_fill_ :
				hit_ ? p_brush_hot_ :
				p_brush_fill_);

			p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
				p_brush_border_, .5f);

			if (!is_static_ && is_enabled_ && selected_)
				p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
					p_brush_selected_, pressed_ ? 1.75f : 1.f);

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(specs_.text).c_str(),
				(UINT32)specs_.text.length(), p_text_format_, rect_.right - rect_.left,
				rect_.bottom - rect_.top, &p_text_layout_);

			if (SUCCEEDED(hr)) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_.left, rect_.top },
					p_text_layout_, is_enabled_ ?
					p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			// release the text layout
			safe_release(&p_text_layout_);

			return rect_;
		}

		void widgets_impl::button::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::button::button_specs&
			widgets_impl::button::specs() { return specs_; }

		widgets::button::button_specs&
			widgets_impl::button::operator()() { return specs(); }
	}
}
