//
// html_editor_impl.cpp - html_editor_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "html_editor_impl.h"
#include "../../utilities/timer.h"
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
			_controls_initialized(false),
			_p_brush(nullptr),
			_p_brush_caret(nullptr),
			_p_brush_border(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_scroll_amount(0.f),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_margin_x(7.5f),
			_margin_y(2.5f),
			_caret_blink_timer_name("caret_blink_timer::html_editor"),
			_autoscroll_timer_name("autoscroll_timer::html_editor"),
			_caret_position(0),
			_caret_visible(true),
			_skip_blink(false),
			_is_selecting(false),
			_is_selected(false),
			_key_up_scheduled(false),
			_key_down_scheduled(false),
			_selection_info({ 0, 0 }),
			_last_color({ 255, 0, 0, 255 }) {}

		widgets::html_editor_impl::~html_editor_impl() { discard_resources(); }

		bool widgets::html_editor_impl::controls_initialized() {
			return _controls_initialized;
		}

		void widgets::html_editor_impl::initialize_controls(bool init) {
			_controls_initialized = init;
		}

		widgets::widget_type
			widgets::html_editor_impl::type() {
			return lecui::widgets::widget_type::html_editor;
		}

		HRESULT widgets::html_editor_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_caret()),
					&_p_brush_caret);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = _p_directwrite_factory->CreateTextFormat(
					convert_string(_specs.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(_specs.font_size()),
					L"", //locale
					&_p_text_format
					);
			}
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::html_editor_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_caret);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::html_editor_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;

				try {
					if (_html_pane_specs.has_value()) {
						// update the special pane widget
						_html_pane_specs.value().get().color_fill() = _specs.color_fill();
						_html_pane_specs.value().get().color_border() = _specs.color_border();
					}

					if (_html_control_pane_specs.has_value()) {
						// update the special pane widget
						_html_control_pane_specs.value().get().color_fill() = _specs.color_control_fill();
						_html_control_pane_specs.value().get().color_border() = _specs.color_control_border();
					}

					// schedule a refresh
					_page._d_page.get_form()._d._schedule_refresh = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// use _specs._rect not _specs.rect() and _specs._on_resize not _specs.on_resize() due to redirection to special pane
			_rect = position(_specs._rect, _specs._on_resize, change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			// make sure caret is well positioned in case text has since been changed
			_caret_position = smallest(_caret_position, static_cast<UINT32>(_specs.text().length()));

			D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			// create a text layout
			std::string _text = _specs.text();

			auto _rect_text = _rect;

			// measure the text
			D2D1_RECT_F rect_optimal = _rect_text;

			// get formatting

			// to-do: performance issues ... only redo if there's been a change
			if (true) {
				parse_formatted_text(_specs.text(), _text, convert_color(_specs.color_text()), _formatting);
			}

			HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_text).c_str(),
				(UINT32)_text.length(), _p_text_format, _rect_text.right - _rect_text.left,
				_rect_text.bottom - _rect_text.top, &_p_text_layout);

			DWRITE_TEXT_METRICS textMetrics;

			if (SUCCEEDED(hr)) {
				// apply formatting
				apply_formatting(_formatting, p_render_target, _p_text_layout, _is_enabled,
					_p_brush_disabled);

				_p_text_layout->GetMetrics(&textMetrics);

				DWRITE_TEXT_METRICS textMetrics;
				_p_text_layout->GetMetrics(&textMetrics);
				rect_optimal.left += textMetrics.left;
				rect_optimal.top += textMetrics.top;

				// wrap text horizontally
				rect_optimal.right = smallest(rect_optimal.left + textMetrics.width, rect_optimal.right);

				// allow text to overflow vertically, scroll bars will do the rest
				rect_optimal.bottom = rect_optimal.top + textMetrics.height;

				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
					_p_text_layout, _p_brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			const auto _optimized_bottom = rect_optimal.bottom + _margin_y;

			if (!_is_static && _is_enabled && _selected) {
				if (_key_up_scheduled || _key_down_scheduled) {
					// get selection rects of entire text area
					const UINT32 start = 0;
					const UINT32 end = static_cast<UINT32>(_text.length());

					auto full_text_selection_rects = get_selection_rects(_p_text_layout,
						_rect_text, start, end);

					// get actual selection rects
					std::vector<D2D1_RECT_F> selection_rects;

					bool end_special_case = false;	// for handling end special case on key_up

					if (_is_selected) {
						selection_rects = get_selection_rects(_p_text_layout,
							_rect_text, _selection_info.start, _selection_info.end);
					}
					else {
						selection_info sel_info;

						end_special_case = _caret_position == _text.length();
						
						if (_key_up_scheduled) {
							auto pos = _caret_position;
							if (pos == _text.length())
								pos--;
							sel_info.start = pos;
							sel_info.end = sel_info.start + 1;
						}
						else {
							// to-do: fix accordingly
							auto pos = _caret_position;
							if (pos > 0)
								pos--;
							sel_info.start = pos;
							sel_info.end = sel_info.start + 1;
						}

						selection_rects = get_selection_rects(_p_text_layout,
							_rect_text, sel_info.start, sel_info.end);
					}

					if (_key_up_scheduled) {
						// move one line above
						D2D1_RECT_F rect_above = full_text_selection_rects[0];

						for (auto& rect : full_text_selection_rects) {
							if (rect.top == selection_rects[0].top)
								break;

							rect_above = rect;
						}

						// find corresponding point within this line for caret
						D2D1_POINT_2F pt = { (end_special_case ? selection_rects[0].right : selection_rects[0].left) * get_dpi_scale(), (rect_above.top + ((rect_above.bottom - rect_above.top) / 2.f)) * get_dpi_scale() };
						_caret_position = get_caret_position(_p_text_layout, _text, _rect_text,
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
						_caret_position = get_caret_position(_p_text_layout, _text, _rect_text,
							pt, get_dpi_scale());

						reset_selection();
					}

					_key_up_scheduled = false;
					_key_down_scheduled = false;
				}
				else {
					if (_hit && _pressed) {
						reset_selection();

						_caret_position = get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point, get_dpi_scale());
						_caret_visible = true;

						if (_point.x != _point_on_press.x || _point.y != _point_on_press.y) {
							// user is making a selection
							_is_selecting = true;

							auto _selection_start = get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point_on_press, get_dpi_scale());
							auto _selection_end = _caret_position;

							auto selection_rects = get_selection_rects(_p_text_layout, _rect_text, _selection_start, _selection_end);
							for (auto selection_rect : selection_rects)
								p_render_target->FillRectangle(selection_rect, _p_brush_selected);
						}
					}
					else
						if (!_pressed && _is_selecting) {
							// user is done with the selection
							_is_selecting = false;

							set_selection(
								get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point_on_press, get_dpi_scale()),
								get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point_on_release, get_dpi_scale()));
						}
				}
			}

			// draw selection rectangles
			if (!_is_static && _is_enabled && _is_selected) {
				auto selection_rects = get_selection_rects(_p_text_layout, _rect_text, _selection_info.start, _selection_info.end);
				for (auto selection_rect : selection_rects)
					p_render_target->FillRectangle(selection_rect, _p_brush_selected);
			}

			float move_v = 0.f;

			// draw caret
			if (!_is_static && _is_enabled && _selected && _caret_visible) {
				const auto caret_rect = get_caret_rect(_p_text_layout, _rect_text, _caret_position);
				p_render_target->FillRectangle(&caret_rect, _p_brush_caret);

				// figure out if caret is within visible area
				const auto pg_rect = _page._d_page.get_rect();

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
			safe_release(&_p_text_layout);

			const auto old_height = _rect.bottom - _rect.top;
			const auto height = _optimized_bottom - _rect.top;

			// update widget rect
			_specs._rect.height(height);

			// move rect to ensure caret visibility
			if (move_v && !timer_manager(get_form()).running(_autoscroll_timer_name)) {
				log("move_v: " + std::to_string(move_v));
				_scroll_amount = move_v;

				timer_manager(get_form()).add(_autoscroll_timer_name, 0,
					[&]() {
						// scroll page to ensure caret visibility
						_page._d_page.scroll_vertically(_scroll_amount);
						get_form().update();

						// stop timer
						timer_manager(get_form()).stop(_autoscroll_timer_name);
					});
			}

			return _rect;
		}

		void widgets::html_editor_impl::on_selection_change(const bool& selected) {
			if (selected) {
				// start blink timer
				timer_manager(get_form()).add(_caret_blink_timer_name, 500,
					[&]() {
						if (_skip_blink)
							_skip_blink = false;
						else {
							_caret_visible = !_caret_visible;
							get_form().update();
						}
					});
			}
			else {
				// stop blink timer
				timer_manager(get_form()).stop(_caret_blink_timer_name);
			}
		}

		widgets::html_editor&
			widgets::html_editor_impl::specs() { return _specs; }

		widgets::html_editor&
			widgets::html_editor_impl::operator()() { return specs(); }

		void widgets::html_editor_impl::set_pane_specs(containers::pane_specs& html_control,
			containers::pane_specs& html) {
			_html_control_pane_specs = html_control;
			_html_pane_specs = html;
		}

		// to-do: insertion mechanics for formatted text
		void widgets::html_editor_impl::insert_character(const char& c) {
			if (!_specs.view_only) {
				try {
					unsigned long tag_number = 0;
					if (_is_selected) {
						if (_selection_info.start > _selection_info.end)
							swap(_selection_info.start, _selection_info.end);

						_caret_position = _selection_info.start;
						tag_number = formatted_text_editor().erase(_selection_info.start, _selection_info.end, _specs.text());
						reset_selection();
					}

					formatted_text_editor().insert_character(c, _caret_position, tag_number, _specs.text());
					_caret_position++;
					_caret_visible = true;
					_skip_blink = true;
				}
				catch (const std::exception& e) { log(e.what()); }
			}

			// force scroll bar to set, in case caret is now hidden
			_page._d_page.force_scrollbar_set();
		}

		// to-do: backspace mechanics for formatted text
		void widgets::html_editor_impl::key_backspace() {
			if (!_specs.view_only) {
				try {
					unsigned long tag_number = 0;
					if (_is_selected) {
						if (_selection_info.start > _selection_info.end)
							swap(_selection_info.start, _selection_info.end);

						_caret_position = _selection_info.start;
						tag_number = formatted_text_editor().erase(_selection_info.start, _selection_info.end, _specs.text());
						reset_selection();
					}
					else {
						tag_number = formatted_text_editor().erase(_caret_position - 1, _caret_position, _specs.text());
						_caret_position--;
						_caret_visible = true;
						_skip_blink = true;
					}
				}
				catch (const std::exception& e) { log(e.what()); }
			}
		}

		// to-do: deletion mechanics for formatted text
		void widgets::html_editor_impl::key_delete() {
			if (!_specs.view_only) {
				try {
					unsigned long tag_number = 0;
					if (_is_selected) {
						if (_selection_info.start > _selection_info.end)
							swap(_selection_info.start, _selection_info.end);

						_caret_position = _selection_info.start;
						tag_number = formatted_text_editor().erase(_selection_info.start, _selection_info.end, _specs.text());
						reset_selection();
					}
					else {
						tag_number = formatted_text_editor().erase(_caret_position, _caret_position + 1, _specs.text());
						_caret_visible = true;
						_skip_blink = true;
					}
				}
				catch (const std::exception& e) { log(e.what()); }
			}
		}

		void widgets::html_editor_impl::key_left() {
			try {
				if (_is_selected) {
					if (_selection_info.start > _selection_info.end)
						swap(_selection_info.start, _selection_info.end);

					_caret_position = _selection_info.start + 1;
					reset_selection();
				}

				if (_caret_position > 0)
					_caret_position--;

				_caret_visible = true;
				_skip_blink = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::html_editor_impl::key_right() {
			try {
				if (_is_selected) {
					if (_selection_info.start > _selection_info.end)
						swap(_selection_info.start, _selection_info.end);

					_caret_position = _selection_info.end - 1;
					reset_selection();
				}

				if (_caret_position < _specs.text().length())
					_caret_position++;

				_caret_visible = true;
				_skip_blink = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::html_editor_impl::key_up() {
			_key_up_scheduled = true;

			_caret_visible = true;
			_skip_blink = true;
		}

		void widgets::html_editor_impl::key_down() {
			_key_down_scheduled = true;

			_caret_visible = true;
			_skip_blink = true;
		}

		void widgets::html_editor_impl::select_all() {
			// move caret to the end
			_caret_position = static_cast<UINT32>(_specs.text().length());

			// select all text
			set_selection(0, _caret_position);
		}

		void widgets::html_editor_impl::selection_font(const std::string& font_name) {
			log("selection_font: " + font_name);
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-family: " + font_name + ";";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
		}

		void widgets::html_editor_impl::selection_font_size(const float& font_size) {
			log("selection_font_size: " + std::to_string(font_size));
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-size: " + std::to_string(font_size) + "pt;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
		}

		void widgets::html_editor_impl::selection_bold() {
			log("selection_bold");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-weight: bold;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
		}

		void widgets::html_editor_impl::selection_italic() {
			log("selection_italic");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "font-style: italic;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
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
				formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
			}
			else
				formatted_text_editor().toggle_tag(_specs.text(), "u", _selection_info.start, _selection_info.end);
		}

		void widgets::html_editor_impl::selection_strikethrough() {
			log("selection_strikethrough");
			std::vector<xml_parser::tag_attribute> tag_attributes;
			xml_parser::tag_attribute tag_attribute;
			tag_attribute.name = "style";
			tag_attribute.value = "text-decoration: line-through;";
			tag_attributes.push_back(tag_attribute);
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
		}

		void widgets::html_editor_impl::selection_color() {
			selection_color(_last_color);
		}

		void widgets::html_editor_impl::selection_color(const color& font_color) {
			_last_color = font_color;

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
			formatted_text_editor().toggle_tag(_specs.text(), "span", tag_attributes, _selection_info.start, _selection_info.end);
		}

		color widgets::html_editor_impl::get_last_color() {
			return _last_color;
		}

		void widgets::html_editor_impl::reset_selection() {
			_selection_info = { 0, 0 };
			_is_selected = false;
		}

		void widgets::html_editor_impl::set_selection(const UINT start, const UINT end) {
			_selection_info.start = start;
			_selection_info.end = end;
			_is_selected = true;
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
				const UINT32& _selection_start, const UINT32& _selection_end) {
			std::vector<D2D1_RECT_F> selection_rects;
			DWRITE_HIT_TEST_METRICS hit_metrics_start, hit_metrics_end;

			UINT32 selection_start = smallest(_selection_start, _selection_end);
			UINT32 selection_end = largest(_selection_start, _selection_end);

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
