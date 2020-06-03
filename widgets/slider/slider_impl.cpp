/*
** slider_impl.cpp - slider_impl implementation
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

#include "slider_impl.h"
#include "../label/label_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::slider::slider(const std::string& page_alias,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_knob_(nullptr),
			p_brush_knob_hot_(nullptr),
			p_brush_knob_border_(nullptr),
			p_brush_tick_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			perc_along_(0.f) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::slider::~slider() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::slider::type() {
			return lecui::widgets_impl::widget_type::slider;
		}

		HRESULT widgets_impl::slider::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().slider == nullptr && specs_.events().click == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_knob),
					&p_brush_knob_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_knob_hot),
					&p_brush_knob_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_knob_border),
					&p_brush_knob_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_tick),
					&p_brush_tick_);
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
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

		void widgets_impl::slider::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_knob_);
			safe_release(&p_brush_knob_hot_);
			safe_release(&p_brush_knob_border_);
			safe_release(&p_brush_tick_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::slider::render(ID2D1HwndRenderTarget* p_render_target,
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

			// function for snapping value to ticks
			auto do_snap = [&](float& value) {
				std::vector<float> snap_to;
				const float major_div = specs_.major_tick_unit;
				const float minor_div = specs_.major_tick_unit / (specs_.minor_tick_count + 1);
				for (float p = specs_.range.min; p <= specs_.range.max; p += specs_.major_tick_unit) {
					// add major div to snap vector
					snap_to.push_back(p);

					// add minor divs to snap vector
					for (float v = p + minor_div; v < smallest(p + major_div, specs_.range.max); v += minor_div)
						snap_to.push_back(v);
				}

				float snap_value = specs_.range.min;

				for (const auto& v : snap_to) {
					auto lower = v;
					auto higher = v + minor_div;
					if (value >= lower && value <= higher) {
						if (value < lower + (minor_div / 2.f))
							snap_value = v;
						else
							snap_value = v + minor_div;
					}
				}

				value = snap_value;
				value = largest(value, specs_.range.min);
				value = smallest(value, specs_.range.max);
			};

			if (abs(rect_.right - rect_.left) > abs(rect_.bottom - rect_.top)) {
				// orient horizontally

				// starting and ending points of slider
				const D2D1_POINT_2F start = { rect_.left + specs_.knob_radius, rect_.top + specs_.knob_radius };
				const D2D1_POINT_2F end = { rect_.right - specs_.knob_radius, rect_.top + specs_.knob_radius };

				// draw slide
				const D2D1_RECT_F slide_rect = {
					start.x - (specs_.slide_thickness / 2.f),
					start.y - (specs_.slide_thickness / 2.f),
					end.x + (specs_.slide_thickness / 2.f),
					end.y + (specs_.slide_thickness / 2.f)
				};

				const D2D1_ROUNDED_RECT slide_rect_rounded = { slide_rect, specs_.slide_thickness / 2.f, specs_.slide_thickness / 2.f };
				p_render_target->FillRoundedRectangle(&slide_rect_rounded, p_brush_fill_);
				p_render_target->DrawRoundedRectangle(&slide_rect_rounded, p_brush_border_, specs_.border);

				if (pressed_) {
					// compute new value
					D2D1_RECT_F rect = slide_rect;
					rect.left = start.x;
					rect.right = end.x;

					scale_RECT(rect, dpi_scale_);

					perc_along_ = 100.f * (point_.x - rect.left) / (rect.right - rect.left);
					perc_along_ = largest(perc_along_, 0.f);
					perc_along_ = smallest(perc_along_, 100.f);

					// calculate value
					specs_.value = (specs_.range.max - specs_.range.min) * perc_along_ / 100.f;
				}

				// snap the value to ticks
				if (specs_.snap_to_ticks)
					do_snap(specs_.value);

				// draw ticks. to-do: remove magic numbers
				const float y = slide_rect.bottom + 5.f;
				const float y_major_tick = y + 6.f;
				const float y_minor_tick = y + 2.5f;
				const float major_div = (specs_.major_tick_unit / (specs_.range.max - specs_.range.min)) * (end.x - start.x);
				const float minor_div = major_div / (specs_.minor_tick_count + 1);

				auto value = 0.f;
				const long major_ticks = static_cast<long>(floor((specs_.range.max - specs_.range.min) / specs_.major_tick_unit));
				const long minor_ticks = major_ticks * specs_.minor_tick_count;

				long minor_tick_count = 0;
				for (long n_major = 0; n_major < major_ticks + 1; n_major++) {
					const float x_major = start.x + major_div * n_major;

					float lower_margin = y;
					if (specs_.show_tick_marks) {
						lower_margin = y_major_tick;

						// draw major ticks
						p_render_target->DrawLine({ x_major, y }, { x_major, y_major_tick }, p_brush_tick_, 0.5f);

						for (long n_minor = 1; minor_tick_count < minor_ticks && n_minor < specs_.minor_tick_count + 1; n_minor++) {
							const float x_minor = x_major + minor_div * n_minor;

							// draw minor ticks
							p_render_target->DrawLine({ x_minor, y }, { x_minor, y_minor_tick }, p_brush_tick_, 0.5f);
							minor_tick_count++;
						}
					}

					// draw tick label
					if (specs_.show_tick_labels) {
						D2D1_RECT_F max_rect = { 0.f, 0.f, major_div, 20.f };

						const auto label = roundoff::tostr<char>(value, 0);
						value += specs_.major_tick_unit;

						auto label_rect = widgets_impl::measure_label(p_directwrite_factory_, label,
							specs_.font, specs_.font_size, true, true, max_rect);

						D2D1_RECT_F optimal_rect = {
							x_major - (label_rect.right - label_rect.left) / 2.f,
							lower_margin + .5f,
							x_major + (label_rect.right - label_rect.left) / 2.f,
							lower_margin + .5f + (label_rect.bottom - label_rect.top)
						};

						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(label).c_str(),
							(UINT32)label.length(), p_text_format_, optimal_rect.right - optimal_rect.left,
							optimal_rect.bottom - optimal_rect.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ optimal_rect.left, optimal_rect.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&p_text_layout_);
					}
				}

				// draw knob
				const D2D1_POINT_2F position = {
					start.x + (specs_.value / (specs_.range.max - specs_.range.min)) * (end.x - start.x),
					start.y };

				const D2D1_RECT_F knob_rect = {
					position.x - specs_.knob_radius,
					position.y - specs_.knob_radius,
					position.x + specs_.knob_radius,
					position.y + specs_.knob_radius,
				};

				// draw knob
				const D2D1_ROUNDED_RECT knob_rect_round_ = { knob_rect, specs_.knob_radius, specs_.knob_radius };
				p_render_target->FillRoundedRectangle(&knob_rect_round_, (!is_static_ && is_enabled_ && hit_) ? p_brush_knob_hot_ : p_brush_knob_);
				p_render_target->DrawRoundedRectangle(&knob_rect_round_, selected_ ? p_brush_selected_ : p_brush_knob_border_, specs_.border);
			}
			else {
				// orient vertically

				// starting and ending points of slider
				const D2D1_POINT_2F start = { rect_.left + specs_.knob_radius, rect_.bottom - specs_.knob_radius };
				const D2D1_POINT_2F end = { rect_.left + specs_.knob_radius, rect_.top + specs_.knob_radius };

				// draw slide
				const D2D1_RECT_F slide_rect = {
					start.x - (specs_.slide_thickness / 2.f),
					start.y + (specs_.slide_thickness / 2.f),
					end.x + (specs_.slide_thickness / 2.f),
					end.y - (specs_.slide_thickness / 2.f)
				};

				const D2D1_ROUNDED_RECT slide_rect_rounded = { slide_rect, specs_.slide_thickness / 2.f, specs_.slide_thickness / 2.f };
				p_render_target->FillRoundedRectangle(&slide_rect_rounded, p_brush_fill_);
				p_render_target->DrawRoundedRectangle(&slide_rect_rounded, p_brush_border_, specs_.border);

				if (pressed_) {
					// compute new value
					D2D1_RECT_F rect = slide_rect;
					rect.bottom = start.y;
					rect.top = end.y;

					scale_RECT(rect, dpi_scale_);

					perc_along_ = 100.f * (rect.bottom - point_.y) / (rect.bottom - rect.top);
					perc_along_ = largest(perc_along_, 0.f);
					perc_along_ = smallest(perc_along_, 100.f);

					// calculate value
					specs_.value = (specs_.range.max - specs_.range.min) * perc_along_ / 100.f;
				}

				// snap the value to ticks
				if (specs_.snap_to_ticks)
					do_snap(specs_.value);

				// draw ticks. to-do: remove magic numbers
				const float x = slide_rect.right + 5.f;
				const float x_major_tick = x + 6.f;
				const float x_minor_tick = x + 2.5f;
				const float major_div = (specs_.major_tick_unit / (specs_.range.max - specs_.range.min)) * (start.y - end.y);
				const float minor_div = major_div / (specs_.minor_tick_count + 1);

				auto value = 0.f;
				const long major_ticks = static_cast<long>(floor((specs_.range.max - specs_.range.min) / specs_.major_tick_unit));
				const long minor_ticks = major_ticks * specs_.minor_tick_count;

				long minor_tick_count = 0;
				for (long n_major = 0; n_major < major_ticks + 1; n_major++) {
					const float y_major = start.y - major_div * n_major;

					float lower_margin = x;
					if (specs_.show_tick_marks) {
						lower_margin = x_major_tick;

						// draw major ticks
						p_render_target->DrawLine({ x, y_major }, { x_major_tick, y_major }, p_brush_tick_, 0.5f);

						for (long n_minor = 1; minor_tick_count < minor_ticks && n_minor < specs_.minor_tick_count + 1; n_minor++) {
							const float y_minor = y_major - minor_div * n_minor;

							// draw minor ticks
							p_render_target->DrawLine({ x, y_minor }, { x_minor_tick, y_minor }, p_brush_tick_, 0.5f);
							minor_tick_count++;
						}
					}

					// draw tick label
					if (specs_.show_tick_labels) {
						D2D1_RECT_F max_rect = { 0.f, 0.f, rect_.right - x_minor_tick - .5f, major_div };

						const auto label = roundoff::tostr<char>(value, 0);
						value += specs_.major_tick_unit;

						auto label_rect = widgets_impl::measure_label(p_directwrite_factory_, label,
							specs_.font, specs_.font_size, true, true, max_rect);

						D2D1_RECT_F optimal_rect = {
							lower_margin + .5f,
							y_major - (label_rect.bottom - label_rect.top) / 2.f,
							lower_margin + .5f + (label_rect.right - label_rect.left),
							y_major + (label_rect.bottom - label_rect.top) / 2.f
						};

						// create a text layout
						HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(label).c_str(),
							(UINT32)label.length(), p_text_format_, optimal_rect.right - optimal_rect.left,
							optimal_rect.bottom - optimal_rect.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ optimal_rect.left, optimal_rect.top },
								p_text_layout_, is_enabled_ ?
								p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&p_text_layout_);
					}
				}

				// draw knob
				const D2D1_POINT_2F position = { start.x,
					start.y - (specs_.value / (specs_.range.max - specs_.range.min)) * (start.y - end.y)};

				const D2D1_RECT_F knob_rect = {
					position.x - specs_.knob_radius,
					position.y - specs_.knob_radius,
					position.x + specs_.knob_radius,
					position.y + specs_.knob_radius,
				};

				// draw knob
				const D2D1_ROUNDED_RECT knob_rect_round_ = { knob_rect, specs_.knob_radius, specs_.knob_radius };
				p_render_target->FillRoundedRectangle(&knob_rect_round_, (!is_static_ && is_enabled_ && hit_) ? p_brush_knob_hot_ : p_brush_knob_);
				p_render_target->DrawRoundedRectangle(&knob_rect_round_, selected_ ? p_brush_selected_ : p_brush_knob_border_, specs_.border);
			}

			return rect_;
		}

		void widgets_impl::slider::on_click() {
			// to-do: how to know if this is a keyboard and not care about either x_change or y_change

			bool x_change = (point_.x != point_on_press_.x);
			bool y_change = (point_.y != point_on_press_.y);

			if (specs_.events().slider)
				specs_.events().slider(specs_.value);

			if (specs_.events().click)
				specs_.events().click();
		}

		bool widgets_impl::slider::contains(const D2D1_POINT_2F& point) {
			// capture the point
			point_ = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = rect_;
			scale_RECT(rect, dpi_scale_);

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

		bool widgets_impl::slider::contains() { return true; }

		bool widgets_impl::slider::hit(const bool& hit) {
			if (is_static_ || hit == hit_) {
				if (pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		widgets::slider::slider_specs&
			widgets_impl::slider::specs() { return specs_; }

		widgets::slider::slider_specs&
			widgets_impl::slider::operator()() { return specs(); }
	}
}
