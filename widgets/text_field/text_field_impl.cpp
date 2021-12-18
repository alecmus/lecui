//
// text_field_impl.cpp - text_field_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "text_field_impl.h"
#include "../../utilities/timer.h"
#include "../label/label_impl.h"

namespace liblec {
	namespace lecui {
		widgets::text_field_impl::text_field_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_caret(nullptr),
			_p_brush_prompt(nullptr),
			_p_brush_border(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_brush_input_error(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_margin_x(7.5f),
			_margin_y(2.5f),
			_caret_blink_timer_name("caret_blink_timer::text_field"),
			_caret_position(0),
			_caret_visible(true),
			_skip_blink(false),
			_text_off_set(0.f),
			_is_selecting(false),
			_is_selected(false),
			_input_error(false),
			_selection_info({ 0, 0 }) {}

		widgets::text_field_impl::~text_field_impl() { discard_resources(); }

		widgets::widget_type
			widgets::text_field_impl::type() {
			return lecui::widgets::widget_type::text_field;
		}

		HRESULT widgets::text_field_impl::create_resources(
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
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_input_error()),
					&_p_brush_input_error);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_caret()),
					&_p_brush_caret);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_prompt()),
					&_p_brush_prompt);
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
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::text_field_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_caret);
			safe_release(&_p_brush_prompt);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_brush_input_error);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::text_field_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			// make sure caret is well positioned in case text has since been changed
			_caret_position = smallest(_caret_position, static_cast<UINT32>(_specs.text().length()));

			// draw background
			D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_enabled ? _p_brush_fill :
				_p_brush_disabled);

			p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_border, .5f);

			if (!_is_static && _is_enabled && _selected)
				p_render_target->DrawRoundedRectangle(&rounded_rect, _input_error ? _p_brush_input_error : _p_brush_selected, 1.75f);

			// create a text layout
			std::string _text = !_specs.text().empty() ?
				_specs.text() : _selected ?
				std::string() : _specs.prompt();

			if (!_specs.text().empty() && _specs.mask != '\0') {
				// mask the text
				for (auto& c : _text)
					c = _specs.mask;
			}

			auto _rect_text = _rect;
			_rect_text.left += _margin_x;
			_rect_text.right -= _margin_x;
			_rect_text.top += _margin_y;
			_rect_text.bottom -= _margin_y;

			// define rectangle for clipping text
			auto _rect_text_clip = _rect;
			_rect_text_clip.left += (_margin_x / 3.f);
			_rect_text_clip.right -= (_margin_x / 3.f);
			_rect_text_clip.top += (_margin_y / 3.f);
			_rect_text_clip.bottom -= (_margin_y / 3.f);

			// define text box rect and actual text rect (with possible overflow)
			const auto _rect_text_box = _rect_text;
			_rect_text = measure_text(_p_directwrite_factory,
				_text, _specs.font(), _specs.font_size(), false, true, true, false, _rect_text);

			// measure the text up to the caret position
			const auto text_to_caret = _text.substr(0, _caret_position);
			const auto _rect_up_to_caret = measure_text(_p_directwrite_factory,
				text_to_caret, _specs.font(), _specs.font_size(), false, true, true, false, _rect_text);

			bool iterate = false;
			do {
				iterate = false;

				// compute offset
				UINT32 hidden_left = 0;
				UINT32 hidden_right = 0;
				const float off_set_right = ((_rect_text.right - _rect_text.left) - (_rect_text_box.right - _rect_text_box.left)) + _text_off_set;
				{
					HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_text).c_str(),
						(UINT32)_text.length(), _p_text_format, _rect_text.right - _rect_text.left,
						_rect_text.bottom - _rect_text.top, &_p_text_layout);

					// characters hidden to the left of text box
					const D2D1_POINT_2F pt_left = D2D1::Point2F(_rect_text.left - _text_off_set, _rect_text.top + (_rect_text.bottom - _rect_text.top) / 2.f);
					hidden_left = count_characters(_p_text_layout, _text, _rect_text, pt_left, get_dpi_scale());

					// characters hidden to the right of text box

					const D2D1_POINT_2F pt_right = D2D1::Point2F(_rect_text_box.left + off_set_right, _rect_text.top + (_rect_text.bottom - _rect_text.top) / 2.f);
					hidden_right = count_characters(_p_text_layout, _text, _rect_text, pt_right, get_dpi_scale());

					safe_release(&_p_text_layout);
				}

				const auto text_field_width = _rect_text_box.right - _rect_text_box.left;
				const auto distance_to_caret = _rect_up_to_caret.right - _rect_up_to_caret.left;
				const auto off_set_left = text_field_width - distance_to_caret;

				if (off_set_left < _text_off_set || hidden_left == 0) {
					// Either
					// 1. caret has reached far right and text is being added
					// 2. text hasn't filled text_field
					//
					// keep caret to the rightmost but within text_field (pin to the right if end is reached,
					// pushing text to the left).
					_text_off_set = off_set_left;
				}
				else {
					if (hidden_left >= _caret_position) {
						// caret has reached far left
						// prevent caret from being hidden by off-setting text by up to 40px
						_text_off_set += 40.f;
						iterate = true;
					}
					else
						if (hidden_right == 0) {
							// keep text pinned to the right as we downsize while there's some hidden on the left
							_text_off_set -= off_set_right;
						}
						else {
							// do nothing under these circumstances
							// * Text has overflown on either or both sides but caret is in the middle
							//   while user is either adding or removing text
						}
				}

				// this offset cannot be greater than zero or text will be indented!!!
				_text_off_set = smallest(_text_off_set, 0.f);
			} while (iterate);

			// apply offset to text rect to ensure visibility of caret
			_rect_text.left += _text_off_set;
			_rect_text.right += _text_off_set;

			HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_text).c_str(),
				(UINT32)_text.length(), _p_text_format, _rect_text.right - _rect_text.left,
				_rect_text.bottom - _rect_text.top, &_p_text_layout);

			if (SUCCEEDED(hr)) {
				// clip text
				auto_clip clip(render, p_render_target, _rect_text_clip, 0.f);

				// draw the text layout
				p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
					_p_text_layout, !_specs.text().empty() ?
					_p_brush : _p_brush_prompt, D2D1_DRAW_TEXT_OPTIONS_CLIP);
			}

			if (!_is_static && _is_enabled && _selected) {
				if (_hit && _pressed) {
					reset_selection();

					_caret_position = get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point, get_dpi_scale());
					_caret_visible = true;

					if (_point.x != _point_on_press.x || _point.y != _point_on_press.y) {
						// user is making a selection
						_is_selecting = true;

						auto _selection_start = get_caret_position(_p_text_layout, _specs.text(), _rect_text, _point_on_press, get_dpi_scale());
						auto _selection_end = _caret_position;

						auto rect_selection = get_selection_rect(_p_text_layout, _rect_text, _selection_start, _selection_end);
						p_render_target->FillRectangle(rect_selection, _p_brush_selected);
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

			// draw selection rectangle
			if (!_is_static && _is_enabled && _is_selected) {
				auto rect_selection = get_selection_rect(_p_text_layout, _rect_text, _selection_info.start, _selection_info.end);
				p_render_target->FillRectangle(rect_selection, _p_brush_selected);
			}

			// draw caret
			if (!_is_static && _is_enabled && _selected && _caret_visible) {
				const auto caret_rect = get_caret_rect(_p_text_layout, _rect_text, _caret_position);
				p_render_target->FillRectangle(&caret_rect, _p_brush_caret);
			}

			// release the text layout
			safe_release(&_p_text_layout);

			return _rect;
		}

		void widgets::text_field_impl::on_selection_change(const bool& selected) {
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

		widgets::text_field&
			widgets::text_field_impl::specs() { return _specs; }

		widgets::text_field&
			widgets::text_field_impl::operator()() { return specs(); }

		void widgets::text_field_impl::insert_character(const char& c) {
			try {
				if (_specs.maximum_length() > 0) {
					// limit length
					if (_specs.text().length() >= static_cast<size_t>(largest(_specs.maximum_length(), 0))) {
						_input_error = true;
						return;
					}
				}

				// check forbidden character set
				if (_specs.forbidden_characters().count(c)) {
					_input_error = true;
					return;
				}

				// check allowed character set
				if (!_specs.allowed_characters().empty() && !_specs.allowed_characters().count(c)) {
					_input_error = true;
					return;
				}

				_input_error = false;

				if (_is_selected) {
					if (_selection_info.start > _selection_info.end)
						swap(_selection_info.start, _selection_info.end);

					_caret_position = _selection_info.start;
					_specs.text().erase(_selection_info.start, _selection_info.end - _selection_info.start);
					reset_selection();
				}

				std::string s;
				s += c;
				_specs.text().insert(_caret_position, s);
				_caret_position++;
				_caret_visible = true;
				_skip_blink = true;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::text_field_impl::key_backspace() {
			_input_error = false;

			try {
				if (_is_selected) {
					if (_selection_info.start > _selection_info.end)
						swap(_selection_info.start, _selection_info.end);

					_caret_position = _selection_info.start;
					_specs.text().erase(_selection_info.start, _selection_info.end - _selection_info.start);
					reset_selection();
				}
				else {
					_specs.text().erase(_caret_position - 1, 1);
					_caret_position--;
					_caret_visible = true;
					_skip_blink = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::text_field_impl::key_delete() {
			_input_error = false;

			try {
				if (_is_selected) {
					if (_selection_info.start > _selection_info.end)
						swap(_selection_info.start, _selection_info.end);

					_caret_position = _selection_info.start;
					_specs.text().erase(_selection_info.start, _selection_info.end - _selection_info.start);
					reset_selection();
				}
				else {
					_specs.text().erase(_caret_position, 1);
					_caret_visible = true;
					_skip_blink = true;
				}
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets::text_field_impl::key_left() {
			_input_error = false;

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

		void widgets::text_field_impl::key_right() {
			_input_error = false;

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

		UINT32
			widgets::text_field_impl::count_characters(IDWriteTextLayout* p_text_layout, const std::string& text,
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
			widgets::text_field_impl::get_caret_position(IDWriteTextLayout* p_text_layout, const std::string& text,
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
			widgets::text_field_impl::get_selection_rect(
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

		float widgets::text_field_impl::get_caret_width() {
			// respect user settings
			DWORD caret_width = 1;
			SystemParametersInfo(SPI_GETCARETWIDTH, 0, &caret_width, 0);
			return static_cast<float>(caret_width);
		}

		D2D1_RECT_F
			widgets::text_field_impl::get_caret_rect(IDWriteTextLayout* p_text_layout,
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
