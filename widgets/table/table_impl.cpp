/*
** table_impl.cpp - table_impl implementation
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

#include "table_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::table::table(const std::string& page_alias,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_scrollbar_border_(nullptr),
			p_brush_text_header_(nullptr),
			p_brush_fill_header_(nullptr),
			p_brush_fill_alternate_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_brush_border_(nullptr),
			p_brush_dropdown_hot_(nullptr),
			p_brush_menu_(nullptr),
			p_brush_grid_(nullptr),
			p_brush_row_hot_(nullptr),
			p_brush_row_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			scrollbar_thickness_(10),

			p_brush_scrollbar_(nullptr),
			p_brush_scrollbar_hot_(nullptr),
			p_brush_scrollbar_hot_pressed_(nullptr),
			p_brush_scrollbar_background_(nullptr),

			v_scrollbar_hit_(false),
			h_scrollbar_hit_(false),
			v_scrollbar_pressed_(false),
			h_scrollbar_pressed_(false),
			v_displacement_(0.f),
			h_displacement_(0.f),
			v_displacement_previous_(0.f),
			h_displacement_previous_(0.f),
			row_height_(20.f),
			margin_(row_height_ / 4.f),
			rect_header_({ 0.f, 0.f, 0.f, 0.f }),
			rectA_({ 0.f, 0.f, 0.f, 0.f }),
			rectB_({ 0.f, 0.f, 0.f, 0.f }),
			v_scrollbar_visible_(false),
			h_scrollbar_visible_(false),
			change_in_height_previous_(0.f),
			change_in_width_previous_(0.f),
			last_selected_(0UL),
			book_on_selection_(false) {
			page_alias_ = page_alias;
			alias_ = alias_;

			widgets::scrollbar_specs bar;

			color_scrollbar_ = bar.color_fill;
			color_scrollbar_hot_ = bar.color_hot;
			color_scrollbar_hot_pressed_ = bar.color_hot_pressed;
			color_scrollbar_background_ = bar.color_background;
			color_scrollbar_border_ = bar.color_scrollbar_border;
		}

		widgets_impl::table::~table() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::table::type() {
			return lecui::widgets_impl::widget_type::table;
		}

		HRESULT widgets_impl::table::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_border_),
					&p_brush_scrollbar_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text_header),
					&p_brush_text_header_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill_header),
					&p_brush_fill_header_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill_alternate),
					&p_brush_fill_alternate_);
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
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown_hot),
					&p_brush_dropdown_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_menu),
					&p_brush_menu_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_grid),
					&p_brush_grid_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_row_hot),
					&p_brush_row_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_row_selected),
					&p_brush_row_selected_);

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_),
					&p_brush_scrollbar_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_hot_),
					&p_brush_scrollbar_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_hot_pressed_),
					&p_brush_scrollbar_hot_pressed_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(color_scrollbar_background_),
					&p_brush_scrollbar_background_);

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
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::table::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_scrollbar_border_);
			safe_release(&p_brush_text_header_);
			safe_release(&p_brush_fill_header_);
			safe_release(&p_brush_fill_alternate_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_dropdown_hot_);
			safe_release(&p_brush_menu_);
			safe_release(&p_brush_grid_);
			safe_release(&p_text_format_);

			safe_release(&p_brush_scrollbar_);
			safe_release(&p_brush_scrollbar_hot_);
			safe_release(&p_brush_scrollbar_hot_pressed_);
			safe_release(&p_brush_scrollbar_background_);

		}

		D2D1_RECT_F&
			widgets_impl::table::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			const int precision = 3;	// to prevent false-positives (4 is enough, 3 is a failsafe)
			auto equal = [&](const D2D1_RECT_F& rect_1, const D2D1_RECT_F& rect_2) {
				if (roundoff::tof(rect_1.left, precision) == roundoff::tof(rect_2.left, precision) &&
					roundoff::tof(rect_1.top, precision) == roundoff::tof(rect_2.top, precision) &&
					roundoff::tof(rect_1.right, precision) == roundoff::tof(rect_2.right, precision) &&
					roundoff::tof(rect_1.bottom, precision) == roundoff::tof(rect_2.bottom, precision))
					return true;
				else
					return false;
			};

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			// step1: clip widget area
			auto_clip clip(render, p_render_target, rect_, 1.f);

			// step2: detecting changes in dimensions and adjust the displacement accordingly
			{
				// calculate change in dimensions
				const float change_in_width_ = (rect_.right - rect_.left) -
					static_cast<float>(specs_.rect.right - specs_.rect.left);
				const float change_in_height_ = (rect_.bottom - rect_.top) -
					static_cast<float>(specs_.rect.bottom - specs_.rect.top);

				// calculate adjustments to make to displacement
				const float height_adjustment = change_in_height_ - change_in_height_previous_;
				change_in_height_previous_ = change_in_height_;
				const float width_adjustment = change_in_width_ - change_in_width_previous_;
				change_in_width_previous_ = change_in_width_;

				// apply height adjustment
				if (v_displacement_ != 0.f) {
					v_displacement_ += height_adjustment;
					v_displacement_previous_ += height_adjustment;
				}

				// apply width adjustment
				if (h_displacement_ != 0.f) {
					h_displacement_ += width_adjustment;
					h_displacement_previous_ += width_adjustment;
				}
			}

			// step3: draw widget background
			{
				D2D1_ROUNDED_RECT rounded_rect{ rect_,
					specs_.corner_radius_x, specs_.corner_radius_y };

				if (render && visible_)
					p_render_target->FillRoundedRectangle(&rounded_rect, !is_enabled_ ?
						p_brush_disabled_ : p_brush_fill_);
			}

			// step4: draw header background
			{
				rect_header_ = rect_;
				rect_header_.bottom = rect_header_.top + row_height_;

				D2D1_ROUNDED_RECT rounded_rect{ rect_header_,
					specs_.corner_radius_x, specs_.corner_radius_y };

				if (render && visible_)
					p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_fill_header_);
			}

			// step5: define rectB_ (the table area)
			rectB_ = { rect_.left, rect_header_.bottom, rect_.right, rect_.bottom };

			float table_width = 0.f;
			for (const auto& it : specs_.columns) table_width += static_cast<float>(it.width);
			float table_height = row_height_ * specs_.data.size();

			// step6: define rectC_ for scroll bars

			// step6a: define rectC_v_
			auto rectC_v_ = rectB_;
			rectC_v_.left = rectC_v_.right - scrollbar_thickness_;
			rectC_v_.top += scrollbar_thickness_;
			rectC_v_.bottom -= scrollbar_thickness_;

			// step6b: define rectC_h_
			auto rectC_h_ = rectB_;
			rectC_h_.top = rectC_h_.bottom - scrollbar_thickness_;
			rectC_h_.left += scrollbar_thickness_;
			rectC_h_.right -= scrollbar_thickness_;

			// step7: handle scroll bars

			// step7a: handle vertical scroll bar
			if (v_scrollbar_visible_) {
				auto rect = rectC_v_;
				scale_RECT(rect, dpi_scale_);

				// handle hit status
				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					v_scrollbar_hit_ = true;
				else
					v_scrollbar_hit_ = false;

				// handle pressed status
				const auto before = v_scrollbar_pressed_;

				if (pressed_ &&
					point_on_press_.x >= rect.left && point_on_press_.x <= rect.right &&
					point_on_press_.y >= rect.top && point_on_press_.y <= rect.bottom)
					v_scrollbar_pressed_ = true;
				else
					v_scrollbar_pressed_ = false;

				const auto after = v_scrollbar_pressed_;

				if (before != after && !v_scrollbar_pressed_)	// scroll bar released just now
					v_displacement_previous_ = v_displacement_;

				// calculate the scale factor for amplifying the movement of the scroll bar
				const float height_A = table_height;
				const float height_C = rectC_v_.bottom - rectC_v_.top;
				const float scale_factor = height_C != 0.f ? height_A / height_C : 1.f;

				if (v_scrollbar_pressed_) {
					// compute the displacement according the the mouse movement
					v_displacement_ = -scale_factor * (point_.y - point_on_press_.y) / dpi_scale_;
					v_displacement_ += v_displacement_previous_;
				}
			}

			// step7b: handle horizontal scroll bar
			if (h_scrollbar_visible_) {
				auto rect = rectC_h_;
				scale_RECT(rect, dpi_scale_);

				// handle hit status
				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					h_scrollbar_hit_ = true;
				else
					h_scrollbar_hit_ = false;

				// handle pressed status
				const auto before = h_scrollbar_pressed_;

				if (pressed_ &&
					point_on_press_.x >= rect.left && point_on_press_.x <= rect.right &&
					point_on_press_.y >= rect.top && point_on_press_.y <= rect.bottom)
					h_scrollbar_pressed_ = true;
				else
					h_scrollbar_pressed_ = false;

				const auto after = h_scrollbar_pressed_;

				if (before != after && !h_scrollbar_pressed_)	// scroll bar released
					h_displacement_previous_ = h_displacement_;

				// calculate the scale factor for amplifying the movement of the scroll bar
				const float width_A = table_width;
				const float width_C = rectC_h_.right - rectC_h_.left;
				const float scale_factor = width_C != 0.f ? width_A / width_C : 1.f;

				if (h_scrollbar_pressed_) {
					// compute the displacement according the the mouse movement
					h_displacement_ = -scale_factor * (point_.x - point_on_press_.x) / dpi_scale_;
					h_displacement_ += h_displacement_previous_;
				}
			}

			// step8: adjust h_displacement_ and v_displacement_, if necessary

			/// Algorithm:
			/// 
			/// rectA_.left is not allowed to get greater than rectB_.left
			/// rectA_.top is not allowed to get greater than rectB_.top
			/// rectA_.right is only allowed to get smaller than rectB_.right
			/// if rectA_.left is equal to rectB_.left
			/// rectA_.bottom is only allowed to get smaller than rectB_.bottom
			/// if rectA_.top is equal to rectB_.top
			h_displacement_ = smallest(0.f,
				largest(h_displacement_, (rectB_.right - rectB_.left) - table_width));
			v_displacement_ = smallest(0.f,
				largest(v_displacement_, (rectB_.bottom - rectB_.top) - table_height));

			// step9: define rectA_ (the area containing all the table contents)

			// if the table cannot fit in rectB_ this area can be navigated using the scroll bars

			rectA_ = rectB_;
			rectA_.left = rectB_.left + h_displacement_;
			rectA_.top = rectB_.top + v_displacement_;
			rectA_.right = rectA_.left + table_width;
			rectA_.bottom = rectA_.top + table_height;

			// step10: draw header
			{
				auto rect_header_cell = rect_header_;
				rect_header_cell.left = rectA_.left;
				rect_header_cell.right = rect_header_cell.left;

				for (const auto& it : specs_.columns) {
					rect_header_cell.left = rect_header_cell.right;
					rect_header_cell.right = rect_header_cell.left + static_cast<float>(it.width);

					auto rect_text = rect_header_cell;
					rect_text.left += margin_;
					rect_text.right -= margin_;

					auto hr = p_directwrite_factory_->CreateTextLayout(
						convert_string(it.name).c_str(),
						(UINT32)it.name.length(),
						p_text_format_,
						rect_text.right - rect_text.left,
						rect_text.bottom - rect_text.top,
						&p_text_layout_);

					DWRITE_TEXT_RANGE text_range = { 0, static_cast<UINT32>(it.name.length()) };

					p_text_layout_->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, text_range);

					if (SUCCEEDED(hr) && render && visible_)
						p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text.left, rect_text.top },
							p_text_layout_, p_brush_text_header_, D2D1_DRAW_TEXT_OPTIONS_CLIP);

					safe_release(&p_text_layout_);
				}
			}

			// step11: draw table
			{
				hot_spots_.clear();

				// step11a: clip to table area
				auto_clip clip(render, p_render_target, rectB_, 0.f);

				// step11b: figure out which rows are hidden from view and exclude them from the rendering
				// this gives a major performance boost when dealing with very large tables
				unsigned long hidden_above = (rectB_.top > rectA_.top) ?
					(static_cast<unsigned long>((rectB_.top - rectA_.top) / row_height_)) : 0;
				unsigned long hidden_below = (rectA_.bottom > rectB_.bottom) ?
					(static_cast<unsigned long>((rectA_.bottom - rectB_.bottom) / row_height_)) : 0;

				// step11c: do the drawing
				{
					auto rect_row = rectA_;
					rect_row.bottom = rect_row.top + row_height_ * (hidden_above);

					for (unsigned long row_number = hidden_above;
						row_number < (specs_.data.size() - hidden_below); row_number++) {
						rect_row.top = rect_row.bottom;
						rect_row.bottom = rect_row.top + row_height_;

						bool selected = std::find(specs_.selected.begin(), specs_.selected.end(),
							row_number) != specs_.selected.end();
						bool hot = false;

						{
							auto rect = rect_row;
							rect.left = rectB_.left;
							rect.right = rectB_.right;

							if (!h_scrollbar_hit_ &&
								!v_scrollbar_hit_ &&
								!h_scrollbar_pressed_ &&
								!v_scrollbar_pressed_)
								hot_spots_[row_number] = rect;

							scale_RECT(rect, dpi_scale_);

							// handle hit status
							if (!h_scrollbar_hit_ &&
								!v_scrollbar_hit_ &&
								!h_scrollbar_pressed_ &&
								!v_scrollbar_pressed_ &&
								point_.x >= rect.left && point_.x <= rect.right &&
								point_.y >= rect.top && point_.y <= rect.bottom)
								hot = true;
							else
								hot = false;
						}

						if (selected) {
							auto rect = rect_row;
							rect.left = rectB_.left;
							rect.right = rectB_.right;

							D2D1_ROUNDED_RECT rounded_rect{ rect,
								specs_.corner_radius_x, specs_.corner_radius_y };

							if (render && visible_)
								p_render_target->FillRoundedRectangle(&rounded_rect,
									p_brush_row_selected_);
						}
						else
							if (hot) {
								auto rect = rect_row;
								rect.left = rectB_.left;
								rect.right = rectB_.right;

								D2D1_ROUNDED_RECT rounded_rect{ rect,
									specs_.corner_radius_x, specs_.corner_radius_y };

								if (render && visible_)
									p_render_target->FillRoundedRectangle(&rounded_rect,
										p_brush_row_hot_);
							}
							else
								if (row_number % 2 == 0) {
									// step11ci: draw alternate background
									auto rect = rect_row;
									rect.left = rectB_.left;
									rect.right = rectB_.right;

									D2D1_ROUNDED_RECT rounded_rect{ rect,
										specs_.corner_radius_x, specs_.corner_radius_y };

									if (render && visible_)
										p_render_target->FillRoundedRectangle(&rounded_rect,
											p_brush_fill_alternate_);
								}

						// step11cii: draw row
						auto rect_cell = rect_row;
						rect_cell.right = rect_cell.left;

						for (const auto& it : specs_.columns) {
							rect_cell.left = rect_cell.right;
							rect_cell.right = rect_cell.left + static_cast<float>(it.width);

							auto rect_text = rect_cell;
							rect_text.left += margin_;
							rect_text.right -= margin_;

							try {
								std::string text = specs_.data.at(row_number).at(it.name);

								// create a text layout
								HRESULT hr = p_directwrite_factory_->CreateTextLayout(
									convert_string(text).c_str(),
									(UINT32)text.length(),
									p_text_format_,
									rect_text.right - rect_text.left,
									rect_text.bottom - rect_text.top,
									&p_text_layout_);

								if (SUCCEEDED(hr) && render && visible_) {
									// draw the text layout
									p_render_target->DrawTextLayout(
										D2D1_POINT_2F{ rect_text.left, rect_text.top },
										p_text_layout_, selected ?
										p_brush_menu_ : p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
								}

								// release the text layout
								safe_release(&p_text_layout_);
							}
							catch (const std::exception&) {}
						}
					}
				}

				// step12: draw grid
				if (render && visible_) {
					const float line_width = .5f;

					// step12a: draw horizontal lines
					{
						auto rect_row = rectA_;
						rect_row.bottom = rect_row.top + row_height_ * (hidden_above);

						do {
							rect_row.top = rect_row.bottom;
							rect_row.bottom = rect_row.top + row_height_;

							if (rect_row.bottom > rectB_.bottom)
								break;

							p_render_target->DrawLine(D2D1::Point2F(rectB_.left, rect_row.bottom),
								D2D1::Point2F(rectB_.right, rect_row.bottom), p_brush_grid_, line_width);
						} while (true);
					}

					// step12b: draw vertical lines
					{
						auto x = rectA_.left;
						for (auto& it : specs_.columns) {
							x += it.width;

							p_render_target->DrawLine(D2D1::Point2F(x, rectB_.top),
								D2D1::Point2F(x, rectB_.bottom), p_brush_grid_, line_width);
						}
					}
				}
			}

			// step13: draw scroll bars

			// step13a: draw vertical scroll bar
			{
				D2D1_RECT_F rectD_ = { 0.f, 0.f, 0.f, 0.f };
				position_v_scrollbar(rectA_, rectB_, rectC_v_, rectD_);

				if (!equal(rectC_v_, rectD_) &&
					!(roundoff::tof((rectD_.bottom - rectD_.top), precision) >=
						roundoff::tof((rectC_v_.bottom - rectC_v_.top), precision))) {
					auto corner_radius = smallest((rectD_.right - rectD_.left) / 3.f,
						(rectD_.bottom - rectD_.top) / 3.f);

					// scroll area
					D2D1_ROUNDED_RECT rounded_rectC{ rectC_v_, corner_radius, corner_radius };

					if (render && visible_)
						p_render_target->FillRoundedRectangle(&rounded_rectC,
							p_brush_scrollbar_background_);

					// scroll bar
					auto rect_scroll_bar = rectD_;
					const float scroll_bar_margin = 2.f;
					rect_scroll_bar.left += scroll_bar_margin;
					rect_scroll_bar.top += scroll_bar_margin;
					rect_scroll_bar.right -= scroll_bar_margin;
					rect_scroll_bar.bottom -= scroll_bar_margin;
					corner_radius = smallest((rect_scroll_bar.bottom - rect_scroll_bar.top) / 3.f,
						(rect_scroll_bar.right - rect_scroll_bar.left) / 3.f);
					D2D1_ROUNDED_RECT rounded_rectD{ rect_scroll_bar, corner_radius, corner_radius };

					if (render && visible_) {
						p_render_target->FillRoundedRectangle(&rounded_rectD,
							v_scrollbar_pressed_ ? p_brush_scrollbar_hot_pressed_ : (v_scrollbar_hit_ ?
								p_brush_scrollbar_hot_ : p_brush_scrollbar_));
						p_render_target->DrawRoundedRectangle(&rounded_rectD, p_brush_scrollbar_border_);
					}

					v_scrollbar_visible_ = true;
				}
				else
					v_scrollbar_visible_ = false;
			}

			// step13b: draw horizontal scroll bar
			{
				D2D1_RECT_F rectD_ = { 0.f, 0.f, 0.f, 0.f };
				position_h_scrollbar(rectA_, rectB_, rectC_h_, rectD_);

				if (!equal(rectC_h_, rectD_) &&
					!(roundoff::tof((rectD_.right - rectD_.left), precision) >=
						roundoff::tof((rectC_h_.right - rectC_h_.left), precision))) {
					auto corner_radius = smallest((rectD_.bottom - rectD_.top) / 3.f,
						(rectD_.right - rectD_.left) / 3.f);

					// scroll area
					D2D1_ROUNDED_RECT rounded_rectC{ rectC_h_, corner_radius, corner_radius };

					if (render && visible_)
						p_render_target->FillRoundedRectangle(&rounded_rectC,
							p_brush_scrollbar_background_);

					// scroll bar
					auto rect_scroll_bar = rectD_;
					const float scroll_bar_margin = 2.f;
					rect_scroll_bar.left += scroll_bar_margin;
					rect_scroll_bar.top += scroll_bar_margin;
					rect_scroll_bar.right -= scroll_bar_margin;
					rect_scroll_bar.bottom -= scroll_bar_margin;
					corner_radius = smallest((rect_scroll_bar.bottom - rect_scroll_bar.top) / 3.f,
						(rect_scroll_bar.right - rect_scroll_bar.left) / 3.f);
					D2D1_ROUNDED_RECT rounded_rectD{ rect_scroll_bar, corner_radius, corner_radius };

					if (render && visible_) {
						p_render_target->FillRoundedRectangle(&rounded_rectD,
							h_scrollbar_pressed_ ? p_brush_scrollbar_hot_pressed_ : (h_scrollbar_hit_ ?
								p_brush_scrollbar_hot_ : p_brush_scrollbar_));
						p_render_target->DrawRoundedRectangle(&rounded_rectD, p_brush_scrollbar_border_);
					}

					h_scrollbar_visible_ = true;
				}
				else
					h_scrollbar_visible_ = false;
			}

			// draw border
			if (render && visible_) {
				D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

				p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
					p_brush_border_, .5f);
			}

			return rect_;
		}

		void widgets_impl::table::on_click() {
			if (!h_scrollbar_pressed_ && !v_scrollbar_pressed_) {
				if (book_on_selection_) {
					if (specs_.on_selection)
						on_selection();

					book_on_selection_ = false;
					return;
				}

				bool ctrl_pressed = (GetKeyState(VK_CONTROL) & 0x8000);
				bool shift_pressed = (GetKeyState(VK_SHIFT) & 0x8000);

				// check if any of the rows have been clicked
				bool selection_made = false;
				auto selected_previous = specs_.selected;
				specs_.selected.clear();

				for (auto& it : hot_spots_) {
					auto rect = it.second;
					scale_RECT(rect, dpi_scale_);

					if (point_.x >= rect.left && point_.x <= rect.right &&
						point_.y >= rect.top && point_.y <= rect.bottom) {
						// change the selection
						selection_made = true;

						if (ctrl_pressed) {
							if (std::find(selected_previous.begin(), selected_previous.end(),
								it.first) == selected_previous.end()) {
								// add this row to the current selection, using the
								// order in which items appear in the table
								for (unsigned long row_number = 0; row_number < specs_.data.size();
									row_number++) {
									if (row_number == it.first)
										specs_.selected.push_back(row_number);
									else
										if (std::find(selected_previous.begin(), selected_previous.end(),
											row_number) != selected_previous.end())
											specs_.selected.push_back(row_number);
								}
							}
							else
							{
								// de-select this item
								for (const auto& m_it : selected_previous) {
									if (m_it == it.first)
										continue;
									specs_.selected.push_back(m_it);
								}
							}
						}
						else
							if (shift_pressed && !selected_previous.empty()) {
								// select all items beginning with the last selected item and this one
								if (it.first > last_selected_)
									for (unsigned long current_row = last_selected_;
										current_row <= it.first; current_row++)
										specs_.selected.push_back(current_row);
								else
									for (unsigned long current_row = it.first;
										current_row <= last_selected_; current_row++)
										specs_.selected.push_back(current_row);
							}
							else
								specs_.selected.push_back(it.first);

						/// Last selected item algorithm:
						/// 
						/// It's the item that's selected when when there is no selection or the item
						/// that's selected with shift is not pressed
						if (selected_previous.empty() || !shift_pressed)
							last_selected_ = it.first;
					}
				}

				if (selection_made) {
					reset_menu();

					if (specs_.on_selection)
						on_selection();

					if (specs_.on_click)
						specs_.on_click();
				}
			}
		}

		bool widgets_impl::table::on_mousewheel(float units) {
			float adjustment = units * row_height_;

			v_displacement_ += adjustment;
			v_displacement_previous_ += adjustment;
			return true;
		}

		bool widgets_impl::table::on_keydown(WPARAM wParam) {
			float adjustment = 0.f;

			switch (wParam) {
			case VK_UP:
				adjustment = row_height_;
				break;
			case VK_DOWN:
				adjustment = -row_height_;
				break;
			default:
				break;
			}

			if (adjustment != 0.f) {
				if (specs_.selected.empty()) {
					// simple scrolling
					v_displacement_ += adjustment;
					v_displacement_previous_ += adjustment;
				}
				else {
					// move last selection one unit (unless it's at the beginning or the end)
					long new_selection = smallest(static_cast<long>(specs_.data.size() - 1),
						largest(0L, static_cast<long>(last_selected_) -
							static_cast<long>(adjustment / row_height_)));

					specs_.selected.clear();
					specs_.selected.push_back(new_selection);
					last_selected_ = new_selection;

					// check out if new selection is within table area
					D2D1_RECT_F rect_selected = rectA_;

					rect_selected.top += (row_height_ * new_selection);
					rect_selected.bottom = rect_selected.top + row_height_;

					float diff = 0.f;
					if (rect_selected.top < rectB_.top)
						diff = rectB_.top - rect_selected.top;	// above table area
					else
						if (rect_selected.bottom > rectB_.bottom)
							diff = rectB_.bottom - rect_selected.bottom;	// below table area

					if (diff != 0.f) {
						// move the displacement to ensure visibility of new selection
						v_displacement_ += diff;
						v_displacement_previous_ += diff;
					}

					book_on_selection_ = true;
				}
			}

			if (adjustment != 0.f)
				return true;
			else
				return false;
		}

		bool widgets_impl::table::hit(const bool& hit) {
			if (is_static_ || hit == hit_) {
				if (hit || pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		widgets::table::table_specs&
			widgets_impl::table::specs() { return specs_; }

		void widgets_impl::table::on_selection() {
			if (specs_.on_selection) {
				std::vector<std::map<std::string, std::string>> var;
				for (const auto& it : specs_.selected) {
					try { var.push_back(specs_.data.at(it)); }
					catch (const std::exception&) {}
				}

				specs_.on_selection(var);
			}
		}
	}
}
