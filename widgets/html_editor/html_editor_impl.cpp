/*
** html_editor_impl.cpp - html_editor_impl implementation
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

#include "html_editor_impl.h"
#include "../../timer.h"
#include "../label/label_impl.h"
#include "../../formatted_text_editor/formatted_text_editor.h"
#include <memory>

namespace liblec {
	namespace lecui {
		constexpr UINT32 max_line_count = 16384;	// 2^14

		std::string widgets_impl::html_editor::alias_font() {
			return std::string("lecui::font");
		}

		std::string widgets_impl::html_editor::alias_font_size() {
			return std::string("lecui::font_size");
		}

		std::string widgets_impl::html_editor::alias_bold() {
			return std::string("lecui::bold");
		}

		std::string widgets_impl::html_editor::alias_italic() {
			return std::string("lecui::italic");
		}

		std::string widgets_impl::html_editor::alias_underline() {
			return std::string("lecui::underline");
		}

		std::string widgets_impl::html_editor::alias_strikethrough() {
			return std::string("lecui::strikethrough");
		}

		widgets_impl::html_editor::html_editor(const std::string& page_alias,
			const std::string& alias,
			form& fm,
			IDWriteFactory* p_directwrite_factory) :
			controls_initialized_(false),
			p_brush_(nullptr),
			p_brush_caret_(nullptr),
			p_brush_border_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			fm_(fm),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			margin_x_(7.5f),
			margin_y_(2.5f),
			caret_blink_timer_name_("caret_blink_timer::html_editor"),
			caret_position_(0),
			caret_visible_(true),
			skip_blink_(false),
			is_selecting_(false),
			is_selected_(false),
			selection_info_({ 0, 0 }) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::html_editor::~html_editor() { discard_resources(); }

		bool widgets_impl::html_editor::controls_initialized() {
			return controls_initialized_;
		}

		void widgets_impl::html_editor::initialize_controls(bool init) {
			controls_initialized_ = init;
		}

		widgets_impl::widget_type
			widgets_impl::html_editor::type() {
			return lecui::widgets_impl::widget_type::html_editor;
		}

		HRESULT widgets_impl::html_editor::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = false;
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
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
			}

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::html_editor::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_caret_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::html_editor::render(ID2D1HwndRenderTarget* p_render_target,
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

			// make sure caret is well positioned in case text has since been changed
			caret_position_ = smallest(caret_position_, static_cast<UINT32>(specs_.text.length()));

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, is_enabled_ ? p_brush_fill_ :
				p_brush_disabled_);

			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_border_, .5f);

			//if (!is_static_ && is_enabled_ && selected_)
			//	p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, 1.75f);

			// create a text layout
			std::string text_ = specs_.text;

			auto rect_text_ = rect_;

			// measure the text
			D2D1_RECT_F rect_optimal = rect_text_;

			// get formatting

			// to-do: performance issues ... only redo if there's been a change
			if (true) {
				parse_formatted_text(specs_.text, text_, convert_color(specs_.color_text), formatting_);
			}

			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
				(UINT32)text_.length(), p_text_format_, rect_text_.right - rect_text_.left,
				rect_text_.bottom - rect_text_.top, &p_text_layout_);

			DWRITE_TEXT_METRICS textMetrics;

			if (SUCCEEDED(hr)) {
				

				// apply formatting
				apply_formatting(formatting_, p_render_target, p_text_layout_, is_enabled_,
					p_brush_disabled_);

				p_text_layout_->GetMetrics(&textMetrics);

				DWRITE_TEXT_METRICS textMetrics;
				p_text_layout_->GetMetrics(&textMetrics);
				rect_optimal.left += textMetrics.left;
				rect_optimal.top += textMetrics.top;

				// wrap text horizontally
				rect_optimal.right = smallest(rect_optimal.left + textMetrics.width, rect_optimal.right);

				// allow text to overflow vertically, scroll bars will do the rest
				rect_optimal.bottom = rect_optimal.top + textMetrics.height;

				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text_.left, rect_text_.top },
					p_text_layout_, p_brush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			const auto optimized_bottom_ = rect_optimal.bottom + margin_y_;


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

						auto selection_rects = get_selection_rects(p_text_layout_, rect_text_, selection_start_, selection_end_);
						for (auto selection_rect : selection_rects)
							p_render_target->FillRectangle(selection_rect, p_brush_selected_);
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

			// draw selection rectangles
			if (!is_static_ && is_enabled_ && is_selected_) {
				auto selection_rects = get_selection_rects(p_text_layout_, rect_text_, selection_info_.start, selection_info_.end);
				for (auto selection_rect : selection_rects)
					p_render_target->FillRectangle(selection_rect, p_brush_selected_);
			}

			// draw caret
			if (!is_static_ && is_enabled_ && selected_ && caret_visible_) {
				const auto caret_rect = get_caret_rect(p_text_layout_, rect_text_, caret_position_);
				p_render_target->FillRectangle(&caret_rect, p_brush_caret_);
			}

			// release the text layout
			safe_release(&p_text_layout_);

			const auto old_height = rect_.bottom - rect_.top;
			const auto height = optimized_bottom_ - rect_.top;

			// update widget rect
			specs_.rect.height(height);

			return rect_;
		}

		void widgets_impl::html_editor::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		void widgets_impl::html_editor::on_selection_change(const bool& selected) {
			if (selected) {
				// start blink timer
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
				timer_management(fm_).stop(caret_blink_timer_name_);
			}
		}

		widgets::html_editor::html_editor_specs&
			widgets_impl::html_editor::specs() { return specs_; }

		widgets::html_editor::html_editor_specs&
			widgets_impl::html_editor::operator()() { return specs(); }

		// to-do: insertion mechanics for formatted text
		void widgets_impl::html_editor::insert_character(const char& c) {
			log("before: " + specs_.text);
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text);
					reset_selection();
				}

				formatted_text_editor().insert_character(c, caret_position_, tag_number, specs_.text);
				caret_position_++;
				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }
			log("after : " + specs_.text);
		}

		// to-do: backspace mechanics for formatted text
		void widgets_impl::html_editor::key_backspace() {
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text);
					reset_selection();
				}
				else {
					tag_number = formatted_text_editor().erase(caret_position_ - 1, caret_position_, specs_.text);
					caret_position_--;
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		// to-do: deletion mechanics for formatted text
		void widgets_impl::html_editor::key_delete() {
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text);
					reset_selection();
				}
				else {
					tag_number = formatted_text_editor().erase(caret_position_, caret_position_ + 1, specs_.text);
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::html_editor::key_left() {
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

		void widgets_impl::html_editor::key_right() {
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

		void widgets_impl::html_editor::selection_font(const std::string& font_name) {
			log("selection_font: " + font_name);
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-family: " + font_name + ";";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets_impl::html_editor::selection_font_size(const float& font_size) {
			log("selection_font_size: " + std::to_string(font_size));
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-size: " + std::to_string(font_size) + "pt;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets_impl::html_editor::selection_bold() {
			log("selection_bold");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-weight: bold;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets_impl::html_editor::selection_italic() {
			log("selection_italic");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-style: italic;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets_impl::html_editor::selection_underline() {
			log("selection_underline");
			if (false) {
				// to-do: make it possible for this to work together with text-decoration: line-through
				std::vector<xml_parser::tag_attribute> tag_attributes;
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "text-decoration: underline;";
				tag_attributes.push_back(tag_attribute);
				formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
			}
			else
				formatted_text_editor().toggle_tag(specs_.text, "u", selection_info_.start, selection_info_.end);
		}

		void widgets_impl::html_editor::selection_strikethrough() {
			log("selection_strikethrough");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "text-decoration: line-through;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text, "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		UINT32
			widgets_impl::html_editor::count_characters(IDWriteTextLayout* p_text_layout, const std::string& text,
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
			widgets_impl::html_editor::get_caret_position(IDWriteTextLayout* p_text_layout, const std::string& text,
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

		std::vector<D2D1_RECT_F>
			widgets_impl::html_editor::get_selection_rects(
				IDWriteTextLayout* p_text_layout, const D2D1_RECT_F& rect_text,
				const UINT32& selection_start_, const UINT32& selection_end_) {
			std::vector<D2D1_RECT_F> selection_rects;
			DWRITE_HIT_TEST_METRICS hit_metrics_start, hit_metrics_end;

			UINT32 selection_start = smallest(selection_start_, selection_end_);
			UINT32 selection_end = largest(selection_start_, selection_end_);

			// get metrics of selection start position
			float p_x, p_y;
			bool isTrailingHit = false;
			p_text_layout->HitTestTextPosition(selection_start,
				isTrailingHit, &p_x, &p_y, &hit_metrics_start);

			// get metrics of selection end position
			p_text_layout->HitTestTextPosition(selection_end,
				isTrailingHit, &p_x, &p_y, &hit_metrics_end);

			// get line metrics
			std::unique_ptr<DWRITE_LINE_METRICS[]> p_line_metrics =
				std::unique_ptr<DWRITE_LINE_METRICS[]>(new DWRITE_LINE_METRICS [max_line_count]);
			UINT32 actual_line_count = 0;
			HRESULT hr = p_text_layout->GetLineMetrics(p_line_metrics.get(), max_line_count, &actual_line_count);

			if (SUCCEEDED(hr)) {
				// figure out which lines are covered by the selection
				struct line_info {
					DWRITE_LINE_METRICS metrics;
					UINT32 line_start_position = 0;
					UINT32 line_end_position = 0;
				};

				std::vector<line_info> lines;

				UINT32 position_count = 0;
				bool within = false;
				for (UINT32 i = 0; i < actual_line_count; i++) {
					DWRITE_LINE_METRICS m = p_line_metrics[i];
					auto line_start = position_count;
					position_count += m.length;
					auto line_end = position_count;

					if (selection_start <= line_end)
						within = true;

					bool is_last_line = selection_end <= line_end;

					if (within) {
						line_info info;
						info.metrics = m;
						info.line_start_position = lines.empty() ? selection_start : line_start;
						info.line_end_position = is_last_line ? selection_end : line_end - 1;
						lines.push_back(info);
					}

					if (is_last_line)
						break;
				}

				auto get_pos_metrics = [&](UINT32 pos) {
					float p_x, p_y;
					bool isTrailingHit = false;
					DWRITE_HIT_TEST_METRICS hit_metrics;
					p_text_layout->HitTestTextPosition(pos,
						isTrailingHit, &p_x, &p_y, &hit_metrics);
					return hit_metrics;
				};

				// get selection rects
				for (auto& info : lines) {
					auto start_metrics = get_pos_metrics(info.line_start_position);
					auto end_metrics = get_pos_metrics(info.line_end_position);

					D2D1_RECT_F rect;
					rect.left = rect_text.left + start_metrics.left;
					rect.top = rect_text.top + start_metrics.top;
					rect.right = rect_text.left + end_metrics.left;
					rect.bottom = rect.top + start_metrics.height;
					selection_rects.push_back(rect);
				}
			}

			return selection_rects;
		}

		float widgets_impl::html_editor::get_caret_width() {
			// respect user settings
			DWORD caret_width = 1;
			SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caret_width, 0);
			return static_cast<float>(caret_width);
		}

		D2D1_RECT_F
			widgets_impl::html_editor::get_caret_rect(IDWriteTextLayout* p_text_layout,
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
	}
}