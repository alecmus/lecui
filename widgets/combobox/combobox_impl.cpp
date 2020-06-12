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
#include "../../menus/context_menu.h"
#include "../../form_impl/form_impl.h"

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
			p_text_layout_(nullptr) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::combobox::~combobox() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::combobox::type() {
			return lecui::widgets_impl::widget_type::combobox;
		}

		HRESULT widgets_impl::combobox::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().selection == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

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

			specs_.text.clear();

			for (auto& item : specs_.items) {
				if (item == specs_.selected) {
					specs_.text = item;
					break;
				}
			}

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
			rect_text_.left += ((rect_combobox_.bottom - rect_combobox_.top) / 4.f);
			rect_text_.right = rect_dropdown_.left - ((rect_combobox_.bottom - rect_combobox_.top) / 4.f);

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(specs_.text).c_str(),
				(UINT32)specs_.text.length(), p_text_format_, rect_text_.right - rect_text_.left,
				rect_text_.bottom - rect_text_.top, &p_text_layout_);

			if (SUCCEEDED(hr) && render && visible_) {
				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
					p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
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
					point_.y >= rect.top && point_.y <= rect.bottom)
					drop_down = true;
				else
					drop_down = false;
			}

			if (drop_down) {
				// check if any of the items have been clicked
				std::string selected_previous = specs_.selected;

				auto selected_new = dropdown(rect_);

				if (!selected_new.empty()) {
					specs_.selected = selected_new;

					if (selected_previous != specs_.selected) {
						if (specs_.events().selection)
							specs_.events().selection(specs_.selected);

						if (specs_.events().click)
							specs_.events().click();
					}
				}
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

		widgets::combobox::combobox_specs&
			widgets_impl::combobox::specs() { return specs_; }

		widgets::combobox::combobox_specs&
			widgets_impl::combobox::operator()() { return specs(); }
		std::string widgets_impl::combobox::dropdown(D2D1_RECT_F rect) {
			context_menu::specs menu_specs;

			for (const auto& item : specs_.items)
				menu_specs.items.push_back({ item });

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
