//
// html_editor_impl.cpp - html_editor_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "html_editor_impl.h"
#include "../../timer.h"
#include "../label/label_impl.h"
#include "../../formatted_text_editor/formatted_text_editor.h"
#include "../../form_impl/form_impl.h"
#include "../../containers/page/page_impl.h"
#include <memory>

namespace liblec {
	namespace lecui {
		constexpr UINT32 max_line_count = 16384;	// 2^14

		std::string widgets::html_editor_impl::alias_font() {
			return std::string("lecui::html_editor::font");
		}

		std::string widgets::html_editor_impl::alias_font_size() {
			return std::string("lecui::html_editor::font_size");
		}

		std::string widgets::html_editor_impl::alias_bold() {
			return std::string("lecui::html_editor::bold");
		}

		std::string widgets::html_editor_impl::alias_italic() {
			return std::string("lecui::html_editor::italic");
		}

		std::string widgets::html_editor_impl::alias_underline() {
			return std::string("lecui::html_editor::underline");
		}

		std::string widgets::html_editor_impl::alias_strikethrough() {
			return std::string("lecui::html_editor::strikethrough");
		}

		std::string widgets::html_editor_impl::alias_font_color() {
			return std::string("lecui::html_editor::font_color");
		}

		std::string widgets::html_editor_impl::alias_font_color_bar() {
			return std::string("lecui::html_editor::font_color_bar");
		}

		widgets::html_editor_impl::html_editor_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			controls_initialized_(false),
			p_brush_(nullptr),
			p_brush_caret_(nullptr),
			p_brush_border_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			scroll_amount_(0.f),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			margin_x_(7.5f),
			margin_y_(2.5f),
			caret_blink_timer_name_("caret_blink_timer::html_editor"),
			autoscroll_timer_name_("autoscroll_timer::html_editor"),
			caret_position_(0),
			caret_visible_(true),
			skip_blink_(false),
			is_selecting_(false),
			is_selected_(false),
			key_up_scheduled_(false),
			key_down_scheduled_(false),
			selection_info_({ 0, 0 }),
			last_color_({ 255, 0, 0, 255 }) {}

		widgets::html_editor_impl::~html_editor_impl() { discard_resources(); }

		bool widgets::html_editor_impl::controls_initialized() {
			return controls_initialized_;
		}

		void widgets::html_editor_impl::initialize_controls(bool init) {
			controls_initialized_ = init;
		}

		widgets::widget_type
			widgets::html_editor_impl::type() {
			return lecui::widgets::widget_type::html_editor;
		}

