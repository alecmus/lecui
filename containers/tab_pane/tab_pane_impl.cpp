/*
** tab_pane_impl.cpp - tab_pane_impl implementation
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

#include "tab_pane_impl.h"
#include "../../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::tab_pane::tab_pane(const std::string& page_alias,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_tabs_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			margin_(12.f),
			tab_gap_(margin_ / 1.25f),
			tab_height_(25.f),
			bar_height_(2.f),
			rect_tabs_({ 0.f, 0.f, 0.f, 0.f }),
			rect_client_area_({ 0.f, 0.f, 0.f, 0.f }),
			rect_tab_pane_({ 0.f, 0.f, 0.f, 0.f }) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::tab_pane::~tab_pane() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::tab_pane::type() {
			return lecui::widgets_impl::widget_type::tab_pane;
		}

		HRESULT widgets_impl::tab_pane::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			is_static_ = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
					&p_brush_);
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_tabs),
					&p_brush_tabs_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);
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

		void widgets_impl::tab_pane::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_tabs_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::tab_pane::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			rect_tab_pane_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_tab_pane_.left -= offset.x;
			rect_tab_pane_.right -= offset.x;
			rect_tab_pane_.top -= offset.y;
			rect_tab_pane_.bottom -= offset.y;

			rect_tabs_ = rect_tab_pane_;
			rect_tabs_.bottom = rect_tabs_.top + tab_height_ + bar_height_;

			// only make the tab area respond to hit testing, even though for scroll bar at form level
			// we need to return the entire region through rect_tab_pane_
			rect_ = rect_tabs_;

			rect_client_area_ = rect_tab_pane_;
			rect_client_area_.top = rect_tabs_.bottom;

			if (!render || !visible_)
				return rect_tab_pane_;

			D2D1_ROUNDED_RECT rounded_rect{ rect_client_area_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			if (specs_.is_filled)
				p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ?
					p_brush_fill_ : p_brush_disabled_);

			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			auto measure_string = [](IDWriteFactory* p_directwrite_factory_,
				IDWriteTextFormat* p_text_format_, IDWriteTextLayout* p_text_layout_,
				const std::string& text_, float max_width, float max_height) {
					D2D1_SIZE_F minSize = { 0.f, 0.f };

					// create a text layout
					HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
						(UINT32)text_.length(), p_text_format_, max_width, max_height, &p_text_layout_);

					// measure minimum width required
					if (SUCCEEDED(hr)) {
						DWRITE_TEXT_METRICS textMetrics;
						p_text_layout_->GetMetrics(&textMetrics);
						minSize.width = textMetrics.width;
						minSize.height = textMetrics.height;
					}

					// release the text layout
					safe_release(&p_text_layout_);

					return minSize;
			};

			// draw tabs rectangle
			rounded_rect = { rect_tabs_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_tabs_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			// draw the tab text
			D2D1_RECT_F rect_current_tab_ = rect_tabs_;
			rect_current_tab_.bottom -= bar_height_;
			rect_current_tab_.right = rect_current_tab_.left;
			for (const auto& page : p_tabs_) {
				// calculate tab rect
				rect_current_tab_.left = rect_current_tab_.right;
				rect_current_tab_.right = rect_client_area_.right;

				// calculate minimum width
				const auto min_size = measure_string(p_directwrite_factory_, p_text_format_,
					p_text_layout_, page.first, rect_current_tab_.right - rect_current_tab_.left,
					rect_current_tab_.bottom - rect_current_tab_.top);

				rect_current_tab_.right = rect_current_tab_.left + min_size.width + 2.f * margin_;

				D2D1_RECT_F rect_text_ = rect_current_tab_;

				if (rect_text_.right > rect_tab_pane_.right)
					break;

				// capture current tab
				p_tab_rects_[page.first];	// add if it doesn't yet exist

				{
					D2D1_RECT_F rect = rect_current_tab_;

					// for aesthetics and to allow switching when moving over from one tab to the other
					rect.left += tab_gap_;
					rect.right -= tab_gap_;

					p_tab_rects_.at(page.first) = rect;
				}

				if (!is_static_ && is_enabled_ && hit_) {
					D2D1_RECT_F rect = rect_current_tab_;

					rect.left += tab_gap_;
					rect.right -= tab_gap_;

					scale_RECT(rect, dpi_scale_);

					if (point_.x >= rect.left && point_.x <= rect.right &&
						point_.y >= rect.top && point_.y <= rect.bottom) {
						if (!is_static_ && is_enabled_ && hit_) {
							if (pressed_)
								current_tab_ = page.first;	// it's a tab, don't wait for a click!

							// move text up a little (visual effect)
							rect_text_.bottom -= 5.f;
						}
					}
				}

				// create a text layout
				HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(page.first).c_str(),
					(UINT32)page.first.length(), p_text_format_, rect_text_.right - rect_text_.left,
					rect_text_.bottom - rect_text_.top, &p_text_layout_);

				if (SUCCEEDED(hr)) {
					// draw the text layout
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
						p_text_layout_, is_enabled_ ?
						p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}

				// release the text layout
				safe_release(&p_text_layout_);
			}

			// draw the selected tab bar
			for (const auto& it : p_tab_rects_) {
				if ((it.second.right + tab_gap_) > rect_tab_pane_.right)
					break;

				if (it.first == current_tab_) {
					D2D1_RECT_F rect_bar_ = it.second;
					rect_bar_.top = rect_bar_.bottom;
					rect_bar_.bottom = rect_bar_.bottom += bar_height_;

					p_render_target->FillRectangle(&rect_bar_, p_brush_selected_);
					break;
				}
			}

			return rect_tab_pane_;
		}

		void widgets_impl::tab_pane::on_click() {
			for (auto& it : p_tab_rects_) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					current_tab_ = it.first;
			}
		}

		containers::tab_pane_specs&
			widgets_impl::tab_pane::specs() { return specs_; }

		const D2D1_RECT_F& widgets_impl::tab_pane::client_area() {
			return rect_client_area_;
		}

		const D2D1_RECT_F& widgets_impl::tab_pane::tab_pane_area() {
			return rect_tab_pane_;
		}

		float widgets_impl::tab_pane::caption_bar_height() {
			return (tab_height_ + bar_height_);
		}

		bool widgets_impl::tab_pane::contains() {
			for (const auto& it : p_tab_rects_) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					return true;
			}

			return false;
		}
	}
}
