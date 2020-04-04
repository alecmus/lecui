/*
** image.cpp - image implementation
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

#include "../image.h"
#include "../../form_impl.h"

bool liblec::lecui::widgets::specs::image::operator==(const image& param) {
	return
		// generic specs
		widget::operator==(param) &&

		// widget specific specs
		(color_border == param.color_border) &&
		(file == param.file) &&
		(png_resource == param.png_resource);
}

bool liblec::lecui::widgets::specs::image::operator!=(const image& param) {
	return !operator==(param);
}

class liblec::lecui::widgets::image::image::image_impl {
public:
	image_impl(liblec::lecui::containers::page& page) :
		page_(page) {}
	liblec::lecui::containers::page& page_;
};

liblec::lecui::widgets::image::image(liblec::lecui::containers::page& page) :
	d_(*(new image_impl(page))) {}

liblec::lecui::widgets::image::~image() { delete& d_; }

liblec::lecui::widgets::specs::image&
liblec::lecui::widgets::image::add(const std::string& name) {
	return d_.page_.d_page_.add_image(name);
}

liblec::lecui::widgets::specs::image&
liblec::lecui::widgets::image::specs(form& fm, const std::string& name) {
	// parse widget path
	std::vector<std::string> path;
	std::string widget_name;
	fm.d_.parse_widget_path(name, path, widget_name);

	// find the page
	auto& page = fm.d_.find_page(fm.d_.p_pages_, path);

	// find the widget
	return page.d_page_.get_image(widget_name).specs();
}
