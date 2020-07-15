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
		widgets::tab_pane_impl::tab_pane_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
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
			padding_(5.f),	// used when caption orientation is perpendicular to the tab area
			tab_gap_(10.f),
			tab_height_(25.f),
			tab_height_set_(false),
			bar_height_(2.f),
			rect_tabs_({ 0.f, 0.f, 0.f, 0.f }),
			rect_client_area_({ 0.f, 0.f, 0.f, 0.f }),
			rect_tab_pane_({ 0.f, 0.f, 0.f, 0.f }) {}

		widgets::tab_pane_impl::~tab_pane_impl() { discard_resources(); }

		widgets::widget_type
			widgets::tab_pane_impl::type() {
			return lecui::widgets::widget_type::tab_pane;
		}

		HRESULT widgets::tab_pane_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			is_static_ = false;
			h_cursor_ = get_cursor(specs_.cursor);

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

		void widgets::tab_pane_impl::discard_resources() {
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
			widgets::tab_pane_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (!resources_created_)
				create_resources(p_render_target);

			rect_tab_pane_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_tab_pane_.left -= offset.x;
			rect_tab_pane_.right -= offset.x;
			rect_tab_pane_.top -= offset.y;
			rect_tab_pane_.bottom -= offset.y;

			/// compute rect_tabs_
			rect_tabs_ = rect_tab_pane_;

			switch (specs_.tab_side)
			{
			case containers::tab_pane::side::left:
				rect_tabs_.right = rect_tabs_.left + tab_height_ + bar_height_;
				break;

			case containers::tab_pane::side::right:
				rect_tabs_.left = rect_tabs_.right - (tab_height_ + bar_height_);
				break;

			case containers::tab_pane::side::bottom:
				rect_tabs_.top = rect_tabs_.bottom - (tab_height_ + bar_height_);
				break;

			case containers::tab_pane::side::top:
			default:
				rect_tabs_.bottom = rect_tabs_.top + tab_height_ + bar_height_;
				break;
			}

			/// compute rect_client_area_
			rect_client_area_ = rect_tab_pane_;

			switch (specs_.tab_side)
			{
			case containers::tab_pane::side::left:
				rect_client_area_.left = rect_tabs_.right;
				break;

			case containers::tab_pane::side::right:
				rect_client_area_.right = rect_tabs_.left;
				break;

			case containers::tab_pane::side::bottom:
				rect_client_area_.bottom = rect_tabs_.top;
				break;

			case containers::tab_pane::side::top:
			default:
				rect_client_area_.top = rect_tabs_.bottom;
				break;
			}

			// only make the tab area respond to hit testing, even though for scroll bar at form level
			// we need to return the entire region through rect_tab_pane_
			rect_ = rect_tabs_;

			if (!render || !visible_)
				return rect_tab_pane_;

			/// draw client area background and border
			D2D1_ROUNDED_RECT rounded_rect{ rect_client_area_,
				specs_.corner_radius_x, specs_.corner_radius_y };

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

			/// draw tabs rectangle
			rounded_rect = { rect_tabs_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_tabs_);
			p_render_target->DrawRoundedRectangle(&rounded_rect, is_enabled_ ?
				p_brush_border_ : p_brush_disabled_, specs_.border);

			/// draw the tab text
			D2D1_RECT_F rect_current_tab_ = rect_tabs_;

			switch (specs_.tab_side)
			{
			case containers::tab_pane::side::left:
				rect_current_tab_.right -= bar_height_;
				rect_current_tab_.bottom = rect_current_tab_.top;
				break;

			case containers::tab_pane::side::right:
				rect_current_tab_.left += bar_height_;
				rect_current_tab_.bottom = rect_current_tab_.top;
				break;

			case containers::tab_pane::side::bottom:
				rect_current_tab_.top += bar_height_;
				rect_current_tab_.right = rect_current_tab_.left;
				break;

			case containers::tab_pane::side::top:
			default:
				rect_current_tab_.bottom -= bar_height_;
				rect_current_tab_.right = rect_current_tab_.left;
				break;
			}

			for (const auto& tab_name : tab_order_) {

				D2D1_SIZE_F min_size = { 0.f, 0.f };

				switch (specs_.tab_side)
				{
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					// calculate tab rect
					rect_current_tab_.top = rect_current_tab_.bottom;
					rect_current_tab_.bottom = rect_client_area_.bottom;

					// calculate minimum size (we will rotate the text, so we're factoring this into this computation)
					min_size = measure_string(p_directwrite_factory_, p_text_format_,
						p_text_layout_, tab_name, rect_current_tab_.bottom - rect_current_tab_.top,
						rect_current_tab_.right - rect_current_tab_.left);
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					// calculate tab rect
					rect_current_tab_.left = rect_current_tab_.right;
					rect_current_tab_.right = rect_client_area_.right;

					// calculate minimum size
					min_size = measure_string(p_directwrite_factory_, p_text_format_,
						p_text_layout_, tab_name, rect_current_tab_.right - rect_current_tab_.left,
						rect_current_tab_.bottom - rect_current_tab_.top);
					break;
				}

				D2D1_RECT_F rect_text_ = { 0.f, 0.f, 0.f, 0.f };
				bool excess = false;

				switch (specs_.tab_side)
				{
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					if (specs_.caption_orientation == containers::tab_pane::orientation::vertical) {
						swap(min_size.width, min_size.height);
						rect_current_tab_.bottom = rect_current_tab_.top + min_size.height + 2.f * tab_gap_;
						rect_text_ = rect_current_tab_;
					}
					else {
						rect_current_tab_.bottom = rect_current_tab_.top + 25.f + (2 * tab_gap_);
						rect_text_ = rect_current_tab_;
						rect_text_.left += padding_;
						rect_text_.right = smallest(rect_text_.left + min_size.width, rect_current_tab_.right - 2 * padding_);
					}

					if (rect_text_.bottom > rect_tab_pane_.bottom)
						excess = true;

					rect_text_.top += tab_gap_;
					rect_text_.bottom -= tab_gap_;
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					if (specs_.caption_orientation == containers::tab_pane::orientation::horizontal) {
						rect_current_tab_.right = rect_current_tab_.left + min_size.width + 2.f * tab_gap_;
						rect_text_ = rect_current_tab_;
					}
					else {
						rect_current_tab_.right = rect_current_tab_.left + 25.f + (2 * tab_gap_);
						rect_text_ = rect_current_tab_;
						rect_text_.bottom -= padding_;
						rect_text_.top = largest(rect_text_.bottom - min_size.width, rect_current_tab_.top + 2 * padding_);
					}

					if (rect_text_.right > rect_tab_pane_.right)
						excess = true;

					rect_text_.left += tab_gap_;
					rect_text_.right -= tab_gap_;
					break;
				}

				if (excess)
					break;

				// capture current tab
				p_tab_rects_[tab_name];	// add if it doesn't yet exist

				{
					D2D1_RECT_F rect = rect_current_tab_;

					// for aesthetics and to allow switching when moving over from one tab to the other

					switch (specs_.tab_side)
					{
					case containers::tab_pane::side::left:
					case containers::tab_pane::side::right:
						rect.top += tab_gap_;
						rect.bottom -= tab_gap_;
						break;

					case containers::tab_pane::side::top:
					case containers::tab_pane::side::bottom:
					default:
						rect.left += tab_gap_;
						rect.right -= tab_gap_;
						break;
					}

					p_tab_rects_.at(tab_name) = rect;
				}

				if (!is_static_ && is_enabled_ && hit_) {
					D2D1_RECT_F rect = rect_current_tab_;

					switch (specs_.tab_side)
					{
					case containers::tab_pane::side::left:
					case containers::tab_pane::side::right:
						rect.top += tab_gap_;
						rect.bottom -= tab_gap_;
						break;

					case containers::tab_pane::side::top:
					case containers::tab_pane::side::bottom:
					default:
						rect.left += tab_gap_;
						rect.right -= tab_gap_;
						break;
					}

					scale_RECT(rect, get_dpi_scale());

					if (point_.x >= rect.left && point_.x <= rect.right &&
						point_.y >= rect.top && point_.y <= rect.bottom) {
						if (!is_static_ && is_enabled_ && hit_) {
							if (pressed_)
								current_tab_ = tab_name;	// it's a tab, don't wait for a click!

							// move text a little (visual effect)
							switch (specs_.tab_side)
							{
							case containers::tab_pane::side::left:
								rect_text_.right += 2.5f;
								rect_text_.left += 2.5f;
								break;

							case containers::tab_pane::side::right:
								rect_text_.left += 2.5f;
								rect_text_.right += 2.5f;
								break;

							case containers::tab_pane::side::bottom:
								rect_text_.top += 2.5f;
								rect_text_.bottom += 2.5f;
								break;

							case containers::tab_pane::side::top:
							default:
								rect_text_.bottom -= 2.5f;
								rect_text_.top -= 2.5f;
								break;
							}
						}
					}
				}

				switch (specs_.tab_side)
				{
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right: {
					if (specs_.caption_orientation == containers::tab_pane::orientation::vertical) {
						// rotate the text rectangle about its center

						const D2D1_POINT_2F center =
						{ rect_text_.left + (rect_text_.right - rect_text_.left) / 2.f,
							rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f };

						D2D1_SIZE_F size =
						{ rect_text_.right - rect_text_.left ,
							rect_text_.bottom - rect_text_.top };
						swap(size.width, size.height);

						rect_text_.left = center.x - (size.width / 2.f);
						rect_text_.right = rect_text_.left + size.width;
						rect_text_.top = center.y - (size.height / 2.f);
						rect_text_.bottom = rect_text_.top + size.height;

						D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(90.f, center);

						D2D1_MATRIX_3X2_F old_transform;
						p_render_target->GetTransform(&old_transform);

						p_render_target->SetTransform(rotation);

						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), p_text_format_, rect_text_.right - rect_text_.left,
							rect_text_.bottom - rect_text_.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						p_render_target->SetTransform(old_transform);
					}
					else {
						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), p_text_format_, rect_text_.right - rect_text_.left,
							rect_text_.bottom - rect_text_.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}
					}
				}
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom: {
				default:
					if (specs_.caption_orientation == containers::tab_pane::orientation::horizontal) {
						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), p_text_format_, rect_text_.right - rect_text_.left,
							rect_text_.bottom - rect_text_.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}
					}
					else {
						// rotate the text rectangle about its center

						const D2D1_POINT_2F center =
						{ rect_text_.left + (rect_text_.right - rect_text_.left) / 2.f,
							rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f };

						D2D1_SIZE_F size =
						{ rect_text_.right - rect_text_.left ,
							rect_text_.bottom - rect_text_.top };
						swap(size.width, size.height);

						rect_text_.left = center.x - (size.width / 2.f);
						rect_text_.right = rect_text_.left + size.width;
						rect_text_.top = center.y - (size.height / 2.f);
						rect_text_.bottom = rect_text_.top + size.height;

						D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(-90.f, center);

						D2D1_MATRIX_3X2_F old_transform;
						p_render_target->GetTransform(&old_transform);

						p_render_target->SetTransform(rotation);

						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), p_text_format_, rect_text_.right - rect_text_.left,
							rect_text_.bottom - rect_text_.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						p_render_target->SetTransform(old_transform);
					}
				}
					break;
				}

				// release the text layout
				safe_release(&p_text_layout_);
			}

			// draw the selected tab bar
			for (const auto& it : p_tab_rects_) {
				bool excess = false;
				switch (specs_.tab_side)
				{
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					if ((it.second.bottom + tab_gap_) > rect_tab_pane_.bottom)
						excess = true;
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					if ((it.second.right + tab_gap_) > rect_tab_pane_.right)
						excess = true;
					break;
				}

				if (excess)
					break;

				if (it.first == current_tab_) {
					D2D1_RECT_F rect_bar_ = it.second;

					switch (specs_.tab_side)
					{
					case containers::tab_pane::side::left:
						rect_bar_.left = rect_bar_.right;
						rect_bar_.right = rect_bar_.right += bar_height_;
						break;

					case containers::tab_pane::side::right:
						rect_bar_.right = rect_bar_.left;
						rect_bar_.left = rect_bar_.left -= bar_height_;
						break;

					case containers::tab_pane::side::bottom:
						rect_bar_.bottom = rect_bar_.top;
						rect_bar_.top = rect_bar_.top -= bar_height_;
						break;

					case containers::tab_pane::side::top:
					default:
						rect_bar_.top = rect_bar_.bottom;
						rect_bar_.bottom = rect_bar_.bottom += bar_height_;
						break;
					}

					p_render_target->FillRectangle(&rect_bar_, p_brush_selected_);
					break;
				}
			}

			return rect_tab_pane_;
		}

		void widgets::tab_pane_impl::on_click() {
			for (auto& it : p_tab_rects_) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, get_dpi_scale());

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					current_tab_ = it.first;
			}
		}

		containers::tab_pane::tab_pane_specs&
			widgets::tab_pane_impl::specs() { return specs_; }

		containers::tab_pane::tab_pane_specs&
			widgets::tab_pane_impl::operator()() { return specs(); }

		const D2D1_RECT_F& widgets::tab_pane_impl::client_area() {
			return rect_client_area_;
		}

		const D2D1_RECT_F& widgets::tab_pane_impl::tab_pane_area() {
			return rect_tab_pane_;
		}

		float widgets::tab_pane_impl::caption_bar_height() {
			return (tab_height_ + bar_height_);
		}

		float widgets::tab_pane_impl::tab_height() {
			return tab_height_;
		}

		float widgets::tab_pane_impl::bar_height() {
			return bar_height_;
		}

		float widgets::tab_pane_impl::padding() {
			return padding_;
		}

		float widgets::tab_pane_impl::margin() {
			return margin_;
		}

		void widgets::tab_pane_impl::set_tab_height(const float height) {
			tab_height_set_ = true;
			tab_height_ = height;
		}

		bool widgets::tab_pane_impl::tab_height_set() {
			return tab_height_set_;
		}

		void widgets::tab_pane_impl::close_tab(const std::string& tab_name) {
			try {
				// erase from tabs (all widgets within this tab are deleted immediately)
				p_tabs_.erase(tab_name);

				// erase from tab rects
				p_tab_rects_.erase(tab_name);

				// skip in tabs order
				std::vector<std::string> temp_tab_order;
				for (auto& tab : tab_order_) {
					if (tab != tab_name)
						temp_tab_order.push_back(tab);
				}
				tab_order_ = temp_tab_order;
			}
			catch (const std::exception&) {}
		}

		bool widgets::tab_pane_impl::contains() {
			for (const auto& it : p_tab_rects_) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, get_dpi_scale());

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					return true;
			}

			return false;
		}
	}
}
