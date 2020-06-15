/*
** combobox_impl.cpp - combobox_impl implementation
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

#include "combobox_impl.h"
#include "../../timer.h"
#include "../../menus/context_menu.h"
#include "../../form_impl/form_impl.h"

#include <algorithm>

namespace liblec {
	namespace lecui {
		widgets_impl::combobox::combobox(const std::string& page_alias,
			const std::string& alias,
			form& fm,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_caret_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_fill_editable_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_brush_border_(nullptr),
			p_brush_dropdown_(nullptr),
			p_brush_dropdown_hot_(nullptr),
			p_brush_dropdown_arrow_(nullptr),
			p_brush_dropdown_arrow_hot_(nullptr),
			p_text_format_(nullptr),
			fm_(fm),
			h_cursor_edit_(get_cursor(widgets::specs::cursor_type::caret)),
			h_cursor_dropdown_(get_cursor(widgets::specs::cursor_type::arrow)),
			p_directwrite_factory_(p_directwrite_factory),
			rect_dropdown_({ 0.f, 0.f, 0.f, 0.f }),
			rect_text_({ 0.f, 0.f, 0.f, 0.f }),
			p_text_layout_(nullptr),
			margin_x_(7.5f),
			margin_y_(2.5f),
			caret_blink_timer_name_("caret_blink_timer::combobox"),
			caret_position_(0),
			caret_visible_(true),
			skip_blink_(false),
			text_off_set_(0.f),
			is_selecting_(false),
			is_selected_(false),
			dropdown_activated_(false),
			skip_nextdropdown_(false),
			selection_info_({ 0, 0 }) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::combobox::~combobox() { discard_resources(); }

		void widgets_impl::combobox::press(const bool& pressed) {
			D2D1_RECT_F rect = rect_dropdown_;
			scale_RECT(rect, dpi_scale_);

			if (point_.x >= rect.left && point_.x <= rect.right &&
				point_.y >= rect.top && point_.y <= rect.bottom) {
				if (dropdown_activated_)
					skip_nextdropdown_ = true;
			}
			else
				skip_nextdropdown_ = false;

			return widgets_impl::widget::press(pressed);
		}

		widgets_impl::widget_type
			widgets_impl::combobox::type() {
			return lecui::widgets_impl::widget_type::combobox;
		}

		HRESULT widgets_impl::combobox::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().selection == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);
			
			for (auto& item : specs_.items) {
				if (item.label == specs_.selected) {
					specs_.text = item.label;
					break;
				}
			}

			sort_items();

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill_editable),
					&p_brush_fill_editable_);
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_caret),
					&p_brush_caret_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown),
					&p_brush_dropdown_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown_hot),
					&p_brush_dropdown_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown_arrow),
					&p_brush_dropdown_arrow_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_dropdown_arrow_hot),
					&p_brush_dropdown_arrow_hot_);

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

		void widgets_impl::combobox::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_caret_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_fill_editable_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_dropdown_);
			safe_release(&p_brush_dropdown_hot_);
			safe_release(&p_brush_dropdown_arrow_);
			safe_release(&p_brush_dropdown_arrow_hot_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::combobox::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_combobox_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_combobox_.left -= offset.x;
			rect_combobox_.right -= offset.x;
			rect_combobox_.top -= offset.y;
			rect_combobox_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			if (!specs_.editable) {
				specs_.text.clear();

				for (auto& item : specs_.items) {
					if (item.label == specs_.selected) {
						specs_.text = item.label;
						break;
					}
				}
			}

			auto text_ = specs_.text;

			// make sure caret is well positioned in case text has since been changed
			caret_position_ = smallest(caret_position_, static_cast<UINT32>(specs_.text.length()));

			D2D1_ROUNDED_RECT rounded_rect{ rect_combobox_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			// draw background
			if (render && visible_)
				p_render_target->FillRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
					specs_.editable ? p_brush_fill_editable_ :
					hit_ ? p_brush_hot_ : p_brush_fill_);

			// draw dropdown rect
			rect_dropdown_ = rect_combobox_;

			// width 80% of height
			rect_dropdown_.left = rect_dropdown_.right -
				.8f * (rect_dropdown_.bottom - rect_dropdown_.top);

			D2D1_ROUNDED_RECT dropdown_rounded_rect{ rect_dropdown_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			// draw dropdown rectangle
			if (render && visible_)
				p_render_target->FillRoundedRectangle(&dropdown_rounded_rect, !is_enabled_ ?
					p_brush_disabled_ : hit_ ? p_brush_dropdown_hot_ : p_brush_dropdown_);

			// define dropdown confines
			D2D1_POINT_2F top_left, top_right, bottom;

			// width 40%
			top_left.x = rect_dropdown_.left + .4f * (rect_dropdown_.right - rect_dropdown_.left) / 2.f;
			top_right.x = rect_dropdown_.right - .4f * (rect_dropdown_.right - rect_dropdown_.left) / 2.f;

			// height 25%
			top_left.y = rect_dropdown_.top + .75f * (rect_dropdown_.bottom - rect_dropdown_.top) / 2.f;
			top_right.y = top_left.y;

			bottom.x = (top_left.x + top_right.x) / 2.f;
			bottom.y = rect_dropdown_.bottom - .75f * (rect_dropdown_.bottom - rect_dropdown_.top) / 2.f;

			if (render && visible_) {
				// draw dropdown arrow
				p_render_target->DrawLine(top_left, bottom, hit_ ?
					p_brush_dropdown_arrow_hot_ : p_brush_dropdown_arrow_);
				p_render_target->DrawLine(top_right, bottom, hit_ ?
					p_brush_dropdown_arrow_hot_ : p_brush_dropdown_arrow_);

				// draw combobox border
				p_render_target->DrawRoundedRectangle(&rounded_rect, !is_enabled_ ? p_brush_disabled_ :
					p_brush_border_);

				if (!is_static_ && is_enabled_ && selected_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, 1.75f);
			}

			rect_text_ = rect_combobox_;
			rect_text_.right = rect_dropdown_.left;

			rect_text_.left += margin_x_;
			rect_text_.right -= margin_x_;
			rect_text_.top += margin_y_;
			rect_text_.bottom -= margin_y_;

			// define rectangle for clipping text
			auto rect_text_clip_ = rect_;
			rect_text_clip_.left += (margin_x_ / 3.f);
			rect_text_clip_.right -= (margin_x_ / 3.f);
			rect_text_clip_.top += (margin_y_ / 3.f);
			rect_text_clip_.bottom -= (margin_y_ / 3.f);

			if (specs_.editable) {
				// define text box rect and actual text rect (with possible overflow)
				const auto rect_text_box_ = rect_text_;
				rect_text_ = measure_text(p_directwrite_factory_,
					text_, specs_.font, specs_.font_size, false, true, true, false, rect_text_);

				// measure the text up to the caret position
				const auto text_to_caret = text_.substr(0, caret_position_);
				const auto rect_up_to_caret_ = measure_text(p_directwrite_factory_,
					text_to_caret, specs_.font, specs_.font_size, false, true, true, false, rect_text_);

				bool iterate = false;
				do {
					iterate = false;

					// compute offset
					UINT32 hidden_left = 0;
					UINT32 hidden_right = 0;
					const float off_set_right = ((rect_text_.right - rect_text_.left) - (rect_text_box_.right - rect_text_box_.left)) + text_off_set_;
					{
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
							(UINT32)text_.length(), p_text_format_, rect_text_.right - rect_text_.left,
							rect_text_.bottom - rect_text_.top, &p_text_layout_);

						// characters hidden to the left of text box
						const D2D1_POINT_2F pt_left = D2D1::Point2F(rect_text_.left - text_off_set_, rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f);
						hidden_left = count_characters(p_text_layout_, text_, rect_text_, pt_left, dpi_scale_);

						// characters hidden to the right of text box

						const D2D1_POINT_2F pt_right = D2D1::Point2F(rect_text_box_.left + off_set_right, rect_text_.top + (rect_text_.bottom - rect_text_.top) / 2.f);
						hidden_right = count_characters(p_text_layout_, text_, rect_text_, pt_right, dpi_scale_);

						safe_release(&p_text_layout_);
					}

					const auto textbox_width = rect_text_box_.right - rect_text_box_.left;
					const auto distance_to_caret = rect_up_to_caret_.right - rect_up_to_caret_.left;
					const auto off_set_left = textbox_width - distance_to_caret;

					if (off_set_left < text_off_set_ || hidden_left == 0) {
						// Either
						// 1. caret has reached far right and text is being added
						// 2. text hasn't filled textbox
						//
						// keep caret to the rightmost but within textbox (pin to the right if end is reached,
						// pushing text to the left).
						text_off_set_ = off_set_left;
					}
					else {
						if (hidden_left >= caret_position_) {
							// caret has reached far left
							// prevent caret from being hidden by off-setting text by up to 40px
							text_off_set_ += 40.f;
							iterate = true;
						}
						else
							if (hidden_right == 0) {
								// keep text pinned to the right as we downsize while there's some hidden on the left
								text_off_set_ -= off_set_right;
							}
							else {
								// do nothing under these circumstances
								// * Text has overflown on either or both sides but caret is in the middle
								//   while user is either adding or removing text
							}
					}

					// this offset cannot be greater than zero or text will be indented!!!
					text_off_set_ = smallest(text_off_set_, 0.f);
				} while (iterate);

				// apply offset to text rect to ensure visibility of caret
				rect_text_.left += text_off_set_;
				rect_text_.right += text_off_set_;
			}

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(specs_.text).c_str(),
				(UINT32)specs_.text.length(), p_text_format_, rect_text_.right - rect_text_.left,
				rect_text_.bottom - rect_text_.top, &p_text_layout_);

			if (SUCCEEDED(hr) && render && visible_) {
				// clip text
				auto_clip clip(render, p_render_target, rect_text_clip_, 0.f);

				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
					p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			if (specs_.editable) {
				if (!is_static_ && is_enabled_ && selected_) {
					if (hit_ && pressed_) {
						reset_selection();

						caret_position_ = get_caret_position(p_text_layout_, specs_.text, rect_text_, point_, dpi_scale_);
						caret_visible_ = true;

						if (point_.x != point_on_press_.x || point_.y != point_on_press_.y) {
							// user is making a selection
							is_selecting_ = true;

							auto selection_start_ = get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_press_, dpi_scale_);
							auto selection_end_ = caret_position_;

							auto rect_selection = get_selection_rect(p_text_layout_, rect_text_, selection_start_, selection_end_);
							p_render_target->FillRectangle(rect_selection, p_brush_selected_);
						}
					}
					else
						if (!pressed_ && is_selecting_) {
							// user is done with the selection
							is_selecting_ = false;

							set_selection(
								get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_press_, dpi_scale_),
								get_caret_position(p_text_layout_, specs_.text, rect_text_, point_on_release_, dpi_scale_));
						}
				}

				// draw selection rectangle
				if (!is_static_ && is_enabled_ && is_selected_) {
					auto rect_selection = get_selection_rect(p_text_layout_, rect_text_, selection_info_.start, selection_info_.end);
					p_render_target->FillRectangle(rect_selection, p_brush_selected_);
				}

				// draw caret
				if (!is_static_ && is_enabled_ && selected_ && caret_visible_) {
					const auto caret_rect = get_caret_rect(p_text_layout_, rect_text_, caret_position_);
					p_render_target->FillRectangle(&caret_rect, p_brush_caret_);
				}
			}

			// release the text layout
			safe_release(&p_text_layout_);

			rect_ = rect_combobox_;
			return rect_;
		}

		void widgets_impl::combobox::on_click() {
			bool drop_down = true;

			if (!is_static_ && specs_.editable) {
				D2D1_RECT_F rect = rect_dropdown_;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom) {
					if (skip_nextdropdown_) {
						skip_nextdropdown_ = false;
						drop_down = false;
					}
					else
						drop_down = true;
				}
				else
					drop_down = false;
			}

			if (drop_down) {
				// check if any of the items have been clicked
				std::string selected_previous = specs_.selected;

				if (specs_.editable)
					selected_previous = specs_.text;

				dropdown_activated_ = true;

				auto selected_new = dropdown(rect_);

				if (!selected_new.empty()) {
					specs_.selected = selected_new;

					if (specs_.editable)
						specs_.text = specs_.selected;

					if (selected_previous != specs_.selected) {
						// move caret to the end
						caret_position_ = static_cast<UINT32>(specs_.selected.length());

						if (specs_.events().selection)
							specs_.events().selection(specs_.selected);

						if (specs_.events().click)
							specs_.events().click();
					}
				}

				dropdown_activated_ = false;
			}
		}

		bool widgets_impl::combobox::hit(const bool& hit) {
			if (!is_static_ && specs_.editable) {
				D2D1_RECT_F rect = rect_dropdown_;
				scale_RECT(rect, dpi_scale_);

				if (point_.x >= rect.left && point_.x <= rect.right &&
					point_.y >= rect.top && point_.y <= rect.bottom)
					h_cursor_ = h_cursor_dropdown_;
				else
					h_cursor_ = h_cursor_edit_;
			}

			if (is_static_ || hit == hit_) {
				if (hit || pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		void widgets_impl::combobox::on_selection_change(const bool& selected) {
			if (specs_.editable) {
				if (selected) {
					// start blink timer
					log("starting caret blink timer: " + alias_);
					timer_management(fm_).add(caret_blink_timer_name_, 500,
						[&]() {
							if (skip_blink_)
								skip_blink_ = false;
							else {
								caret_visible_ = !caret_visible_;
								fm_.update();
							}
						});
				}
				else {
					// stop blink timer
					log("stopping caret blink timer: " + alias_);
					timer_management(fm_).stop(caret_blink_timer_name_);

					// stop selection
					reset_selection();
				}
			}
		}

		widgets::combobox::combobox_specs&
			widgets_impl::combobox::specs() { return specs_; }

		void widgets_impl::combobox::insert_character(const char& c) {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
					reset_selection();
				}

				std::string s;
				s += c;
				specs_.text.insert(caret_position_, s);
				caret_position_++;
				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::combobox::key_backspace() {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
					reset_selection();
				}
				else {
					specs_.text.erase(caret_position_ - 1, 1);
					caret_position_--;
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::combobox::key_delete() {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					specs_.text.erase(selection_info_.start, selection_info_.end - selection_info_.start);
					reset_selection();
				}
				else {
					specs_.text.erase(caret_position_, 1);
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::combobox::key_left() {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start + 1;
					reset_selection();
				}

				if (caret_position_ > 0)
					caret_position_--;

				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::combobox::key_right() {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.end - 1;
					reset_selection();
				}

				if (caret_position_ < specs_.text.length())
					caret_position_++;

				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::combobox::key_return() {
			if (specs_.editable) {
				bool already_in_list = false;

				for (const auto& item : specs_.items) {
					if (item.label == specs_.text) {
						already_in_list = true;
						break;
					}
				}

				if (!already_in_list) {
					// add text to items list
					widgets::combobox::combobox_item item;
					item.label = specs_.text;
					specs_.items.push_back(item);
				}

				specs_.selected = specs_.text;
				sort_items();
			}
		}

		UINT32
			widgets_impl::combobox::count_characters(IDWriteTextLayout* p_text_layout, const std::string& text,
				const D2D1_RECT_F& rect_text, const D2D1_POINT_2F& point,
				const float& dpi_scale) {
			BOOL is_trailing;
			BOOL is_inside;
			DWRITE_HIT_TEST_METRICS hitTestMetrics;

			// check position of click
			p_text_layout->HitTestPoint(point.x - rect_text.left, point.y - rect_text.top,
				&is_trailing, &is_inside, &hitTestMetrics);

			auto characters = hitTestMetrics.textPosition;
			if (is_trailing && (characters < text.length())) characters++;
			return characters;
		}

		UINT32
			widgets_impl::combobox::get_caret_position(IDWriteTextLayout* p_text_layout, const std::string& text,
				const D2D1_RECT_F& rect_text, const D2D1_POINT_2F& point,
				const float& dpi_scale) {
			auto rect_hit = rect_text;

			scale_RECT(rect_hit, dpi_scale);

			auto p_x = point.x - rect_hit.left;
			auto p_y = point.y - rect_hit.top;

			p_x /= dpi_scale;
			p_y /= dpi_scale;

			BOOL is_trailing;
			BOOL is_inside;
			DWRITE_HIT_TEST_METRICS hitTestMetrics;

			// check position of click
			p_text_layout->HitTestPoint(p_x, p_y,
				&is_trailing, &is_inside, &hitTestMetrics);

			// change caret position
			auto caret_position = hitTestMetrics.textPosition;

			if (is_trailing && (caret_position < text.length()))
				caret_position++;

			return caret_position;
		}

		D2D1_RECT_F
			widgets_impl::combobox::get_selection_rect(
				IDWriteTextLayout* p_text_layout, const D2D1_RECT_F& rect_text,
				const UINT32& selection_start, const UINT32& selection_end) {
			DWRITE_HIT_TEST_METRICS hit_metrics_start, hit_metrics_end;

			float p_x, p_y;
			bool isTrailingHit = false;
			p_text_layout->HitTestTextPosition(selection_start,
				isTrailingHit, &p_x, &p_y, &hit_metrics_start);

			p_text_layout->HitTestTextPosition(selection_end,
				isTrailingHit, &p_x, &p_y, &hit_metrics_end);

			return D2D1::RectF(rect_text.left + hit_metrics_start.left,
				rect_text.top, rect_text.left + hit_metrics_end.left, rect_text.bottom);
		}

		float widgets_impl::combobox::get_caret_width() {
			// respect user settings
			DWORD caret_width = 1;
			SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caret_width, 0);
			return static_cast<float>(caret_width);
		}

		D2D1_RECT_F
			widgets_impl::combobox::get_caret_rect(IDWriteTextLayout* p_text_layout,
				const D2D1_RECT_F& rect_text, const UINT32& caret_position) {
			DWRITE_HIT_TEST_METRICS hit_metrics;
			float p_x, p_y;

			bool is_trailing_hit = false;
			p_text_layout->HitTestTextPosition(caret_position,
				is_trailing_hit, &p_x, &p_y, &hit_metrics);

			const float half_caret_width = get_caret_width() / 2.f;

			return D2D1::RectF(rect_text.left + hit_metrics.left - half_caret_width,
				rect_text.top + hit_metrics.top,
				rect_text.left + hit_metrics.left + half_caret_width,
				rect_text.top + hit_metrics.top + hit_metrics.height);
		}

		bool widgets_impl::combobox::is_numeric(const std::string& text) {
			bool numeric = true;

			for (const auto& c : text) {
				if (isalpha(c))
					numeric = false;

				if (!numeric)
					break;
			}

			return numeric;
		}

		bool widgets_impl::combobox::is_numeric(const std::vector<widgets::combobox::combobox_item>& items) {
			bool numeric = true;

			for (auto& item : items) {
				if (!is_numeric(item.label))
					numeric = false;

				if (!numeric)
					break;
			}

			return numeric;
		}

		void widgets_impl::combobox::sort_items() {
			auto sort_ascending = [](widgets::combobox::combobox_item& a, widgets::combobox::combobox_item& b) {
				return a.label < b.label;
			};

			auto sort_descending = [](widgets::combobox::combobox_item& a, widgets::combobox::combobox_item& b) {
				return a.label > b.label;
			};

			auto sort_ascending_numeric = [](widgets::combobox::combobox_item& a, widgets::combobox::combobox_item& b) {
				return atof(a.label.c_str()) < atof(b.label.c_str());
			};

			auto sort_descending_numeric = [](widgets::combobox::combobox_item& a, widgets::combobox::combobox_item& b) {
				return atof(a.label.c_str()) > atof(b.label.c_str());
			};

			// sort the items
			switch (specs_.sort) {
			case sort_options::ascending:
				if (specs_.force_numerical_sort || is_numeric(specs_.items))
					std::sort(specs_.items.begin(), specs_.items.end(), sort_ascending_numeric);
				else
					std::sort(specs_.items.begin(), specs_.items.end(), sort_ascending);
				break;
			case sort_options::descending:
				if (specs_.force_numerical_sort || is_numeric(specs_.items))
					std::sort(specs_.items.begin(), specs_.items.end(), sort_descending_numeric);
				else
					std::sort(specs_.items.begin(), specs_.items.end(), sort_descending);
				break;
			case sort_options::none:
			default:
				break;
			}
		}

		widgets::combobox::combobox_specs&
			widgets_impl::combobox::operator()() { return specs(); }
		std::string widgets_impl::combobox::dropdown(D2D1_RECT_F rect) {
			context_menu::specs menu_specs;

			for (const auto& item : specs_.items) {
				menu_item mi;
				mi.label = item.label;
				mi.font = item.font;
				mi.font_size = item.font_size;
				menu_specs.items.push_back(mi);
			}

			menu_specs.pin = convert_rect(rect_combobox_);

			POINT pt = { 0, 0 };
			ClientToScreen(fm_.d_.hWnd_, &pt);

			menu_specs.pin.left += (pt.x / dpi_scale_);
			menu_specs.pin.right += (pt.x / dpi_scale_);
			menu_specs.pin.top += (pt.y / dpi_scale_);
			menu_specs.pin.bottom += (pt.y / dpi_scale_);

			return context_menu()(fm_, menu_specs);
		}
	}
}
