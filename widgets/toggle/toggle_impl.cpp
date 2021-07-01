//
// toggle_impl.cpp - toggle_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "toggle_impl.h"

namespace liblec {
	namespace lecui {
		widgets::toggle_impl::toggle_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_on_(nullptr),
			p_brush_off_(nullptr),
			p_brush_on_hot_(nullptr),
			p_brush_off_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			perc_along_(0.f) {}

		widgets::toggle_impl::~toggle_impl() { discard_resources(); }

		widgets::widget_type
			widgets::toggle_impl::type() {
			return lecui::widgets::widget_type::toggle;
		}

		HRESULT widgets::toggle_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().toggle == nullptr && specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_on()),
					&p_brush_on_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_off()),
					&p_brush_off_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(
					convert_color(lighten_color(specs_.color_on(), 25)), &p_brush_on_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(
					convert_color(lighten_color(specs_.color_off(), 25)), &p_brush_off_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled()),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected()),
					&p_brush_selected_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text()),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory_->CreateTextFormat(
					convert_string(specs_.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(specs_.font_size()),
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

		void widgets::toggle_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_on_);
			safe_release(&p_brush_off_);
			safe_release(&p_brush_on_hot_);
			safe_release(&p_brush_off_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets::toggle_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect(), specs_.on_resize(), change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			const float toggle_height = smallest(rect_.bottom - rect_.top, rect_.right - rect_.left);
			const float toggle_width = 2.f * toggle_height;

			rect_toggle_ = rect_;
			rect_toggle_.right = rect_toggle_.left + toggle_width;
			rect_toggle_.bottom = rect_toggle_.top + toggle_height;
			pos_rect(rect_, rect_toggle_, 0.f, 50.f);

			D2D1_RECT_F toggle_background_ = rect_toggle_;
			D2D1_ROUNDED_RECT toggle_background_round_ = { toggle_background_,
				(toggle_background_.bottom - toggle_background_.top) / 2.f,
				(toggle_background_.bottom - toggle_background_.top) / 2.f };

			D2D1_RECT_F toggle_foreground_ = rect_toggle_;
			toggle_foreground_.right = toggle_foreground_.right -
				(toggle_foreground_.right - toggle_foreground_.left) / 2.f;

			perc_along_ = specs_.on() ? 100.f : 0.f;

			bool x_change = (point_.x != point_on_press_.x);
			bool y_change = (point_.y != point_on_press_.y);

			if (pressed_ && (x_change || y_change)) {
				D2D1_RECT_F rect = rect_toggle_;
				rect.left += ((toggle_foreground_.right - toggle_foreground_.left) / 2.f);
				rect.right -= ((toggle_foreground_.right - toggle_foreground_.left) / 2.f);

				scale_RECT(rect, get_dpi_scale());

				const float mid_point_x = rect.left + ((rect.right - rect.left) / 2.f);

				perc_along_ = 100.f * (point_.x - rect.left) / (rect.right - rect.left);
				perc_along_ = largest(perc_along_, 0.f);
				perc_along_ = smallest(perc_along_, 100.f);
			}

			pos_rect(rect_toggle_, toggle_foreground_, perc_along_, 50.f);

			// to-do: shrink by exactly 90%
			const float shrink_amount = .05f * (toggle_foreground_.right - toggle_foreground_.left);
			toggle_foreground_.left += shrink_amount;
			toggle_foreground_.right -= shrink_amount;
			toggle_foreground_.top += shrink_amount;
			toggle_foreground_.bottom -= shrink_amount;

			D2D1_ROUNDED_RECT toggle_foreground_round_ = { toggle_foreground_,
				(toggle_foreground_.bottom - toggle_foreground_.top) / 2.f,
				(toggle_foreground_.bottom - toggle_foreground_.top) / 2.f };

			ID2D1SolidColorBrush* p_brush = (perc_along_ >= 50.f) ? p_brush_on_ : p_brush_off_;
			ID2D1SolidColorBrush* p_brush_hot = (perc_along_ >= 50.f) ?
				p_brush_on_hot_ : p_brush_off_hot_;

			if (render && visible_) {
				p_render_target->FillRoundedRectangle(&toggle_background_round_, !is_enabled_ ?
					p_brush_disabled_ : hit_ ? p_brush_hot : p_brush);
				p_render_target->FillRoundedRectangle(&toggle_foreground_round_, !is_enabled_ ?
					p_brush_fill_ : p_brush_fill_);
			}

			auto rect_text_ = rect_;
			rect_text_.left = rect_toggle_.right + ((rect_toggle_.bottom - rect_toggle_.top) / 3.f);

			// create a text layout
			const std::string& text = specs_.on() ? specs_.text() : specs_.text_off();
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

				if (render && visible_ && !is_static_ && is_enabled_ && selected_)
					p_render_target->FillRectangle(&rect_text_, p_brush_selected_);

				if (render && visible_)
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_og.left, rect_text_og.top },
						p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			rect_.right = rect_text_.right;

			// release the text layout
			safe_release(&p_text_layout_);

			return rect_;
		}

		void widgets::toggle_impl::on_click() {
			bool x_change = (point_.x != point_on_press_.x);
			bool y_change = (point_.y != point_on_press_.y);

			if (!(x_change || y_change)) {
				specs_.on() = !specs_.on();	// A click with no mouse move. Toggle.
				perc_along_ = specs_.on() ? 100.f : 0.f;
			}
			else
				specs_.on() = perc_along_ >= 50.f;

			if (specs_.events().toggle)
				specs_.events().toggle(specs_.on());

			if (specs_.events().click)
				specs_.events().click();
		}

		void widgets::toggle_impl::on_action() {
			specs_.on(!specs_.on());	// Toggle.

			if (specs_.events().toggle)
				specs_.events().toggle(specs_.on());

			if (specs_.events().action)
				specs_.events().action();
		}

		void widgets::toggle_impl::on_right_click() {
			if (specs_.events().right_click)
				specs_.events().right_click();
		}

		bool widgets::toggle_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, get_dpi_scale());

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return contains();
			else {
				if (pressed_)
					return true;
				else
					return false;
			}
		}

		bool widgets::toggle_impl::contains() { return true; }

		bool widgets::toggle_impl::hit(const bool& hit) {
			if (is_static_ || hit == hit_) {
				if (pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		widgets::toggle_specs&
			widgets::toggle_impl::specs() { return specs_; }

		widgets::toggle_specs&
			widgets::toggle_impl::operator()() { return specs(); }
	}
}
