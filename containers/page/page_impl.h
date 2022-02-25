//
// page_impl.h - page::impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include <optional>

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
#include "../../widgets/table_view/table_view_impl.h"
#include "../../widgets/custom/custom_impl.h"
#include "../../widgets/image_view/image_view_impl.h"
#include "../../widgets/progress_indicator/progress_indicator_impl.h"
#include "../../widgets/progress_bar/progress_bar_impl.h"
#include "../../widgets/checkbox/checkbox_impl.h"
#include "../../widgets/text_field/text_field_impl.h"
#include "../../widgets/tree_view/tree_view_impl.h"
#include "../../widgets/slider/slider_impl.h"
#include "../../widgets/html_editor/html_editor_impl.h"
#include "../../widgets/combobox/combobox_impl.h"
#include "../../widgets/line/line_impl.h"
#include "../../widgets/time/time_impl.h"
#include "../../widgets/date/date_impl.h"
#include "../../widgets/icon/icon_impl.h"
#include "../../widgets/strength_bar/strength_bar_impl.h"

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
			void parent(containers::page& p);
			std::optional<std::reference_wrapper<containers::page>> parent();

			float get_dpi_scale();
			form& get_form();

			void size(const lecui::size& size);
			const lecui::size& size();
			void width(const float& width);
			const float width();
			void height(const float& height);
			const float height();
			bool contains(const D2D1_POINT_2F& point);

			containers::tab_pane&
				add_tab_pane(std::string alias, const float& content_margin);
			containers::pane&
				add_pane(std::string alias, const float& content_margin);

			widgets::rectangle&
				add_rectangle(std::string alias);
			widgets::label&
				add_label(std::string alias);
			containers::group&
				add_group(std::string alias);
			widgets::button&
				add_button(std::string alias);
			widgets::toggle&
				add_toggle(std::string alias);
			widgets::table_view&
				add_table_view(std::string alias);
			widgets::custom&
				add_custom(std::string alias);
			widgets::image_view&
				add_image_view(std::string alias);
			widgets::progress_indicator&
				add_progress_indicator(std::string alias);
			widgets::progress_bar&
				add_progress_bar(std::string alias);
			widgets::checkbox&
				add_checkbox(std::string alias);
			widgets::text_field&
				add_text_field(std::string alias);
			widgets::tree_view&
				add_tree(std::string alias);
			widgets::slider&
				add_slider(std::string alias);
			widgets::html_editor&
				add_html_editor(std::string alias);
			widgets::combobox&
				add_combobox(std::string alias);
			widgets::line&
				add_line(std::string alias);
			widgets::time&
				add_time(std::string alias);
			widgets::date&
				add_date(std::string alias);
			widgets::icon&
				add_icon(std::string alias);
			widgets::strength_bar&
				add_strength_bar(std::string alias);

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
				get_tab_pane_impl(const std::string& alias);
			widgets::pane_impl&
				get_pane_impl(const std::string& alias);

			widgets::rectangle_impl&
				get_rectangle_impl(const std::string& alias);
			widgets::button_impl&
				get_button_impl(const std::string& alias);
			widgets::label_impl&
				get_label_impl(const std::string& alias);
			widgets::group_impl&
				get_group_impl(const std::string& alias);
			widgets::toggle_impl&
				get_toggle_impl(const std::string& alias);
			widgets::table_view_impl&
				get_table_view_impl(const std::string& alias);
			widgets::custom_impl&
				get_custom_impl(const std::string& alias);
			widgets::image_view_impl&
				get_image_view_impl(const std::string& alias);
			widgets::progress_indicator_impl&
				get_progress_indicator_impl(const std::string& alias);
			widgets::progress_bar_impl&
				get_progress_bar_impl(const std::string& alias);
			widgets::checkbox_impl&
				get_checkbox_impl(const std::string& alias);
			widgets::text_field_impl&
				get_text_field_impl(const std::string& alias);
			widgets::tree_view_impl&
				get_tree_view_impl(const std::string& alias);
			widgets::slider_impl&
				get_slider_impl(const std::string& alias);
			widgets::html_editor_impl&
				get_html_editor_impl(const std::string& alias);
			widgets::combobox_impl&
				get_combobox_impl(const std::string& alias);
			widgets::line_impl&
				get_line_impl(const std::string& alias);
			widgets::time_impl&
				get_time_impl(const std::string& alias);
			widgets::date_impl&
				get_date_impl(const std::string& alias);
			widgets::icon_impl&
				get_icon_impl(const std::string& alias);
			widgets::strength_bar_impl&
				get_strength_bar_impl(const std::string& alias);

			bool close_widget(const std::string& alias,
				widgets::widget_type type, std::string& error);

			D2D1_RECT_F get_rect();

			bool hit();
			bool on_mousewheel(float units);
			void scroll_vertically(float pixels);
			void scroll_horizontally(float pixels);
			void force_scrollbar_set();

			float _content_margin = 0.f;

		private:
			void check_alias(std::string& alias);

			ID2D1Factory* _p_direct2d_factory;
			IDWriteFactory* _p_directwrite_factory;
			IWICImagingFactory* _p_iwic_factory;
			std::string _alias;
			lecui::size _size;
			bool _hit;
			bool _scrollbar_set;

			std::map<std::string, widgets::widget_impl&> _widgets;
			std::vector<std::string> _widgets_order;

			widgets::h_scrollbar_impl _h_scrollbar;
			widgets::v_scrollbar_impl _v_scrollbar;

			std::map<std::string, widgets::tab_pane_impl> _tab_panes;
			std::map<std::string, widgets::pane_impl> _panes;

			std::map<std::string, widgets::rectangle_impl> _rectangles;
			std::map<std::string, widgets::label_impl> _labels;
			std::map<std::string, widgets::group_impl> _groups;
			std::map<std::string, widgets::button_impl> _buttons;
			std::map<std::string, widgets::toggle_impl> _toggles;
			std::map<std::string, widgets::table_view_impl> _table_views;
			std::map<std::string, widgets::custom_impl> _customs;
			std::map<std::string, widgets::image_view_impl> _image_views;
			std::map<std::string, widgets::progress_indicator_impl> _progress_indicators;
			std::map<std::string, widgets::progress_bar_impl> _progress_bars;
			std::map<std::string, widgets::checkbox_impl> _checkboxes;
			std::map<std::string, widgets::text_field_impl> _text_fields;
			std::map<std::string, widgets::tree_view_impl> _trees;
			std::map<std::string, widgets::slider_impl> _sliders;
			std::map<std::string, widgets::html_editor_impl> _html_editors;
			std::map<std::string, widgets::combobox_impl> _comboboxes;
			std::map<std::string, widgets::line_impl> _lines;
			std::map<std::string, widgets::time_impl> _times;
			std::map<std::string, widgets::date_impl> _dates;
			std::map<std::string, widgets::icon_impl> _icons;
			std::map<std::string, widgets::strength_bar_impl> _strength_bars;

			form& _fm;
			containers::page& _pg;
			std::optional<std::reference_wrapper<containers::page>> _parent;

			friend class form;
			friend class containers::tab_pane_builder;
			friend class containers::tab_builder;
			friend class containers::pane_builder;
			friend class containers::group_builder;
			friend class widgets::rectangle_builder;
			friend class widgets::label_builder;
			friend class widgets::button_builder;
			friend class widgets::toggle_builder;
			friend class widgets::table_view_builder;
			friend class widgets::custom_builder;
			friend class widgets::image_view_builder;
			friend class widgets::progress_indicator_builder;
			friend class widgets::progress_bar_builder;
			friend class widgets::checkbox_builder;
			friend class widgets::text_field_builder;
			friend class widgets::tree_view_builder;
			friend class widgets::slider_builder;
			friend class widgets::html_editor_builder;
			friend class widgets::combobox_builder;
			friend class widgets::line_builder;
			friend class widgets::time_builder;
			friend class widgets::date_builder;
			friend class widgets::icon_builder;
			friend class widgets::strength_bar_builder;

			friend class widgets::html_editor_impl;
			friend class widgets::tree_view_impl;
		};
	}
}
