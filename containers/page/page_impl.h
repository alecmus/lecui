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
#include "../../widgets/h_scrollbar/h_scrollbar.h"
#include "../../widgets/v_scrollbar/v_scrollbar.h"

#include "../tab_pane/tab_pane_impl.h"
#include "../pane/pane_impl.h"
#include "../group/group_impl.h"

#include "../../widgets/rectangle/rectangle_impl.h"
#include "../../widgets/label/label_impl.h"
#include "../../widgets/button/button_impl.h"
#include "../../widgets/toggle/toggle_impl.h"
#include "../../widgets/combo/combo_impl.h"
#include "../../widgets/table/table_impl.h"
#include "../../widgets/custom/custom_impl.h"
#include "../../widgets/image/image_impl.h"
#include "../../widgets/progress_indicator/progress_indicator_impl.h"
#include "../../widgets/progress_bar/progress_bar_impl.h"
#include "../../widgets/checkbox/checkbox_impl.h"
#include "../../widgets/textbox/textbox_impl.h"
#include "../../widgets/tree/tree_impl.h"

namespace liblec {
	namespace lecui {
		class containers::page::impl {
		public:
			impl(form& fm, const std::string& alias);
			~impl();

			const std::string& alias();
			void direct2d_factory(ID2D1Factory* p_direct2d_factory);
			ID2D1Factory* direct2d_factory();
			void directwrite_factory(IDWriteFactory* p_directwrite_factory);
			IDWriteFactory* directwrite_factory();
			void iwic_factory(IWICImagingFactory* p_iwic_factory);
			IWICImagingFactory* iwic_factory();

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
			widgets::combo::combo_specs&
				add_combo(std::string alias);
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

			std::map<std::string,
				widgets_impl::widget&>&
				widgets();
			const std::vector<std::string>&
				widgets_order();
			widgets_impl::h_scrollbar&
				h_scrollbar();
			widgets_impl::v_scrollbar&
				v_scrollbar();

			widgets_impl::tab_pane&
				get_tab_pane(const std::string& alias);
			widgets_impl::pane&
				get_pane(const std::string& alias);

			widgets_impl::rectangle&
				get_rectangle(const std::string& alias);
			widgets_impl::button&
				get_button(const std::string& alias);
			widgets_impl::label&
				get_label(const std::string& alias);
			widgets_impl::group&
				get_group(const std::string& alias);
			widgets_impl::toggle&
				get_toggle(const std::string& alias);
			widgets_impl::combo&
				get_combo(const std::string& alias);
			widgets_impl::table&
				get_table(const std::string& alias);
			widgets_impl::custom&
				get_custom(const std::string& alias);
			widgets_impl::image&
				get_image(const std::string& alias);
			widgets_impl::progress_indicator&
				get_progress_indicator(const std::string& alias);
			widgets_impl::progress_bar&
				get_progress_bar(const std::string& alias);
			widgets_impl::checkbox&
				get_checkbox(const std::string& alias);
			widgets_impl::textbox&
				get_textbox(const std::string& alias);
			widgets_impl::tree&
				get_tree(const std::string& alias);

			bool close_widget(const std::string& alias,
				widgets_impl::widget_type type, std::string& error);

		private:
			void check_alias(std::string& alias);

			ID2D1Factory* p_direct2d_factory_;
			IDWriteFactory* p_directwrite_factory_;
			IWICImagingFactory* p_iwic_factory_;
			std::string alias_;
			lecui::size size_;

			std::map<std::string, widgets_impl::widget&> widgets_;
			std::vector<std::string> widgets_order_;

			widgets_impl::h_scrollbar h_scrollbar_;
			widgets_impl::v_scrollbar v_scrollbar_;

			std::map<std::string, widgets_impl::tab_pane> tab_panes_;
			std::map<std::string, widgets_impl::pane> panes_;

			std::map<std::string, widgets_impl::rectangle> rectangles_;
			std::map<std::string, widgets_impl::label> labels_;
			std::map<std::string, widgets_impl::group> groups_;
			std::map<std::string, widgets_impl::button> buttons_;
			std::map<std::string, widgets_impl::toggle> toggles_;
			std::map<std::string, widgets_impl::combo> combos_;
			std::map<std::string, widgets_impl::table> tables_;
			std::map<std::string, widgets_impl::custom> customs_;
			std::map<std::string, widgets_impl::image> images_;
			std::map<std::string, widgets_impl::progress_indicator> progress_indicators_;
			std::map<std::string, widgets_impl::progress_bar> progress_bars_;
			std::map<std::string, widgets_impl::checkbox> checkboxes_;
			std::map<std::string, widgets_impl::textbox> textboxes_;
			std::map<std::string, widgets_impl::tree> trees_;

			form& fm_;

			friend class form;
			friend class containers::tab_pane;
			friend class containers::tab;
			friend class containers::pane;
			friend class containers::group;
			friend class widgets::rectangle;
			friend class widgets::label;
			friend class widgets::button;
			friend class widgets::toggle;
			friend class widgets::combo;
			friend class widgets::table;
			friend class widgets::custom;
			friend class widgets::image;
			friend class widgets::progress_indicator;
			friend class widgets::progress_bar;
			friend class widgets::checkbox;
			friend class widgets::textbox;
			friend class widgets::tree;
		};
	}
}
