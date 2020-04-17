/*
** label_impl.cpp - label_impl implementation
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

#include "label_impl.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

namespace liblec {
	namespace lecui {
		void widgets_impl::parse_formatted_text(
			const std::string& formatted_text,
			std::string& plain_text_,
			D2D1_COLOR_F default_color,
			std::vector<text_range_properties>& formatting_) {
			try {
				plain_text_.clear();
				formatting_.clear();
				std::string text = formatted_text;

				do {
					const std::string start_marker = "<text";
					const std::string end_marker = "</text>";

					auto start_pos = text.find(start_marker);
					auto end_pos = text.find(end_marker);

					if (start_pos == std::string::npos || end_pos == std::string::npos)
						break;

					// get the xml
					std::string xml = text.substr(start_pos, end_pos + end_marker.length() - start_pos);

					// create an empty property tree object
					boost::property_tree::ptree tree;

					// parse the xml into the property tree
					std::stringstream ss;
					ss << xml;
					boost::property_tree::read_xml(ss, tree);

					// get the value of the text
					std::string value = tree.get<std::string>("text");

					// erase the xml from the text
					text.erase(start_pos, end_pos + end_marker.length() - start_pos);

					// insert the value
					text.insert(start_pos, value);

					// capture text properties
					text_range_properties properties;

					// capture text range
					properties.text_range = { static_cast<UINT32>(start_pos),
						static_cast<UINT32>(value.length()) };

					auto get_attribute = [&](const std::string& attribute) {
						try { return tree.get<std::string>(attribute); }
						catch (const std::exception&) {}
						return std::string();
					};

					// capture font name, size
					properties.font = get_attribute("text.<xmlattr>.font");

					try {
						properties.size = boost::lexical_cast<float>(get_attribute("text.<xmlattr>.size"));
					}
					catch (const std::exception&) {}

					// capture bold, italic, underline properties
					properties.bold = get_attribute("text.<xmlattr>.bold") == "true";
					properties.italic = get_attribute("text.<xmlattr>.italic") == "true";
					properties.underline = get_attribute("text.<xmlattr>.underline") == "true";

					auto get_color = [&](const std::string& text) {
						D2D1_COLOR_F color = default_color;

						try {
							auto color_ = tree.get<std::string>("text.<xmlattr>.color");

							std::stringstream ss;
							ss << color_;

							unsigned long hex = 0;
							ss >> std::hex >> hex;

							color = convert_color(lecui::color{
									boost::lexical_cast<unsigned short>((hex >> 24) & 0xff),
									boost::lexical_cast<unsigned short>((hex >> 16) & 0xff),
									boost::lexical_cast<unsigned short>((hex >> 8) & 0xff),
									boost::lexical_cast<unsigned short>((hex >> 0) & 0xff)
								});
						}
						catch (const std::exception&) {}

						return color;
					};

					// capture color
					properties.color = get_color(text);

					// add to formatting vector
					formatting_.push_back(properties);
				} while (true);

				// capture the text
				plain_text_ = text;
			}
			catch (const std::exception& e) { log(e.what()); }
		}

		void widgets_impl::apply_formatting(
			const std::vector<text_range_properties>& formatting_,
			ID2D1HwndRenderTarget* p_render_target,
			IDWriteTextLayout* p_text_layout_,
			bool is_enabled,
			ID2D1SolidColorBrush* p_brush_disabled) {
			// apply formatting
			for (auto& properties : formatting_) {
				// apply font
				if (!properties.font.empty())
					p_text_layout_->SetFontFamilyName(convert_string(properties.font).c_str(),
						properties.text_range);

				// apply font size
				if (properties.size)
					p_text_layout_->SetFontSize(convert_fontsize_to_dip(properties.size),
						properties.text_range);

				// apply bold, italic, underline
				if (properties.bold)
					p_text_layout_->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, properties.text_range);
				if (properties.italic)
					p_text_layout_->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, properties.text_range);
				if (properties.underline)
					p_text_layout_->SetUnderline(TRUE, properties.text_range);

				// apply color
				if (p_render_target) {
					ID2D1SolidColorBrush* p_brush;
					HRESULT hr = p_render_target->CreateSolidColorBrush(properties.color, &p_brush);
					if (SUCCEEDED(hr))
						p_text_layout_->SetDrawingEffect(is_enabled ?
							p_brush : p_brush_disabled, properties.text_range);
					safe_release(&p_brush);
				}
			}
		}

		D2D1_RECT_F
			widgets_impl::measure_label(IDWriteFactory* p_directwrite_factory,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				const D2D1_RECT_F max_rect) {
			// the default color doesn't matter here we're just measuring the text
			std::string plain_text_;
			std::vector<text_range_properties> formatting_;
			parse_formatted_text(formatted_text, plain_text_, D2D1::ColorF(D2D1::ColorF::Black), formatting_);

			D2D1_RECT_F rect = max_rect;

			HRESULT hr = S_OK;

			// Create a DirectWrite text format object.
			IDWriteTextFormat* p_text_format_ = nullptr;
			if (SUCCEEDED(hr)) {
				hr = p_directwrite_factory->CreateTextFormat(convert_string(font).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(font_size), L"", //locale
					&p_text_format_);
			}

			IDWriteTextLayout* p_text_layout_ = nullptr;
			if (SUCCEEDED(hr)) {
				p_text_format_->SetTextAlignment(center_h ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				p_text_format_->SetParagraphAlignment(center_v ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				// create a text layout
				hr = p_directwrite_factory->CreateTextLayout(convert_string(plain_text_).c_str(),
					(UINT32)plain_text_.length(), p_text_format_, rect.right - rect.left,
					rect.bottom - rect.top, &p_text_layout_);
			}

			if (SUCCEEDED(hr)) {
				apply_formatting(formatting_, nullptr, p_text_layout_, true, nullptr);

				DWRITE_TEXT_METRICS textMetrics;
				p_text_layout_->GetMetrics(&textMetrics);
				rect.left += textMetrics.left;
				rect.top += textMetrics.top;
				rect.right = smallest(rect.left + textMetrics.width, rect.right);
				rect.bottom = smallest(rect.top + textMetrics.height, rect.bottom);
			}

			// release the text layout
			safe_release(&p_text_layout_);
			safe_release(&p_text_format_);
			return rect;
		}

		D2D1_RECT_F
			widgets_impl::measure_text(IDWriteFactory* p_directwrite_factory,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				bool allow_h_overflow,
				bool allow_v_overflow,
				const D2D1_RECT_F max_rect) {
			// the default color doesn't matter here we're just measuring the text
			std::string plain_text_;
			std::vector<text_range_properties> formatting_;
			parse_formatted_text(formatted_text, plain_text_, D2D1::ColorF(D2D1::ColorF::Black), formatting_);

			D2D1_RECT_F rect = max_rect;

			HRESULT hr = S_OK;

			// Create a DirectWrite text format object.
			IDWriteTextFormat* p_text_format_ = nullptr;
			if (SUCCEEDED(hr)) {
				hr = p_directwrite_factory->CreateTextFormat(convert_string(font).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(font_size), L"", //locale
					&p_text_format_);
			}

			if (SUCCEEDED(hr) && allow_h_overflow) {
				// make text overflow layout rectangle
				p_text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
			}

			IDWriteTextLayout* p_text_layout_ = nullptr;
			if (SUCCEEDED(hr)) {
				p_text_format_->SetTextAlignment(center_h ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				p_text_format_->SetParagraphAlignment(center_v ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				// create a text layout
				hr = p_directwrite_factory->CreateTextLayout(convert_string(plain_text_).c_str(),
					(UINT32)plain_text_.length(), p_text_format_, rect.right - rect.left,
					rect.bottom - rect.top, &p_text_layout_);
			}

			if (SUCCEEDED(hr)) {
				apply_formatting(formatting_, nullptr, p_text_layout_, true, nullptr);

				DWRITE_TEXT_METRICS textMetrics;
				p_text_layout_->GetMetrics(&textMetrics);
				rect.left += textMetrics.left;
				rect.top += textMetrics.top;

				if (allow_h_overflow)
					rect.right = rect.left + textMetrics.width;
				else
					rect.right = smallest(rect.left + textMetrics.width, rect.right);

				rect.bottom = smallest(rect.top + textMetrics.height, rect.bottom);
			}

			// release the text layout
			safe_release(&p_text_layout_);
			safe_release(&p_text_format_);
			return rect;
		}

		widgets_impl::label::label(const std::string& page_alias,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_hot_pressed_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::label::~label() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::label::type() {
			return lecui::widgets_impl::widget_type::label;
		}

		HRESULT widgets_impl::label::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			parse_formatted_text(specs_.text, text_, convert_color(specs_.color_text), formatting_);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot_pressed),
					&p_brush_hot_pressed_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory_->CreateTextFormat(convert_string(specs_.font).c_str(),
					NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(specs_.font_size), L"", //locale
					&p_text_format_);
			}
			if (SUCCEEDED(hr)) {
				p_text_format_->SetTextAlignment(specs_.center_h ?
					DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
				p_text_format_->SetParagraphAlignment(specs_.center_v ?
					DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

				if (!specs_.multiline)
					make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::label::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_hot_pressed_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::label::render(ID2D1HwndRenderTarget* p_render_target,
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

			// create a text layout
			HRESULT hr = p_directwrite_factory_->CreateTextLayout(convert_string(text_).c_str(),
				(UINT32)text_.length(), p_text_format_, rect_.right - rect_.left,
				rect_.bottom - rect_.top, &p_text_layout_);

			DWRITE_TEXT_METRICS textMetrics;

			if (SUCCEEDED(hr)) {
				apply_formatting(formatting_, p_render_target, p_text_layout_, is_enabled_,
					p_brush_disabled_);

				p_text_layout_->GetMetrics(&textMetrics);
				const auto rect_text = rect_;

				rect_.left += textMetrics.left;
				rect_.top += textMetrics.top;
				rect_.right = smallest(rect_.left + textMetrics.width, rect_.right);
				rect_.bottom = smallest(rect_.top + textMetrics.height, rect_.bottom);

				if (render && visible_) {
					if (!is_static_ && is_enabled_) {
						if (hit_ || pressed_)
							p_render_target->FillRectangle(&rect_, pressed_ ?
								p_brush_hot_pressed_ : p_brush_hot_);

						if (selected_)
							p_render_target->FillRectangle(&rect_, p_brush_selected_);
					}

					// draw the text layout
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text.left, rect_text.top },
						p_text_layout_, is_enabled_ ?
						p_brush_ : p_brush_disabled_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}
			}

			// release the text layout
			safe_release(&p_text_layout_);

			return rect_;
		}

		void widgets_impl::label::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::label::label_specs&
			widgets_impl::label::specs() {
			return specs_;
		}

		widgets::label::label_specs&
			widgets_impl::label::operator()() {
			return specs();
		}
	}
}
