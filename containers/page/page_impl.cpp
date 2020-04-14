/*
** page_impl.cpp - page::impl implementation
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

#include "page_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::impl::impl(form& fm, const std::string& alias) :
			fm_(fm),
			p_direct2d_factory_(nullptr),
			p_directwrite_factory_(nullptr),
			p_iwic_factory_(nullptr),
			alias_(alias),
			h_scrollbar_(alias),
			v_scrollbar_(alias) {
			widgets_.emplace(h_scrollbar_.alias(), h_scrollbar_);
			widgets_.emplace(v_scrollbar_.alias(), v_scrollbar_);
		}

		containers::page::impl::~impl() {}
		const std::string& containers::page::impl::alias() { return alias_; }

		void containers::page::impl::direct2d_factory(ID2D1Factory* p_direct2d_factory) {
			p_direct2d_factory_ = p_direct2d_factory;
		}

		ID2D1Factory* containers::page::impl::direct2d_factory() {
			return p_direct2d_factory_;
		}

		void
			containers::page::impl::directwrite_factory(IDWriteFactory* p_directwrite_factory) {
			p_directwrite_factory_ = p_directwrite_factory;
		}

		IDWriteFactory* containers::page::impl::directwrite_factory() {
			return p_directwrite_factory_;
		}

		void
			containers::page::impl::iwic_factory(IWICImagingFactory* p_iwic_factory) {
			p_iwic_factory_ = p_iwic_factory;
		}

		IWICImagingFactory* containers::page::impl::iwic_factory() {
			return p_iwic_factory_;
		}

		void containers::page::impl::size(const lecui::size& size) { size_ = size; }
		const size& containers::page::impl::size() { return size_; }
		void containers::page::impl::width(const float& width) { size_.width = width; }
		const float containers::page::impl::width() { return size_.width; }
		void containers::page::impl::height(const float& height) { size_.height = height; }
		const float containers::page::impl::height() { return size_.height; }

		bool containers::page::impl::contains(const D2D1_POINT_2F& point) {
			try {
				// check if minimal page border rect contains the point
				auto& rect = rectangles_.at(widgets_impl::rectangle::page_rect_alias());
				return rect.contains(point);
			}
			catch (const std::exception&) {}
			return false;
		}

		containers::tab_pane_specs&
			containers::page::impl::add_tab_pane(const std::string& alias) {
			tab_panes_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, tab_panes_.at(alias));
			widgets_order_.emplace_back(alias);
			return tab_panes_.at(alias).specs();
		}

		containers::pane_specs&
			containers::page::impl::add_pane(const std::string& alias) {
			panes_.try_emplace(alias, alias_, alias);
			widgets_.emplace(alias, panes_.at(alias));
			widgets_order_.emplace_back(alias);
			return panes_.at(alias).specs();
		}

		widgets::rectangle_specs&
			containers::page::impl::add_rectangle(const std::string& alias) {
			rectangles_.try_emplace(alias, alias_, alias);
			widgets_.emplace(alias, rectangles_.at(alias));
			widgets_order_.emplace_back(alias);
			return rectangles_.at(alias).specs();
		}

		widgets::label_specs&
			containers::page::impl::add_label(const std::string& alias) {
			labels_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, labels_.at(alias));
			widgets_order_.emplace_back(alias);
			return labels_.at(alias).specs();
		}

		containers::group_specs&
			containers::page::impl::add_group(const std::string& alias) {
			groups_.try_emplace(alias, alias_, alias);
			widgets_.emplace(alias, groups_.at(alias));
			widgets_order_.emplace_back(alias);
			return groups_.at(alias).specs();
		}

		widgets::button_specs&
			containers::page::impl::add_button(const std::string& alias) {
			buttons_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, buttons_.at(alias));
			widgets_order_.emplace_back(alias);
			return buttons_.at(alias).specs();
		}

		widgets::toggle_specs&
			containers::page::impl::add_toggle(const std::string& alias) {
			toggles_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, toggles_.at(alias));
			widgets_order_.emplace_back(alias);
			return toggles_.at(alias).specs();
		}

		widgets::combo_specs&
			containers::page::impl::add_combo(const std::string& alias) {
			combos_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, combos_.at(alias));
			widgets_order_.emplace_back(alias);
			return combos_.at(alias).specs();
		}

		widgets::list_specs&
			containers::page::impl::add_list(const std::string& alias) {
			lists_.try_emplace(alias, alias_, alias, p_directwrite_factory_);
			widgets_.emplace(alias, lists_.at(alias));
			widgets_order_.emplace_back(alias);
			return lists_.at(alias).specs();
		}

		widgets::custom_specs&
			containers::page::impl::add_custom(const std::string& alias) {
			customs_.try_emplace(alias, alias_, alias, p_directwrite_factory_, p_iwic_factory_);
			widgets_.emplace(alias, customs_.at(alias));
			widgets_order_.emplace_back(alias);
			return customs_.at(alias).specs();
		}

		widgets::image_specs&
			containers::page::impl::add_image(const std::string& alias) {
			images_.try_emplace(alias, alias_, alias, p_iwic_factory_);
			widgets_.emplace(alias, images_.at(alias));
			widgets_order_.emplace_back(alias);
			return images_.at(alias).specs();
		}

		widgets::progress_indicator_specs&
			containers::page::impl::add_progress_indicator(const std::string& alias) {
			progress_indicators_.try_emplace(alias, alias_, alias, p_direct2d_factory_, p_directwrite_factory_);
			widgets_.emplace(alias, progress_indicators_.at(alias));
			widgets_order_.emplace_back(alias);
			return progress_indicators_.at(alias).specs();
		}

		widgets::progress_bar_specs&
			containers::page::impl::add_progress_bar(const std::string& alias) {
			progress_bars_.try_emplace(alias, alias_, alias, p_direct2d_factory_, p_directwrite_factory_);
			widgets_.emplace(alias, progress_bars_.at(alias));
			widgets_order_.emplace_back(alias);
			return progress_bars_.at(alias).specs();
		}

		widgets::checkbox_specs&
			containers::page::impl::add_checkbox(const std::string& alias) {
			checkboxes_.try_emplace(alias, alias_, alias, p_direct2d_factory_, p_directwrite_factory_);
			widgets_.emplace(alias, checkboxes_.at(alias));
			widgets_order_.emplace_back(alias);
			return checkboxes_.at(alias).specs();
		}

		widgets::textbox_specs&
			containers::page::impl::add_textbox(const std::string& alias) {
			textboxes_.try_emplace(alias, alias_, alias, fm_, p_directwrite_factory_);
			widgets_.emplace(alias, textboxes_.at(alias));
			widgets_order_.emplace_back(alias);
			return textboxes_.at(alias).specs();
		}

		widgets::tree_specs&
			containers::page::impl::add_tree(const std::string& alias) {
			trees_.try_emplace(alias, alias_, alias, p_direct2d_factory_, p_directwrite_factory_);
			widgets_.emplace(alias, trees_.at(alias));
			widgets_order_.emplace_back(alias);
			return trees_.at(alias).specs();
		}

		std::map<std::string, widgets_impl::widget&>&
			containers::page::impl::widgets() { return widgets_; }

		const std::vector<std::string>&
			containers::page::impl::widgets_order() { return widgets_order_; }

		widgets_impl::h_scrollbar&
			containers::page::impl::h_scrollbar() { return h_scrollbar_; }
		widgets_impl::v_scrollbar&
			containers::page::impl::v_scrollbar() { return v_scrollbar_; }

		widgets_impl::tab_pane&
			containers::page::impl::get_tab_pane(const std::string& alias) { return tab_panes_.at(alias); }

		widgets_impl::pane&
			containers::page::impl::get_pane(const std::string& alias) { return panes_.at(alias); }

		widgets_impl::rectangle&
			containers::page::impl::get_rectangle(const std::string& alias) { return rectangles_.at(alias); }

		widgets_impl::button&
			containers::page::impl::get_button(const std::string& alias) { return buttons_.at(alias); }

		widgets_impl::label&
			containers::page::impl::get_label(const std::string& alias) { return labels_.at(alias); }

		widgets_impl::group&
			containers::page::impl::get_group(const std::string& alias) { return groups_.at(alias); }

		widgets_impl::toggle&
			containers::page::impl::get_toggle(const std::string& alias) { return toggles_.at(alias); }

		widgets_impl::combo&
			containers::page::impl::get_combo(const std::string& alias) { return combos_.at(alias); }

		widgets_impl::list&
			containers::page::impl::get_list(const std::string& alias) { return lists_.at(alias); }

		widgets_impl::custom&
			containers::page::impl::get_custom(const std::string& alias) { return customs_.at(alias); }

		widgets_impl::image&
			containers::page::impl::get_image(const std::string& alias) { return images_.at(alias); }

		widgets_impl::progress_indicator&
			containers::page::impl::get_progress_indicator(const std::string& alias) { return progress_indicators_.at(alias); }

		widgets_impl::progress_bar&
			containers::page::impl::get_progress_bar(const std::string& alias) { return progress_bars_.at(alias); }

		widgets_impl::checkbox&
			containers::page::impl::get_checkbox(const std::string& alias) { return checkboxes_.at(alias); }

		widgets_impl::textbox&
			containers::page::impl::get_textbox(const std::string& alias) { return textboxes_.at(alias); }

		widgets_impl::tree&
			containers::page::impl::get_tree(const std::string& alias) { return trees_.at(alias); }

		bool
			containers::page::impl::close_widget(const std::string& alias,
				widgets_impl::widget_type type,
				std::string& error) {
			// to-do: prevent erasure from widgets_ only to throw later in the switch because the data
			// was wrong to begin with!!!!!
			try {
				///
				/// close widget by
				/// 1. removing it from widgets_
				/// 2. removing it from the page it's in, e.g. tab_panes_ for tab panes
				/// 3. removing it from widgets_order_

				// step 1
				widgets_.erase(alias);

				// step 2
				switch (type) {
				case widgets_impl::widget_type::rectangle:
					rectangles_.erase(alias);
					break;
				case widgets_impl::widget_type::label:
					labels_.erase(alias);
					break;
				case widgets_impl::widget_type::group:
					groups_.erase(alias);
					break;
				case widgets_impl::widget_type::tab_pane:
					tab_panes_.erase(alias);
					break;
				case widgets_impl::widget_type::button:
					buttons_.erase(alias);
					break;
				case widgets_impl::widget_type::toggle:
					toggles_.erase(alias);
					break;
				case widgets_impl::widget_type::combo:
					combos_.erase(alias);
					break;
				case widgets_impl::widget_type::list:
					lists_.erase(alias);
					break;
				case widgets_impl::widget_type::custom:
					customs_.erase(alias);
					break;
				case widgets_impl::widget_type::pane:
					panes_.erase(alias);
					break;
				case widgets_impl::widget_type::image:
					images_.erase(alias);
					break;
				case widgets_impl::widget_type::progress_indicator:
					progress_indicators_.erase(alias);
					break;
				case widgets_impl::widget_type::progress_bar:
					progress_bars_.erase(alias);
					break;
				case widgets_impl::widget_type::checkbox:
					checkboxes_.erase(alias);
					break;
				case widgets_impl::widget_type::textbox:
					textboxes_.erase(alias);
					break;
				case widgets_impl::widget_type::tree:
					trees_.erase(alias);
					break;
				case widgets_impl::widget_type::close_button:
				case widgets_impl::widget_type::maximize_button:
				case widgets_impl::widget_type::minimize_button:
				case widgets_impl::widget_type::h_scrollbar:
				case widgets_impl::widget_type::v_scrollbar:
				default:
					break;
				}

				// step 3
				std::vector<std::string> widgets_order;
				for (auto it : widgets_order_)
					if (it != alias)
						widgets_order.push_back(it);
				widgets_order_ = widgets_order;

				return true;
			}
			catch (const std::exception& e) {
				error = e.what();
				return false;
			}
		}
	}
}
