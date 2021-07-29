//
// label_impl.cpp - label_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "label_impl.h"

#include <boost/lexical_cast.hpp>

namespace liblec {
	namespace lecui {
		void widgets::parse_formatted_text(
			const std::string& formatted_text,
			std::string& _plain_text,
			D2D1_COLOR_F default_color,
			std::vector<formatted_text_parser::text_range_properties>& _formatting) {
			const auto props = formatted_text_parser().read(formatted_text, default_color);
			_plain_text = props.xml.plain_text;
			_formatting = props.dwrite;
		}

		void widgets::apply_formatting(
			const std::vector<formatted_text_parser::text_range_properties>& _formatting,
			ID2D1HwndRenderTarget* p_render_target,
			IDWriteTextLayout* _p_text_layout,
			bool is_enabled,
			ID2D1SolidColorBrush* p_brush_disabled) {
			// apply formatting
			for (auto& properties : _formatting) {
				// apply font
				if (!properties.font.empty())
					_p_text_layout->SetFontFamilyName(convert_string(properties.font).c_str(),
						properties.text_range);

				// apply font size
				if (properties.font_size)
					_p_text_layout->SetFontSize(convert_fontsize_to_dip(properties.font_size),
						properties.text_range);

				// apply bold, italic, underline
				if (properties.bold)
					_p_text_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, properties.text_range);
				if (properties.italic)
					_p_text_layout->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, properties.text_range);
				if (properties.underline)
					_p_text_layout->SetUnderline(TRUE, properties.text_range);
				if (properties.strikethrough)
					_p_text_layout->SetStrikethrough(TRUE, properties.text_range);

