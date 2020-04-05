/*
** page_impl.h - page_impl interface
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

#pragma once

#include "../page.h"
#include "../../widgets/h_scrollbar/h_scrollbar.h"
#include "../../widgets/v_scrollbar/v_scrollbar.h"

#include "../tab_control/tab_control_impl.h"
#include "../pane/pane_impl.h"

#include "../../widgets/rectangle/rectangle_impl.h"
#include "../../widgets/label/label_impl.h"
#include "../../widgets/group/group_impl.h"
#include "../../widgets/button/button_impl.h"
#include "../../widgets/toggle/toggle_impl.h"
#include "../../widgets/combo/combo_impl.h"
#include "../../widgets/list/list_impl.h"
#include "../../widgets/custom/custom_impl.h"
#include "../../widgets/image/image_impl.h"
#include "../../widgets/progress_indicator/progress_indicator_impl.h"
#include "../../widgets/progress_bar/progress_bar_impl.h"

class liblec::lecui::containers::page::page_impl {
public:
	page_impl(const std::string& name);
	~page_impl();

	std::string name();
	void direct2d_factory(ID2D1Factory* p_direct2d_factory);
	ID2D1Factory* direct2d_factory();
	void directwrite_factory(IDWriteFactory* p_directwrite_factory);
	IDWriteFactory* directwrite_factory();
	void iwic_factory(IWICImagingFactory* p_iwic_factory);
	IWICImagingFactory* iwic_factory();

	void size(const liblec::lecui::size& size);
	const liblec::lecui::size& size();
	void width(const float& width);
	const float width();
	void height(const float& height);
	const float height();
	bool contains(const D2D1_POINT_2F& point);

	liblec::lecui::containers::specs::tab_control&
		add_tab_control(const std::string& name);
	liblec::lecui::containers::specs::pane&
		add_pane(const std::string& name);

	liblec::lecui::widgets::specs::rectangle&
		add_rectangle(const std::string& name);
	liblec::lecui::widgets::specs::label&
		add_label(const std::string& name);
	liblec::lecui::widgets::specs::group&
		add_group(const std::string& name);
	liblec::lecui::widgets::specs::button&
		add_button(const std::string& name);
	liblec::lecui::widgets::specs::toggle&
		add_toggle(const std::string& name);
	liblec::lecui::widgets::specs::combo&
		add_combo(const std::string& name);
	liblec::lecui::widgets::specs::list&
		add_list(const std::string& name);
	liblec::lecui::widgets::specs::custom&
		add_custom(const std::string& name);
	liblec::lecui::widgets::specs::image&
		add_image(const std::string& name);
	liblec::lecui::widgets::specs::progress_indicator&
		add_progress_indicator(const std::string& name);
	liblec::lecui::widgets::specs::progress_bar&
		add_progress_bar(const std::string& name);

	std::map<std::string,
		liblec::lecui::widgets_implementation::widget&>&
		widgets();
	const std::vector<std::string>&
		widgets_order();
	liblec::lecui::widgets_implementation::h_scrollbar&
		h_scrollbar();
	liblec::lecui::widgets_implementation::v_scrollbar&
		v_scrollbar();

	liblec::lecui::widgets_implementation::tab_control&
		get_tab_control(const std::string& name);
	liblec::lecui::widgets_implementation::pane&
		get_pane(const std::string& name);

	liblec::lecui::widgets_implementation::rectangle&
		get_rectangle(const std::string& name);
	liblec::lecui::widgets_implementation::button&
		get_button(const std::string& name);
	liblec::lecui::widgets_implementation::label&
		get_label(const std::string& name);
	liblec::lecui::widgets_implementation::group&
		get_group(const std::string& name);
	liblec::lecui::widgets_implementation::toggle&
		get_toggle(const std::string& name);
	liblec::lecui::widgets_implementation::combo&
		get_combo(const std::string& name);
	liblec::lecui::widgets_implementation::list&
		get_list(const std::string& name);
	liblec::lecui::widgets_implementation::custom&
		get_custom(const std::string& name);
	liblec::lecui::widgets_implementation::image&
		get_image(const std::string& name);
	liblec::lecui::widgets_implementation::progress_indicator&
		get_progress_indicator(const std::string& name);
	liblec::lecui::widgets_implementation::progress_bar&
		get_progress_bar(const std::string& name);

private:
	ID2D1Factory* p_direct2d_factory_;
	IDWriteFactory* p_directwrite_factory_;
	IWICImagingFactory* p_iwic_factory_;
	std::string name_;
	liblec::lecui::size size_;

	std::map<std::string, liblec::lecui::widgets_implementation::widget&> widgets_;
	std::vector<std::string> widgets_order_;

	liblec::lecui::widgets_implementation::h_scrollbar h_scrollbar_;
	liblec::lecui::widgets_implementation::v_scrollbar v_scrollbar_;

	std::map<std::string, liblec::lecui::widgets_implementation::tab_control> tab_controls_;
	std::map<std::string, liblec::lecui::widgets_implementation::pane> panes_;

	std::map<std::string, liblec::lecui::widgets_implementation::rectangle> rectangles_;
	std::map<std::string, liblec::lecui::widgets_implementation::label> labels_;
	std::map<std::string, liblec::lecui::widgets_implementation::group> groups_;
	std::map<std::string, liblec::lecui::widgets_implementation::button> buttons_;
	std::map<std::string, liblec::lecui::widgets_implementation::toggle> toggles_;
	std::map<std::string, liblec::lecui::widgets_implementation::combo> combos_;
	std::map<std::string, liblec::lecui::widgets_implementation::list> lists_;
	std::map<std::string, liblec::lecui::widgets_implementation::custom> customs_;
	std::map<std::string, liblec::lecui::widgets_implementation::image> images_;
	std::map<std::string, liblec::lecui::widgets_implementation::progress_indicator> progress_indicators_;
	std::map<std::string, liblec::lecui::widgets_implementation::progress_bar> progress_bars_;

	friend liblec::lecui::form;
};
