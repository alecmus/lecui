/*
** label_impl.cpp - label widget implementation
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

/// 
/// Start and end of XML formatting is marked by <text></text>
/// 
/// The supported attributes of the text are:
/// 
/// font		The font name
/// size		The font size, in points
/// bold		'true' or 'false'
/// italic		'true' or 'false'
/// underline	'true' or 'false'
/// color		0xAABBCCDD where r = AA, g = BB, b=CC, a=DD
/// 
/// e.g. "Last word is <text size='16.5' italic='true' color='0x0000FFFF'>formatted</text>.";
/// 

#include "label_impl.h"
#include "../../containers/page/page_impl.h"
#include "../../form_common.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

void liblec::lecui::widgets_implementation::parse_formatted_text(
	const std::string& formatted_text,
	std::string& plain_text_,
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
				catch (const std::exception& e) { log(e.what()); }

				return std::string();
			};

			// capture font name, size
			properties.font = get_attribute("text.<xmlattr>.font");

			try {
				properties.size = boost::lexical_cast<float>(get_attribute("text.<xmlattr>.size"));
			}
			catch (const std::exception& e) { log(e.what()); }

			// capture bold, italic, underline properties
			properties.bold = get_attribute("text.<xmlattr>.bold") == "true";
			properties.italic = get_attribute("text.<xmlattr>.italic") == "true";
			properties.underline = get_attribute("text.<xmlattr>.underline") == "true";

			auto get_color = [&](const std::string& text) {
				D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f };

				try {
					auto color_ = tree.get<std::string>("text.<xmlattr>.color");

					std::stringstream ss;
					ss << color_;

					unsigned long hex = 0;
					ss >> std::hex >> hex;

					color = convert_color(liblec::lecui::color{
							boost::lexical_cast<unsigned short>((hex >> 24) & 0xff),
							boost::lexical_cast<unsigned short>((hex >> 16) & 0xff),
							boost::lexical_cast<unsigned short>((hex >> 8) & 0xff),
							boost::lexical_cast<unsigned short>((hex >> 0) & 0xff)
						});
				}
				catch (const std::exception& e) { log(e.what()); }

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

void liblec::lecui::widgets_implementation::apply_formatting(
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
liblec::lecui::widgets_implementation::measure_label(IDWriteFactory* p_directwrite_factory,
	const std::string& formatted_text,
	const std::string& font,
	const float font_size,
	bool center_h,
	bool center_v,
	const D2D1_RECT_F max_rect) {
	std::string plain_text_;
	std::vector<text_range_properties> formatting_;
	parse_formatted_text(formatted_text, plain_text_, formatting_);

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

class liblec::lecui::widgets::label::label::label_impl {
public:
	label_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::label::label(liblec::lecui::containers::page& page) :
	d_(*(new label_impl(page))) {}

liblec::lecui::widgets::label::~label() { delete& d_; }

liblec::lecui::widgets::specs::label&
liblec::lecui::widgets::label::add(const std::string& name) {
	return d_.page_.d_page_.add_label(name);
}

liblec::lecui::widgets_implementation::label::label(const std::string& page,
	const std::string& name,
	IDWriteFactory* p_directwrite_factory) :
	p_brush_(nullptr),
	p_brush_hot_(nullptr),
	p_brush_hot_pressed_(nullptr),
	p_brush_disabled_(nullptr),
	p_brush_selected_(nullptr),
	p_text_format_(nullptr),
	p_directwrite_factory_(p_directwrite_factory),
	p_text_layout_(nullptr) {
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::label::~label() {
	discard_resources();
	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::label::name() { return name_; }
std::string liblec::lecui::widgets_implementation::label::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::label::type() {
	return lecui::widgets_implementation::widget_type::label;
}

HRESULT liblec::lecui::widgets_implementation::label::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = (specs_.on_click == nullptr);

	parse_formatted_text(specs_.text, text_, formatting_);

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

void liblec::lecui::widgets_implementation::label::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	resources_created_ = false;
	safe_release(&p_brush_);
	safe_release(&p_brush_hot_);
	safe_release(&p_brush_hot_pressed_);
	safe_release(&p_brush_disabled_);
	safe_release(&p_brush_selected_);
	safe_release(&p_text_format_);
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::label::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	if (!resources_created_)
		create_resources(p_render_target);

	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

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

void liblec::lecui::widgets_implementation::label::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::label&
liblec::lecui::widgets_implementation::label::specs() {
	return specs_;
}
