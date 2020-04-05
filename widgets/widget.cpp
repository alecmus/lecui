/*
** widget.cpp - widget implementation
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

#include "widget.h"

bool liblec::lecui::widgets::specs::on_resize::operator==(const on_resize& param) {
	return
		(perc_x == param.perc_x) &&
		(perc_y == param.perc_y) &&
		(perc_width == param.perc_width) &&
		(perc_height == param.perc_height) &&
		(min_x == param.min_x) &&
		(min_y == param.min_y) &&
		(max_x == param.max_x) &&
		(max_y == param.max_y) &&
		(min_width == param.min_width) &&
		(min_height == param.min_height) &&
		(max_width == param.max_width) &&
		(max_height == param.max_height);
}

bool liblec::lecui::widgets::specs::on_resize::operator!=(const on_resize& param) {
	return !operator==(param);
}

bool liblec::lecui::widgets::specs::widget::operator==(const widget& param) {
	return
		// to-do: find out how to check if a std::function has changed so we can
		// also check if the on_click handler has been changed
		(text == param.text) &&
		(font == param.font) &&
		(font_size == param.font_size) &&
		(rect == param.rect) &&
		(resize == param.resize) &&
		(color_text == param.color_text) &&
		(color_fill == param.color_fill) &&
		(color_hot == param.color_hot) &&
		(color_selected == param.color_selected) &&
		(color_disabled == param.color_disabled);
}

bool liblec::lecui::widgets::specs::widget::operator!=(const widget& param) {
	return !operator==(param);
}