				// apply color (-1.f for alpha indicates color has't been set)
				if (p_render_target && properties.color.a != -1.f) {
					ID2D1SolidColorBrush* p_brush;
					HRESULT hr = p_render_target->CreateSolidColorBrush(properties.color, &p_brush);
					if (SUCCEEDED(hr))
						_p_text_layout->SetDrawingEffect(is_enabled ?
							p_brush : p_brush_disabled, properties.text_range);
					safe_release(&p_brush);
				}
			}
		}

		D2D1_RECT_F
			widgets::measure_label(IDWriteFactory* p_directwrite_factory,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				const D2D1_RECT_F max_rect) {
			// the default color doesn't matter here we're just measuring the text
			std::string _plain_text;
			std::vector<formatted_text_parser::text_range_properties> _formatting;
			parse_formatted_text(formatted_text, _plain_text, D2D1::ColorF(D2D1::ColorF::Black), _formatting);

			D2D1_RECT_F rect = max_rect;

			HRESULT hr = S_OK;

			// Create a DirectWrite text format object.
			IDWriteTextFormat* _p_text_format = nullptr;
			if (SUCCEEDED(hr)) {
				hr = p_directwrite_factory->CreateTextFormat(convert_string(font).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(font_size), L"", //locale
					&_p_text_format);
			}

			IDWriteTextLayout* _p_text_layout = nullptr;
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(center_h ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(center_v ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				// create a text layout
				hr = p_directwrite_factory->CreateTextLayout(convert_string(_plain_text).c_str(),
					(UINT32)_plain_text.length(), _p_text_format, rect.right - rect.left,
					rect.bottom - rect.top, &_p_text_layout);
			}

			if (SUCCEEDED(hr)) {
				apply_formatting(_formatting, nullptr, _p_text_layout, true, nullptr);

				DWRITE_TEXT_METRICS textMetrics;
				_p_text_layout->GetMetrics(&textMetrics);
				rect.left += textMetrics.left;
				rect.top += textMetrics.top;
				rect.right = smallest(rect.left + textMetrics.width, rect.right);
				rect.bottom = smallest(rect.top + textMetrics.height, rect.bottom);
			}

			// release the text layout
			safe_release(&_p_text_layout);
			safe_release(&_p_text_format);
			return rect;
		}

		D2D1_RECT_F
			widgets::measure_text(IDWriteFactory* p_directwrite_factory,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				bool allow_h_overflow,
				bool allow_v_overflow,
				const D2D1_RECT_F max_rect) {
			// the default color doesn't matter here we're just measuring the text
			std::string _plain_text;
			std::vector<formatted_text_parser::text_range_properties> _formatting;
			parse_formatted_text(formatted_text, _plain_text, D2D1::ColorF(D2D1::ColorF::Black), _formatting);

			D2D1_RECT_F rect = max_rect;

			HRESULT hr = S_OK;

			// Create a DirectWrite text format object.
			IDWriteTextFormat* _p_text_format = nullptr;
			if (SUCCEEDED(hr)) {
				hr = p_directwrite_factory->CreateTextFormat(convert_string(font).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(font_size), L"", //locale
					&_p_text_format);
			}

			if (SUCCEEDED(hr) && allow_h_overflow) {
				// make text overflow layout rectangle
				_p_text_format->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
			}

			IDWriteTextLayout* _p_text_layout = nullptr;
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(center_h ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(center_v ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				// create a text layout
				hr = p_directwrite_factory->CreateTextLayout(convert_string(_plain_text).c_str(),
					(UINT32)_plain_text.length(), _p_text_format, rect.right - rect.left,
					rect.bottom - rect.top, &_p_text_layout);
			}

			if (SUCCEEDED(hr)) {
				apply_formatting(_formatting, nullptr, _p_text_layout, true, nullptr);

				DWRITE_TEXT_METRICS textMetrics;
				_p_text_layout->GetMetrics(&textMetrics);
				rect.left += textMetrics.left;
				rect.top += textMetrics.top;

				if (allow_h_overflow)
					rect.right = rect.left + textMetrics.width;
				else
					rect.right = smallest(rect.left + textMetrics.width, rect.right);

				if (allow_v_overflow)
					rect.bottom = rect.top + textMetrics.height;
				else
					rect.bottom = smallest(rect.top + textMetrics.height, rect.bottom);
			}

			// release the text layout
			safe_release(&_p_text_layout);
			safe_release(&_p_text_format);
			return rect;
		}

		widgets::label_impl::label_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_hot_pressed(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr) {}

		widgets::label_impl::~label_impl() { discard_resources(); }

		widgets::widget_type
			widgets::label_impl::type() {
			return lecui::widgets::widget_type::label;
		}

		HRESULT widgets::label_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			parse_formatted_text(_specs.text(), _text, convert_color(_specs.color_text()), _formatting);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot_pressed()),
					&_p_brush_hot_pressed);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = _p_directwrite_factory->CreateTextFormat(convert_string(_specs.font()).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(_specs.font_size()), L"", //locale
					&_p_text_format);
			}
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(_specs.center_h() ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(_specs.center_v() ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				if (!_specs.multiline())
					make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::label_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_hot_pressed);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::label_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			// create a text layout
			HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_text).c_str(),
				(UINT32)_text.length(), _p_text_format, _rect.right - _rect.left,
				_rect.bottom - _rect.top, &_p_text_layout);

			DWRITE_TEXT_METRICS textMetrics;

			if (SUCCEEDED(hr)) {
				apply_formatting(_formatting, p_render_target, _p_text_layout, _is_enabled,
					_p_brush_disabled);

				_p_text_layout->GetMetrics(&textMetrics);
				const auto rect_text = _rect;

				_rect.left += textMetrics.left;
				_rect.top += textMetrics.top;
				_rect.right = smallest(_rect.left + textMetrics.width, _rect.right);
				_rect.bottom = smallest(_rect.top + textMetrics.height, _rect.bottom);

				if (render && _visible) {
					if (!_is_static && _is_enabled) {
						if (_hit || _pressed)
							p_render_target->FillRectangle(&_rect, _pressed ?
								_p_brush_hot_pressed : _p_brush_hot);

						if (_selected)
							p_render_target->FillRectangle(&_rect, _p_brush_selected);
					}

					// draw the text layout
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text.left, rect_text.top },
						_p_text_layout, _is_enabled ?
						_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}
			}

			// release the text layout
			safe_release(&_p_text_layout);

			return _rect;
		}

		widgets::label&
			widgets::label_impl::specs() {
			return _specs;
		}

		widgets::label&
			widgets::label_impl::operator()() {
			return specs();
		}
	}
}
