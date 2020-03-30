/*
** custom_impl.cpp - custom widget implementation
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

#include "custom_impl.h"
#include "../../containers/page/page_impl.h"
#include "../../form_common.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>

class liblec::lecui::widgets::custom::custom::custom_impl {
public:
	custom_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::custom::custom(liblec::lecui::containers::page& page) :
	d_(*(new custom_impl(page))) {}

liblec::lecui::widgets::custom::~custom() { delete& d_; }

liblec::lecui::widgets::specs::custom& liblec::lecui::widgets::custom::add(const std::string& name) {
	return d_.page_.d_page_.add_custom(name);
}

liblec::lecui::widgets_implementation::custom::custom(const std::string& page,
	const std::string& name,
	IDWriteFactory* p_directwrite_factory, IWICImagingFactory* p_iwic_factory) :
	p_directwrite_factory_(p_directwrite_factory),
	p_iwic_factory_(p_iwic_factory){
	page_ = page;
	name_ = name;
	log("constructor: " + page_ + ":" + name_);
}

liblec::lecui::widgets_implementation::custom::~custom() {
	// DO NOT call discard_resources() here. Let the client do that
	// in their own destructor (which will be called far before this one).
	// Trying to discard resources here will result in an access violation
	// if the custom widget is not a static object, and we certainly do
	// not want any widget to be a static object!

	log("destructor: " + page_ + ":" + name_);
}

std::string liblec::lecui::widgets_implementation::custom::name() { return name_; }
std::string liblec::lecui::widgets_implementation::custom::page() { return page_; }

liblec::lecui::widgets_implementation::widget_type
liblec::lecui::widgets_implementation::custom::type() {
	return lecui::widgets_implementation::widget_type::custom;
}

HRESULT liblec::lecui::widgets_implementation::custom::create_resources(
	ID2D1HwndRenderTarget* p_render_target) {
	log("creating resources:   " + page_ + ":" + name_);
	is_static_ = (specs_.on_click == nullptr);

	if (specs_.on_create_resources != nullptr)
		specs_.on_create_resources(p_render_target, p_directwrite_factory_, p_iwic_factory_);

	return S_OK;
}

void liblec::lecui::widgets_implementation::custom::discard_resources() {
	log("discarding resources: " + page_ + ":" + name_);
	if (specs_.on_discard_resources != nullptr)
		specs_.on_discard_resources();
}

D2D1_RECT_F&
liblec::lecui::widgets_implementation::custom::render(ID2D1HwndRenderTarget* p_render_target,
	const float& change_in_width, const float& change_in_height, float x_off_set, float y_off_set,
	const bool& render) {
	rect_ = position(specs_.rect, specs_.resize, change_in_width, change_in_height);
	rect_.left -= x_off_set;
	rect_.right -= x_off_set;
	rect_.top -= y_off_set;
	rect_.bottom -= y_off_set;

	if (render && (specs_.on_render != nullptr))
		specs_.on_render(&rect_, is_enabled_, hit_, pressed_, selected_);

	return rect_;
}

void liblec::lecui::widgets_implementation::custom::on_click() {
	if (specs_.on_click)
		specs_.on_click();
}

liblec::lecui::widgets::specs::custom&
liblec::lecui::widgets_implementation::custom::specs() {
	return specs_;
}