		HRESULT widgets::html_editor_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = false;
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border()),
					&p_brush_border_);
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_caret()),
					&p_brush_caret_);
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
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets::html_editor_impl::discard_resources() {
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
			widgets::html_editor_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;

				try {
					if (html_pane_specs_.has_value()) {
						// update the special pane specs
						html_pane_specs_.value().get().color_fill() = specs_.color_fill();
						html_pane_specs_.value().get().color_border() = specs_.color_border();
					}

					if (html_control_pane_specs_.has_value()) {
						// update the special pane specs
						html_control_pane_specs_.value().get().color_fill() = specs_.color_control_fill();
						html_control_pane_specs_.value().get().color_border() = specs_.color_control_border();
					}

					// schedule a refresh
					page_.d_page_.get_form().d_.schedule_refresh_ = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect_, specs_.on_resize_, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			// make sure caret is well positioned in case text has since been changed
			caret_position_ = smallest(caret_position_, static_cast<UINT32>(specs_.text().length()));

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x(), specs_.corner_radius_y() };

			// create a text layout
			std::string text_ = specs_.text();

			auto rect_text_ = rect_;

			// measure the text
			D2D1_RECT_F rect_optimal = rect_text_;

			// get formatting

			// to-do: performance issues ... only redo if there's been a change
			if (true) {
				parse_formatted_text(specs_.text(), text_, convert_color(specs_.color_text()), formatting_);
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
				if (key_up_scheduled_ || key_down_scheduled_) {
					// get selection rects of entire text area
					const UINT32 start = 0;
					const UINT32 end = static_cast<UINT32>(text_.length());

					auto full_text_selection_rects = get_selection_rects(p_text_layout_,
						rect_text_, start, end);

					// get actual selection rects
					std::vector<D2D1_RECT_F> selection_rects;

					bool end_special_case = false;	// for handling end special case on key_up

					if (is_selected_) {
						selection_rects = get_selection_rects(p_text_layout_,
							rect_text_, selection_info_.start, selection_info_.end);
					}
					else {
						selection_info sel_info;

						end_special_case = caret_position_ == text_.length();
						
						if (key_up_scheduled_) {
							auto pos = caret_position_;
							if (pos == text_.length())
								pos--;
							sel_info.start = pos;
							sel_info.end = sel_info.start + 1;
						}
						else {
							// to-do: fix accordingly
							auto pos = caret_position_;
							if (pos > 0)
								pos--;
							sel_info.start = pos;
							sel_info.end = sel_info.start + 1;
						}

						selection_rects = get_selection_rects(p_text_layout_,
							rect_text_, sel_info.start, sel_info.end);
					}

					if (key_up_scheduled_) {
						// move one line above
						D2D1_RECT_F rect_above = full_text_selection_rects[0];

						for (auto& rect : full_text_selection_rects) {
							if (rect.top == selection_rects[0].top)
								break;

							rect_above = rect;
						}

						// find corresponding point within this line for caret
						D2D1_POINT_2F pt = { (end_special_case ? selection_rects[0].right : selection_rects[0].left) * get_dpi_scale(), (rect_above.top + ((rect_above.bottom - rect_above.top) / 2.f)) * get_dpi_scale() };
						caret_position_ = get_caret_position(p_text_layout_, text_, rect_text_,
							pt, get_dpi_scale());

						reset_selection();
					}
					else {
						// move one line below
						D2D1_RECT_F rect_below = full_text_selection_rects[full_text_selection_rects.size() - 1];

						for (auto& rect : full_text_selection_rects) {
							rect_below = rect;

							if (rect.bottom > selection_rects[selection_rects.size() - 1].bottom)
								break;
						}

						// find corresponding point within this line for caret
						D2D1_POINT_2F pt = { selection_rects[selection_rects.size() - 1].right * get_dpi_scale(), (rect_below.top + ((rect_below.bottom - rect_below.top) / 2.f)) * get_dpi_scale() };
						caret_position_ = get_caret_position(p_text_layout_, text_, rect_text_,
							pt, get_dpi_scale());

						reset_selection();
					}

					key_up_scheduled_ = false;
					key_down_scheduled_ = false;
				}
				else {
					if (hit_ && pressed_) {
						reset_selection();

						caret_position_ = get_caret_position(p_text_layout_, specs_.text(), rect_text_, point_, get_dpi_scale());
						caret_visible_ = true;

						if (point_.x != point_on_press_.x || point_.y != point_on_press_.y) {
							// user is making a selection
							is_selecting_ = true;

							auto selection_start_ = get_caret_position(p_text_layout_, specs_.text(), rect_text_, point_on_press_, get_dpi_scale());
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
								get_caret_position(p_text_layout_, specs_.text(), rect_text_, point_on_press_, get_dpi_scale()),
								get_caret_position(p_text_layout_, specs_.text(), rect_text_, point_on_release_, get_dpi_scale()));
						}
				}
			}

			// draw selection rectangles
			if (!is_static_ && is_enabled_ && is_selected_) {
				auto selection_rects = get_selection_rects(p_text_layout_, rect_text_, selection_info_.start, selection_info_.end);
				for (auto selection_rect : selection_rects)
					p_render_target->FillRectangle(selection_rect, p_brush_selected_);
			}

			float move_v = 0.f;

			// draw caret
			if (!is_static_ && is_enabled_ && selected_ && caret_visible_) {
				const auto caret_rect = get_caret_rect(p_text_layout_, rect_text_, caret_position_);
				p_render_target->FillRectangle(&caret_rect, p_brush_caret_);

				// figure out if caret is within visible area
				const auto pg_rect = page_.d_page_.get_rect();

				if (!(pg_rect.left <= caret_rect.right &&
					pg_rect.top <= caret_rect.top &&
					pg_rect.right >= caret_rect.left &&
					pg_rect.bottom >= caret_rect.bottom)) {

					// handle vertical hiding
					if (!(pg_rect.top <= caret_rect.top &&
						pg_rect.bottom >= caret_rect.bottom)) {
						if (!(pg_rect.top <= caret_rect.top)) {
							// caret hidden at the top

							// compute how much to move downwards
							float move_downwards = pg_rect.top - caret_rect.top;
							move_v = move_downwards + 1.f;
						}
						else {
							// caret hidden at the bottom

							// compute how much to move upwards
							float move_updwards = caret_rect.bottom - pg_rect.bottom;
							move_v = 0.f - move_updwards - 1.f;
						}
					}
				}
			}

			// release the text layout
			safe_release(&p_text_layout_);

			const auto old_height = rect_.bottom - rect_.top;
			const auto height = optimized_bottom_ - rect_.top;

			// update widget rect
			specs_.rect_.height(height);

			// move rect to ensure caret visibility
			if (move_v && !timer_management(get_form()).running(autoscroll_timer_name_)) {
				log("move_v: " + std::to_string(move_v));
				scroll_amount_ = move_v;

				timer_management(get_form()).add(autoscroll_timer_name_, 0,
					[&]() {
						// scroll page to ensure caret visibility
						page_.d_page_.scroll(scroll_amount_);
						get_form().update();

						// stop timer
						timer_management(get_form()).stop(autoscroll_timer_name_);
					});
			}

			return rect_;
		}

		void widgets::html_editor_impl::on_selection_change(const bool& selected) {
			if (selected) {
				// start blink timer
				timer_management(get_form()).add(caret_blink_timer_name_, 500,
					[&]() {
						if (skip_blink_)
							skip_blink_ = false;
						else {
							caret_visible_ = !caret_visible_;
							get_form().update();
						}
					});
			}
			else {
				// stop blink timer
				timer_management(get_form()).stop(caret_blink_timer_name_);
			}
		}

		widgets::html_editor_specs&
			widgets::html_editor_impl::specs() { return specs_; }

		widgets::html_editor_specs&
			widgets::html_editor_impl::operator()() { return specs(); }

		void widgets::html_editor_impl::set_pane_specs(containers::pane_specs& html_control,
			containers::pane_specs& html) {
			html_control_pane_specs_ = html_control;
			html_pane_specs_ = html;
		}

		// to-do: insertion mechanics for formatted text
		void widgets::html_editor_impl::insert_character(const char& c) {
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text());
					reset_selection();
				}

				formatted_text_editor().insert_character(c, caret_position_, tag_number, specs_.text());
				caret_position_++;
				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }

			// force scroll bar to set, in case caret is now hidden
			page_.d_page_.force_scrollbar_set();
		}

		// to-do: backspace mechanics for formatted text
		void widgets::html_editor_impl::key_backspace() {
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text());
					reset_selection();
				}
				else {
					tag_number = formatted_text_editor().erase(caret_position_ - 1, caret_position_, specs_.text());
					caret_position_--;
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		// to-do: deletion mechanics for formatted text
		void widgets::html_editor_impl::key_delete() {
			try {
				unsigned long tag_number = 0;
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.start;
					tag_number = formatted_text_editor().erase(selection_info_.start, selection_info_.end, specs_.text());
					reset_selection();
				}
				else {
					tag_number = formatted_text_editor().erase(caret_position_, caret_position_ + 1, specs_.text());
					caret_visible_ = true;
					skip_blink_ = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::html_editor_impl::key_left() {
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

		void widgets::html_editor_impl::key_right() {
			try {
				if (is_selected_) {
					if (selection_info_.start > selection_info_.end)
						swap(selection_info_.start, selection_info_.end);

					caret_position_ = selection_info_.end - 1;
					reset_selection();
				}

				if (caret_position_ < specs_.text().length())
					caret_position_++;

				caret_visible_ = true;
				skip_blink_ = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::html_editor_impl::key_up() {
			key_up_scheduled_ = true;

			caret_visible_ = true;
			skip_blink_ = true;
		}

		void widgets::html_editor_impl::key_down() {
			key_down_scheduled_ = true;

			caret_visible_ = true;
			skip_blink_ = true;
		}

		void widgets::html_editor_impl::selection_font(const std::string& font_name) {
			log("selection_font: " + font_name);
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-family: " + font_name + ";";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_font_size(const float& font_size) {
			log("selection_font_size: " + std::to_string(font_size));
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-size: " + std::to_string(font_size) + "pt;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_bold() {
			log("selection_bold");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-weight: bold;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_italic() {
			log("selection_italic");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-style: italic;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_underline() {
			log("selection_underline");
			if (false) {
				// to-do: make it possible for this to work together with text-decoration: line-through
				std::vector<xml_parser::tag_attribute> tag_attributes;
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "text-decoration: underline;";
				tag_attributes.push_back(tag_attribute);
				formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
			}
			else
				formatted_text_editor().toggle_tag(specs_.text(), "u", selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_strikethrough() {
			log("selection_strikethrough");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "text-decoration: line-through;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		void widgets::html_editor_impl::selection_color() {
			selection_color(last_color_);
		}

		void widgets::html_editor_impl::selection_color(const color& font_color) {
			last_color_ = font_color;

			std::string color_string = "rgb(" +
				std::to_string(font_color.get_red()) + ", "
				+ std::to_string(font_color.get_green()) + ", "
				+ std::to_string(font_color.get_blue()) + ")";
			log("selection_color: " + color_string);
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "color: " + color_string + ";";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(specs_.text(), "span", tag_attributes, selection_info_.start, selection_info_.end);
		}

		color widgets::html_editor_impl::get_last_color() {
			return last_color_;
		}

		void widgets::html_editor_impl::reset_selection() {
			selection_info_ = { 0, 0 };
			is_selected_ = false;
		}

		void widgets::html_editor_impl::set_selection(const UINT start, const UINT end) {
			selection_info_.start = start;
			selection_info_.end = end;
			is_selected_ = true;
		}

		UINT32
			widgets::html_editor_impl::count_characters(IDWriteTextLayout* p_text_layout, const std::string& text,
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
			widgets::html_editor_impl::get_caret_position(IDWriteTextLayout* p_text_layout, const std::string& text,
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
			widgets::html_editor_impl::get_selection_rects(
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
					DWRITE_LINE_METRICS metrics = { 0 };
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

					if (selection_start < line_end)
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

		float widgets::html_editor_impl::get_caret_width() {
			// respect user settings
			DWORD caret_width = 1;
			SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caret_width, 0);
			return static_cast<float>(caret_width);
		}

		D2D1_RECT_F
			widgets::html_editor_impl::get_caret_rect(IDWriteTextLayout* p_text_layout,
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
