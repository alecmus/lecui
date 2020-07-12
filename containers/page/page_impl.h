/*
** page_impl.h - page::impl interface
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
#include "../../widgets/h_scrollbar/h_scrollbar_impl.h"
#include "../../widgets/v_scrollbar/v_scrollbar_impl.h"

#include "../tab_pane/tab_pane_impl.h"
#include "../pane/pane_impl.h"
#include "../group/group_impl.h"

#include "../../widgets/rectangle/rectangle_impl.h"
#include "../../widgets/label/label_impl.h"
#include "../../widgets/button/button_impl.h"
#include "../../widgets/toggle/toggle_impl.h"
#include "../../widgets/table/table_impl.h"
#include "../../widgets/custom/custom_impl.h"
#include "../../widgets/image/image_impl.h"
#include "../../widgets/progress_indicator/progress_indicator_impl.h"
#include "../../widgets/progress_bar/progress_bar_impl.h"
#include "../../widgets/checkbox/checkbox_impl.h"
#include "../../widgets/textbox/textbox_impl.h"
#include "../../widgets/tree/tree_impl.h"
#include "../../widgets/slider/slider_impl.h"
#include "../../widgets/html_editor/html_editor_impl.h"
#include "../../widgets/combobox/combobox_impl.h"
#include "../../widgets/line/line_impl.h"
#include "../../widgets/time/time_impl.h"
#include "../../widgets/date/date_impl.h"
#include "../../widgets/icon/icon_impl.h"

namespace liblec {
	namespace lecui {
		class containers::page::impl {
		public:
			impl(form& fm, containers::page& pg, const std::string& alias);
			~impl();

			const std::string& alias();
			void direct2d_factory(ID2D1Factory* p_direct2d_factory);
			ID2D1Factory* direct2d_factory();
			void directwrite_factory(IDWriteFactory* p_directwrite_factory);
			IDWriteFactory* directwrite_factory();
			void iwic_factory(IWICImagingFactory* p_iwic_factory);
			IWICImagingFactory* iwic_factory();

			float get_dpi_scale();
			form& get_form();

			void size(const lecui::size& size);
			const lecui::size& size();
			void width(const float& width);
			const float width();
			void height(const float& height);
			const float height();
			bool contains(const D2D1_POINT_2F& point);

			containers::tab_pane::tab_pane_specs&
				add_tab_pane(std::string alias);
			containers::pane::pane_specs&
				add_pane(std::string alias);

			widgets::rectangle::rectangle_specs&
				add_rectangle(std::string alias);
			widgets::label::label_specs&
				add_label(std::string alias);
			containers::group::group_specs&
				add_group(std::string alias);
			widgets::button::button_specs&
				add_button(std::string alias);
			widgets::toggle::toggle_specs&
				add_toggle(std::string alias);
			widgets::table::table_specs&
				add_table(std::string alias);
			widgets::custom::custom_specs&
				add_custom(std::string alias);
			widgets::image::image_specs&
				add_image(std::string alias);
			widgets::progress_indicator::progress_indicator_specs&
				add_progress_indicator(std::string alias);
			widgets::progress_bar::progress_bar_specs&
				add_progress_bar(std::string alias);
			widgets::checkbox::checkbox_specs&
				add_checkbox(std::string alias);
			widgets::textbox::textbox_specs&
				add_textbox(std::string alias);
			widgets::tree::tree_specs&
				add_tree(std::string alias);
			widgets::slider::slider_specs&
				add_slider(std::string alias);
			widgets::html_editor::html_editor_specs&
				add_html_editor(std::string alias);
			widgets::combobox::combobox_specs&
				add_combobox(std::string alias);
			widgets::line::line_specs&
				add_line(std::string alias);
			widgets::time::time_specs&
				add_time(std::string alias);
			widgets::date::date_specs&
				add_date(std::string alias);
			widgets::icon::icon_specs&
				add_icon(std::string alias);

			std::map<std::string,
				widgets::widget_impl&>&
				widgets();
			const std::vector<std::string>&
				widgets_order();
			widgets::h_scrollbar_impl&
				h_scrollbar();
			widgets::v_scrollbar_impl&
				v_scrollbar();

			widgets::tab_pane_impl&
				get_tab_pane(const std::string& alias);
			widgets::pane_impl&
				get_pane(const std::string& alias);

			widgets::rectangle_impl&
				get_rectangle(const std::string& alias);
			widgets::button_impl&
				get_button(const std::string& alias);
			widgets::label_impl&
				get_label(const std::string& alias);
			widgets::group_impl&
				get_group(const std::string& alias);
			widgets::toggle_impl&
				get_toggle(const std::string& alias);
			widgets::table_impl&
				get_table(const std::string& alias);
			widgets::custom_impl&
				get_custom(const std::string& alias);
			widgets::image_impl&
				get_image(const std::string& alias);
			widgets::progress_indicator_impl&
				get_progress_indicator(const std::string& alias);
			widgets::progress_bar_impl&
				get_progress_bar(const std::string& alias);
			widgets::checkbox_impl&
				get_checkbox(const std::string& alias);
			widgets::textbox_impl&
				get_textbox(const std::string& alias);
			widgets::tree_impl&
				get_tree(const std::string& alias);
			widgets::slider_impl&
				get_slider(const std::string& alias);
			widgets::html_editor_impl&
				get_html_editor(const std::string& alias);
			widgets::combobox_impl&
				get_combobox(const std::string& alias);
			widgets::line_impl&
				get_line(const std::string& alias);
			widgets::time_impl&
				get_time(const std::string& alias);
			widgets::date_impl&
				get_date(const std::string& alias);
			widgets::icon_impl&
				get_icon(const std::string& alias);

			bool close_widget(const std::string& alias,
				widgets::widget_type type, std::string& error);

			D2D1_RECT_F get_rect();

			bool hit();
			bool on_mousewheel(float units);
			void scroll(float pixels);
			void force_scrollbar_set();

		private:
			void check_alias(std::string& alias);

			ID2D1Factory* p_direct2d_factory_;
			IDWriteFactory* p_directwrite_factory_;
			IWICImagingFactory* p_iwic_factory_;
			std::string alias_;
			lecui::size size_;
			bool hit_;
			bool scrollbar_set_;

			std::map<std::string, widgets::widget_impl&> widgets_;
			std::vector<std::string> widgets_order_;

			widgets::h_scrollbar_impl h_scrollbar_;
			widgets::v_scrollbar_impl v_scrollbar_;

			std::map<std::string, widgets::tab_pane_impl> tab_panes_;
			std::map<std::string, widgets::pane_impl> panes_;

			std::map<std::string, widgets::rectangle_impl> rectangles_;
			std::map<std::string, widgets::label_impl> labels_;
			std::map<std::string, widgets::group_impl> groups_;
			std::map<std::string, widgets::button_impl> buttons_;
			std::map<std::string, widgets::toggle_impl> toggles_;
			std::map<std::string, widgets::table_impl> tables_;
			std::map<std::string, widgets::custom_impl> customs_;
			std::map<std::string, widgets::image_impl> images_;
			std::map<std::string, widgets::progress_indicator_impl> progress_indicators_;
			std::map<std::string, widgets::progress_bar_impl> progress_bars_;
			std::map<std::string, widgets::checkbox_impl> checkboxes_;
			std::map<std::string, widgets::textbox_impl> textboxes_;
			std::map<std::string, widgets::tree_impl> trees_;
			std::map<std::string, widgets::slider_impl> sliders_;
			std::map<std::string, widgets::html_editor_impl> html_editors_;
			std::map<std::string, widgets::combobox_impl> comboboxes_;
			std::map<std::string, widgets::line_impl> lines_;
			std::map<std::string, widgets::time_impl> times_;
			std::map<std::string, widgets::date_impl> dates_;
			std::map<std::string, widgets::icon_impl> icons_;

			form& fm_;
			containers::page& pg_;

			friend class form;
			friend class containers::tab_pane;
			friend class containers::tab;
			friend class containers::pane;
			friend class containers::group;
			friend class widgets::rectangle;
			friend class widgets::label;
			friend class widgets::button;
			friend class widgets::toggle;
			friend class widgets::table;
			friend class widgets::custom;
			friend class widgets::image;
			friend class widgets::progress_indicator;
			friend class widgets::progress_bar;
			friend class widgets::checkbox;
			friend class widgets::textbox;
			friend class widgets::tree;
			friend class widgets::slider;
			friend class widgets::html_editor;
			friend class widgets::combobox;
			friend class widgets::line;
			friend class widgets::time;
			friend class widgets::date;
			friend class widgets::icon;

			friend class widgets::html_editor_impl;
		};
	}
}
