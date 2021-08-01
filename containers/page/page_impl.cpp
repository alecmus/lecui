//
// page_impl.cpp - page::impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "page_impl.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::impl::impl(form& fm, containers::page& pg, const std::string& alias) :
			_fm(fm),
			_pg(pg),
			_p_direct2d_factory(nullptr),
			_p_directwrite_factory(nullptr),
			_p_iwic_factory(nullptr),
			_alias(alias),
			_hit(false),
			_scrollbar_set(false),
			_h_scrollbar(pg),
			_v_scrollbar(pg) {
			_widgets.emplace(_h_scrollbar.alias(), _h_scrollbar);
			_widgets.emplace(_v_scrollbar.alias(), _v_scrollbar);
		}

		containers::page::impl::~impl() {}
		const std::string& containers::page::impl::alias() { return _alias; }

		void containers::page::impl::direct2d_factory(ID2D1Factory* p_direct2d_factory) {
			_p_direct2d_factory = p_direct2d_factory;
		}

		ID2D1Factory* containers::page::impl::direct2d_factory() {
			return _p_direct2d_factory;
		}

		void
			containers::page::impl::directwrite_factory(IDWriteFactory* p_directwrite_factory) {
			_p_directwrite_factory = p_directwrite_factory;
		}

		IDWriteFactory* containers::page::impl::directwrite_factory() {
			return _p_directwrite_factory;
		}

		void
			containers::page::impl::iwic_factory(IWICImagingFactory* p_iwic_factory) {
			_p_iwic_factory = p_iwic_factory;
		}

		IWICImagingFactory* containers::page::impl::iwic_factory() {
			return _p_iwic_factory;
		}

		void containers::page::impl::parent(containers::page& p) {
			_parent = p;
		}

		std::optional<std::reference_wrapper<containers::page>>
			containers::page::impl::parent() {
			return _parent;
		}

		float containers::page::impl::get_dpi_scale() {
			return _fm._d.get_dpi_scale();
		}

		form& containers::page::impl::get_form() {
			return _fm;
		}

		void containers::page::impl::size(const lecui::size& size) { _size = size; }
		const size& containers::page::impl::size() { return _size; }
		void containers::page::impl::width(const float& width) { _size.width(width); }
		const float containers::page::impl::width() { return _size.get_width(); }
		void containers::page::impl::height(const float& height) { _size.height(height); }
		const float containers::page::impl::height() { return _size.get_height(); }

		bool containers::page::impl::contains(const D2D1_POINT_2F& point) {
			bool _contains = false;

			try {
				// check if minimal page border rect contains the point
				auto& rect = _rectangles.at(widgets::rectangle_impl::page_rect_alias());
				_contains = rect.contains(point);
			}
			catch (const std::exception&) {}

			_hit = _contains;

			if (!_contains)
				_scrollbar_set = false;

			return _contains;
		}

		containers::tab_pane&
			containers::page::impl::add_tab_pane(std::string alias, const float& content_margin) {
			check_alias(alias);
			if (_tab_panes.try_emplace(alias, _pg, alias, content_margin, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _tab_panes.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_tab_panes.at(alias).specs().alias(alias);
			return _tab_panes.at(alias).specs();
		}

		containers::pane&
			containers::page::impl::add_pane(std::string alias, const float& content_margin) {
			check_alias(alias);
			if (_panes.try_emplace(alias, _pg, alias, content_margin).second) {
				_widgets.emplace(alias, _panes.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_panes.at(alias).specs().alias(alias);
			return _panes.at(alias)._p_panes.at(_panes.at(alias)._current_pane);
		}

		widgets::rectangle&
			containers::page::impl::add_rectangle(std::string alias) {
			check_alias(alias);
			if (_rectangles.try_emplace(alias, _pg, alias, _h_scrollbar, _v_scrollbar).second) {
				_widgets.emplace(alias, _rectangles.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_rectangles.at(alias).specs().alias(alias);
			return _rectangles.at(alias).specs();
		}

		widgets::label&
			containers::page::impl::add_label(std::string alias) {
			check_alias(alias);
			if (_labels.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _labels.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_labels.at(alias).specs().alias(alias);
			return _labels.at(alias).specs();
		}

		containers::group&
			containers::page::impl::add_group(std::string alias) {
			check_alias(alias);
			if (_groups.try_emplace(alias, _pg, alias).second) {
				_widgets.emplace(alias, _groups.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_groups.at(alias).specs().alias(alias);
			return _groups.at(alias).specs();
		}

		widgets::button&
			containers::page::impl::add_button(std::string alias) {
			check_alias(alias);
			if (_buttons.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _buttons.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_buttons.at(alias).specs().alias(alias);
			return _buttons.at(alias).specs();
		}

		widgets::toggle&
			containers::page::impl::add_toggle(std::string alias) {
			check_alias(alias);
			if (_toggles.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _toggles.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_toggles.at(alias).specs().alias(alias);
			return _toggles.at(alias).specs();
		}

		widgets::table_view&
			containers::page::impl::add_table_view(std::string alias) {
			check_alias(alias);
			if (_table_views.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _table_views.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_table_views.at(alias).specs().alias(alias);
			return _table_views.at(alias).specs();
		}

		widgets::custom&
			containers::page::impl::add_custom(std::string alias) {
			check_alias(alias);
			if (_customs.try_emplace(alias, _pg, alias, _p_directwrite_factory, _p_iwic_factory).second) {
				_widgets.emplace(alias, _customs.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_customs.at(alias).specs().alias(alias);
			return _customs.at(alias).specs();
		}

		widgets::image_view&
			containers::page::impl::add_image_view(std::string alias) {
			check_alias(alias);
			if (_image_views.try_emplace(alias, _pg, alias, _p_iwic_factory).second) {
				_widgets.emplace(alias, _image_views.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_image_views.at(alias).specs().alias(alias);
			return _image_views.at(alias).specs();
		}

		widgets::progress_indicator&
			containers::page::impl::add_progress_indicator(std::string alias) {
			check_alias(alias);
			if (_progress_indicators.try_emplace(alias, _pg, alias, _p_direct2d_factory, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _progress_indicators.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_progress_indicators.at(alias).specs().alias(alias);
			return _progress_indicators.at(alias).specs();
		}

		widgets::progress_bar&
			containers::page::impl::add_progress_bar(std::string alias) {
			check_alias(alias);
			if (_progress_bars.try_emplace(alias, _pg, alias, _p_direct2d_factory, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _progress_bars.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_progress_bars.at(alias).specs().alias(alias);
			return _progress_bars.at(alias).specs();
		}

		widgets::checkbox&
			containers::page::impl::add_checkbox(std::string alias) {
			check_alias(alias);
			if (_checkboxes.try_emplace(alias, _pg, alias, _p_direct2d_factory, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _checkboxes.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_checkboxes.at(alias).specs().alias(alias);
			return _checkboxes.at(alias).specs();
		}

		widgets::text_field&
			containers::page::impl::add_text_field(std::string alias) {
			check_alias(alias);
			if (_text_fields.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _text_fields.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_text_fields.at(alias).specs().alias(alias);
			return _text_fields.at(alias).specs();
		}

		widgets::tree_view&
			containers::page::impl::add_tree(std::string alias) {
			check_alias(alias);
			if (_trees.try_emplace(alias, _pg, alias, _p_direct2d_factory, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _trees.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_trees.at(alias).specs().alias(alias);
			return _trees.at(alias).specs();
		}

		widgets::slider&
			containers::page::impl::add_slider(std::string alias) {
			check_alias(alias);
			if (_sliders.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _sliders.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_sliders.at(alias).specs().alias(alias);
			return _sliders.at(alias).specs();
		}

		widgets::html_editor&
			containers::page::impl::add_html_editor(std::string alias) {
			check_alias(alias);
			if (_html_editors.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _html_editors.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_html_editors.at(alias).specs().alias(alias);
			return _html_editors.at(alias).specs();
		}

		widgets::combobox&
			containers::page::impl::add_combobox(std::string alias) {
			check_alias(alias);
			if (_comboboxes.try_emplace(alias, _pg, alias, _p_directwrite_factory).second) {
				_widgets.emplace(alias, _comboboxes.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_comboboxes.at(alias).specs().alias(alias);
			return _comboboxes.at(alias).specs();
		}

		widgets::line&
			containers::page::impl::add_line(std::string alias) {
			check_alias(alias);
			if (_lines.try_emplace(alias, _pg, alias).second) {
				_widgets.emplace(alias, _lines.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_lines.at(alias).specs().alias(alias);
			return _lines.at(alias).specs();
		}

		widgets::time&
			containers::page::impl::add_time(std::string alias) {
			check_alias(alias);
			if (_times.try_emplace(alias, _pg, alias).second) {
				_widgets.emplace(alias, _times.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_times.at(alias).specs().alias(alias);
			return _times.at(alias).specs();
		}

		widgets::date&
			containers::page::impl::add_date(std::string alias) {
			check_alias(alias);
			if (_dates.try_emplace(alias, _pg, alias).second) {
				_widgets.emplace(alias, _dates.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_dates.at(alias).specs().alias(alias);
			return _dates.at(alias).specs();
		}

		widgets::icon&
			containers::page::impl::add_icon(std::string alias) {
			check_alias(alias);
			if (_icons.try_emplace(alias, _pg, alias).second) {
				_widgets.emplace(alias, _icons.at(alias));
				_widgets_order.emplace_back(alias);
			}
			_icons.at(alias).specs().alias(alias);
			return _icons.at(alias).specs();
		}

		std::map<std::string, widgets::widget_impl&>&
			containers::page::impl::widgets() { return _widgets; }

		const std::vector<std::string>&
			containers::page::impl::widgets_order() { return _widgets_order; }

		widgets::h_scrollbar_impl&
			containers::page::impl::h_scrollbar() { return _h_scrollbar; }
		widgets::v_scrollbar_impl&
			containers::page::impl::v_scrollbar() { return _v_scrollbar; }

		widgets::tab_pane_impl&
			containers::page::impl::get_tab_pane_impl(const std::string& alias) { return _tab_panes.at(alias); }

		widgets::pane_impl&
			containers::page::impl::get_pane_impl(const std::string& alias) { return _panes.at(alias); }

		widgets::rectangle_impl&
			containers::page::impl::get_rectangle_impl(const std::string& alias) { return _rectangles.at(alias); }

		widgets::button_impl&
			containers::page::impl::get_button_impl(const std::string& alias) { return _buttons.at(alias); }

		widgets::label_impl&
			containers::page::impl::get_label_impl(const std::string& alias) { return _labels.at(alias); }

		widgets::group_impl&
			containers::page::impl::get_group_impl(const std::string& alias) { return _groups.at(alias); }

		widgets::toggle_impl&
			containers::page::impl::get_toggle_impl(const std::string& alias) { return _toggles.at(alias); }

		widgets::table_view_impl&
			containers::page::impl::get_table_view_impl(const std::string& alias) { return _table_views.at(alias); }

		widgets::custom_impl&
			containers::page::impl::get_custom_impl(const std::string& alias) { return _customs.at(alias); }

		widgets::image_view_impl&
			containers::page::impl::get_image_view_impl(const std::string& alias) { return _image_views.at(alias); }

		widgets::progress_indicator_impl&
			containers::page::impl::get_progress_indicator_impl(const std::string& alias) { return _progress_indicators.at(alias); }

		widgets::progress_bar_impl&
			containers::page::impl::get_progress_bar_impl(const std::string& alias) { return _progress_bars.at(alias); }

		widgets::checkbox_impl&
			containers::page::impl::get_checkbox_impl(const std::string& alias) { return _checkboxes.at(alias); }

		widgets::text_field_impl&
			containers::page::impl::get_text_field_impl(const std::string& alias) { return _text_fields.at(alias); }

		widgets::tree_view_impl&
			containers::page::impl::get_tree_view_impl(const std::string& alias) { return _trees.at(alias); }

		widgets::slider_impl&
			containers::page::impl::get_slider_impl(const std::string& alias) { return _sliders.at(alias); }

		widgets::html_editor_impl&
			containers::page::impl::get_html_editor_impl(const std::string& alias) { return _html_editors.at(alias); }

		widgets::combobox_impl&
			containers::page::impl::get_combobox_impl(const std::string& alias) { return _comboboxes.at(alias); }

		widgets::line_impl&
			containers::page::impl::get_line_impl(const std::string& alias) { return _lines.at(alias); }

		widgets::time_impl&
			containers::page::impl::get_time_impl(const std::string& alias) { return _times.at(alias); }

		widgets::date_impl&
			containers::page::impl::get_date_impl(const std::string& alias) { return _dates.at(alias); }

		widgets::icon_impl&
			containers::page::impl::get_icon_impl(const std::string& alias) { return _icons.at(alias); }

		bool
			containers::page::impl::close_widget(const std::string& alias,
				widgets::widget_type type,
				std::string& error) {
			// make a local copy since this reference may become invalid before we exit depending
			// on how the closing is being done.
			const std::string _alias(alias);

			// to-do: prevent erasure from _widgets only to throw later in the switch because the data
			// was wrong to begin with!!!!!
			try {
				///
				/// close widget by
				/// 1. removing it from _widgets
				/// 2. removing it from the page it's in, e.g. _tab_panes for tab panes
				/// 3. removing it from _widgets_order

				// step 1
				_widgets.erase(_alias);

				// step 2
				switch (type) {
				case widgets::widget_type::rectangle:
					_rectangles.erase(_alias);
					break;
				case widgets::widget_type::label:
					_labels.erase(_alias);
					break;
				case widgets::widget_type::group:
					_groups.erase(_alias);
					break;
				case widgets::widget_type::tab_pane:
					_tab_panes.erase(_alias);
					break;
				case widgets::widget_type::button:
					_buttons.erase(_alias);
					break;
				case widgets::widget_type::toggle:
					_toggles.erase(_alias);
					break;
				case widgets::widget_type::table_view:
					_table_views.erase(_alias);
					break;
				case widgets::widget_type::custom:
					_customs.erase(_alias);
					break;
				case widgets::widget_type::pane:
					_panes.erase(_alias);
					break;
				case widgets::widget_type::image_view:
					_image_views.erase(_alias);
					break;
				case widgets::widget_type::progress_indicator:
					_progress_indicators.erase(_alias);
					break;
				case widgets::widget_type::progress_bar:
					_progress_bars.erase(_alias);
					break;
				case widgets::widget_type::checkbox:
					_checkboxes.erase(_alias);
					break;
				case widgets::widget_type::text_field:
					_text_fields.erase(_alias);
					break;
				case widgets::widget_type::tree_view:
					_trees.erase(_alias);
					break;
				case widgets::widget_type::slider:
					_sliders.erase(_alias);
					break;
				case widgets::widget_type::html_editor:
					_html_editors.erase(_alias);
					break;
				case widgets::widget_type::combobox:
					_comboboxes.erase(_alias);
					break;
				case widgets::widget_type::line:
					_lines.erase(_alias);
					break;
				case widgets::widget_type::time:
					_times.erase(_alias);
					break;
				case widgets::widget_type::date:
					_dates.erase(_alias);
					break;
				case widgets::widget_type::icon:
					_icons.erase(_alias);
					break;
				case widgets::widget_type::close_button:
				case widgets::widget_type::maximize_button:
				case widgets::widget_type::minimize_button:
				case widgets::widget_type::h_scrollbar:
				case widgets::widget_type::v_scrollbar:
				default:
					break;
				}

				// step 3
				std::vector<std::string> widgets_order;
				for (auto it : _widgets_order)
					if (it != _alias)
						widgets_order.push_back(it);
				_widgets_order = widgets_order;

				return true;
			}
			catch (const std::exception& e) {
				error = e.what();
				return false;
			}
		}

		D2D1_RECT_F containers::page::impl::get_rect() {
			auto rect_pg = D2D1_RECT_F();
			try {
				// check if minimal page border rect contains the point
				auto& rect = _rectangles.at(widgets::rectangle_impl::page_rect_alias());
				rect_pg = rect.get_rect();
				scale_RECT(rect_pg, get_dpi_scale());

				auto scroll_bar_offset = rect.get_scrollbar_offset();
				rect_pg.left += scroll_bar_offset.x;
				rect_pg.right += scroll_bar_offset.x;
				rect_pg.top += scroll_bar_offset.y;
				rect_pg.bottom += scroll_bar_offset.y;

				unscale_RECT(rect_pg, get_dpi_scale());
			}
			catch (const std::exception&) {}
			return rect_pg;
		}

		bool containers::page::impl::hit() { return _hit; }

		bool containers::page::impl::on_mousewheel(float units) {
			if (_hit && _v_scrollbar.visible()) {
				float _row_height = 10.f;
				float adjustment = units * _row_height;
				scroll(adjustment);
				return true;
			}
			else
				return false;
		}

		void containers::page::impl::scroll(float pixels) {
			if (_v_scrollbar.visible()) {
				if (!_scrollbar_set) {
					// check scroll bar
					_v_scrollbar.max_displacement(
						_v_scrollbar._max_displacement_top,
						_v_scrollbar._max_displacement_bottom);
					_v_scrollbar._max_displacement_top *= get_dpi_scale();
					_v_scrollbar._max_displacement_bottom *= get_dpi_scale();

					_v_scrollbar._max_displacement_top += _v_scrollbar._y_displacement;
					_v_scrollbar._max_displacement_bottom += _v_scrollbar._y_displacement;

					_scrollbar_set = true;
				}

				float adjustment = pixels;

				_v_scrollbar._y_displacement -= adjustment;
				_v_scrollbar._y_displacement_previous -= adjustment;
			}
		}

		void containers::page::impl::force_scrollbar_set() { _scrollbar_set = false; }

		void containers::page::impl::check_alias(std::string& alias) {
			// prevent empty alias
			if (alias.empty())
				alias = unique_string_short();
		}
	}
}
