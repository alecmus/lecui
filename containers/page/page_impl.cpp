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
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::impl::impl(form& fm, containers::page& pg, const std::string& alias) :
			fm_(fm),
			pg_(pg),
			p_direct2d_factory_(nullptr),
			p_directwrite_factory_(nullptr),
			p_iwic_factory_(nullptr),
			alias_(alias),
			hit_(false),
			scrollbar_set_(false),
			h_scrollbar_(pg),
			v_scrollbar_(pg) {
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

		float containers::page::impl::get_dpi_scale() {
			return fm_.d_.get_dpi_scale();
		}

		form& containers::page::impl::get_form() {
			return fm_;
		}

		void containers::page::impl::size(const lecui::size& size) { size_ = size; }
		const size& containers::page::impl::size() { return size_; }
		void containers::page::impl::width(const float& width) { size_.width = width; }
		const float containers::page::impl::width() { return size_.width; }
		void containers::page::impl::height(const float& height) { size_.height = height; }
		const float containers::page::impl::height() { return size_.height; }

		bool containers::page::impl::contains(const D2D1_POINT_2F& point) {
			bool contains_ = false;

			try {
				// check if minimal page border rect contains the point
				auto& rect = rectangles_.at(widgets_impl::rectangle::page_rect_alias());
				contains_ = rect.contains(point);
			}
			catch (const std::exception&) {}

			hit_ = contains_;

			if (!contains_)
				scrollbar_set_ = false;

			return contains_;
		}

		containers::tab_pane::tab_pane_specs&
			containers::page::impl::add_tab_pane(std::string alias) {
			check_alias(alias);
			if (tab_panes_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, tab_panes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return tab_panes_.at(alias).specs();
		}

		containers::pane::pane_specs&
			containers::page::impl::add_pane(std::string alias) {
			check_alias(alias);
			if (panes_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, panes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return panes_.at(alias).specs();
		}

		widgets::rectangle::rectangle_specs&
			containers::page::impl::add_rectangle(std::string alias) {
			check_alias(alias);
			if (rectangles_.try_emplace(alias, pg_, alias, h_scrollbar_, v_scrollbar_).second) {
				widgets_.emplace(alias, rectangles_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return rectangles_.at(alias).specs();
		}

		widgets::label::label_specs&
			containers::page::impl::add_label(std::string alias) {
			check_alias(alias);
			if (labels_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, labels_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return labels_.at(alias).specs();
		}

		containers::group::group_specs&
			containers::page::impl::add_group(std::string alias) {
			check_alias(alias);
			if (groups_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, groups_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return groups_.at(alias).specs();
		}

		widgets::button::button_specs&
			containers::page::impl::add_button(std::string alias) {
			check_alias(alias);
			if (buttons_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, buttons_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return buttons_.at(alias).specs();
		}

		widgets::toggle::toggle_specs&
			containers::page::impl::add_toggle(std::string alias) {
			check_alias(alias);
			if (toggles_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, toggles_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return toggles_.at(alias).specs();
		}

		widgets::table::table_specs&
			containers::page::impl::add_table(std::string alias) {
			check_alias(alias);
			if (tables_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, tables_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return tables_.at(alias).specs();
		}

		widgets::custom::custom_specs&
			containers::page::impl::add_custom(std::string alias) {
			check_alias(alias);
			if (customs_.try_emplace(alias, pg_, alias, p_directwrite_factory_, p_iwic_factory_).second) {
				widgets_.emplace(alias, customs_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return customs_.at(alias).specs();
		}

		widgets::image::image_specs&
			containers::page::impl::add_image(std::string alias) {
			check_alias(alias);
			if (images_.try_emplace(alias, pg_, alias, p_iwic_factory_).second) {
				widgets_.emplace(alias, images_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return images_.at(alias).specs();
		}

		widgets::progress_indicator::progress_indicator_specs&
			containers::page::impl::add_progress_indicator(std::string alias) {
			check_alias(alias);
			if (progress_indicators_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, progress_indicators_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return progress_indicators_.at(alias).specs();
		}

		widgets::progress_bar::progress_bar_specs&
			containers::page::impl::add_progress_bar(std::string alias) {
			check_alias(alias);
			if (progress_bars_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, progress_bars_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return progress_bars_.at(alias).specs();
		}

		widgets::checkbox::checkbox_specs&
			containers::page::impl::add_checkbox(std::string alias) {
			check_alias(alias);
			if (checkboxes_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, checkboxes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return checkboxes_.at(alias).specs();
		}

		widgets::textbox::textbox_specs&
			containers::page::impl::add_textbox(std::string alias) {
			check_alias(alias);
			if (textboxes_.try_emplace(alias, pg_, alias, fm_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, textboxes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return textboxes_.at(alias).specs();
		}

		widgets::tree::tree_specs&
			containers::page::impl::add_tree(std::string alias) {
			check_alias(alias);
			if (trees_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, trees_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return trees_.at(alias).specs();
		}

		widgets::slider::slider_specs&
			containers::page::impl::add_slider(std::string alias) {
			check_alias(alias);
			if (sliders_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, sliders_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return sliders_.at(alias).specs();
		}

		widgets::html_editor::html_editor_specs&
			containers::page::impl::add_html_editor(std::string alias) {
			check_alias(alias);
			if (html_editors_.try_emplace(alias, pg_, alias, fm_, pg_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, html_editors_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return html_editors_.at(alias).specs();
		}

		widgets::combobox::combobox_specs&
			containers::page::impl::add_combobox(std::string alias) {
			check_alias(alias);
			if (comboboxes_.try_emplace(alias, pg_, alias, fm_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, comboboxes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return comboboxes_.at(alias).specs();
		}

		widgets::line::line_specs&
			containers::page::impl::add_line(std::string alias) {
			check_alias(alias);
			if (lines_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, lines_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			return lines_.at(alias).specs();
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

		widgets_impl::table&
			containers::page::impl::get_table(const std::string& alias) { return tables_.at(alias); }

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

		widgets_impl::slider&
			containers::page::impl::get_slider(const std::string& alias) { return sliders_.at(alias); }

		widgets_impl::html_editor&
			containers::page::impl::get_html_editor(const std::string& alias) { return html_editors_.at(alias); }

		widgets_impl::combobox&
			containers::page::impl::get_combobox(const std::string& alias) { return comboboxes_.at(alias); }

		widgets_impl::line&
			containers::page::impl::get_line(const std::string& alias) { return lines_.at(alias); }

		bool
			containers::page::impl::close_widget(const std::string& alias,
				widgets_impl::widget_type type,
				std::string& error) {
			// make a local copy since this reference may become invalid before we exit depending
			// on how the closing is being done.
			const std::string alias_(alias);

			// to-do: prevent erasure from widgets_ only to throw later in the switch because the data
			// was wrong to begin with!!!!!
			try {
				///
				/// close widget by
				/// 1. removing it from widgets_
				/// 2. removing it from the page it's in, e.g. tab_panes_ for tab panes
				/// 3. removing it from widgets_order_

				// step 1
				widgets_.erase(alias_);

				// step 2
				switch (type) {
				case widgets_impl::widget_type::rectangle:
					rectangles_.erase(alias_);
					break;
				case widgets_impl::widget_type::label:
					labels_.erase(alias_);
					break;
				case widgets_impl::widget_type::group:
					groups_.erase(alias_);
					break;
				case widgets_impl::widget_type::tab_pane:
					tab_panes_.erase(alias_);
					break;
				case widgets_impl::widget_type::button:
					buttons_.erase(alias_);
					break;
				case widgets_impl::widget_type::toggle:
					toggles_.erase(alias_);
					break;
				case widgets_impl::widget_type::table:
					tables_.erase(alias_);
					break;
				case widgets_impl::widget_type::custom:
					customs_.erase(alias_);
					break;
				case widgets_impl::widget_type::pane:
					panes_.erase(alias_);
					break;
				case widgets_impl::widget_type::image:
					images_.erase(alias_);
					break;
				case widgets_impl::widget_type::progress_indicator:
					progress_indicators_.erase(alias_);
					break;
				case widgets_impl::widget_type::progress_bar:
					progress_bars_.erase(alias_);
					break;
				case widgets_impl::widget_type::checkbox:
					checkboxes_.erase(alias_);
					break;
				case widgets_impl::widget_type::textbox:
					textboxes_.erase(alias_);
					break;
				case widgets_impl::widget_type::tree:
					trees_.erase(alias_);
					break;
				case widgets_impl::widget_type::slider:
					sliders_.erase(alias_);
					break;
				case widgets_impl::widget_type::html_editor:
					html_editors_.erase(alias_);
					break;
				case widgets_impl::widget_type::combobox:
					comboboxes_.erase(alias_);
					break;
				case widgets_impl::widget_type::line:
					lines_.erase(alias_);
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
					if (it != alias_)
						widgets_order.push_back(it);
				widgets_order_ = widgets_order;

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
				auto& rect = rectangles_.at(widgets_impl::rectangle::page_rect_alias());
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

		bool containers::page::impl::hit() { return hit_; }

		bool containers::page::impl::on_mousewheel(float units) {
			if (hit_ && v_scrollbar_.visible()) {
				float row_height_ = 10.f;
				float adjustment = units * row_height_;
				scroll(adjustment);
				return true;
			}
			else
				return false;
		}

		void containers::page::impl::scroll(float pixels) {
			if (v_scrollbar_.visible()) {
				if (!scrollbar_set_) {
					// check scroll bar
					v_scrollbar_.max_displacement(
						v_scrollbar_.max_displacement_top_,
						v_scrollbar_.max_displacement_bottom_);
					v_scrollbar_.max_displacement_top_ *= get_dpi_scale();
					v_scrollbar_.max_displacement_bottom_ *= get_dpi_scale();

					v_scrollbar_.max_displacement_top_ += v_scrollbar_.y_displacement_;
					v_scrollbar_.max_displacement_bottom_ += v_scrollbar_.y_displacement_;

					scrollbar_set_ = true;
				}

				float adjustment = pixels;

				v_scrollbar_.y_displacement_ -= adjustment;
				v_scrollbar_.y_displacement_previous_ -= adjustment;
			}
		}

		void containers::page::impl::force_scrollbar_set() { scrollbar_set_ = false; }

		void containers::page::impl::check_alias(std::string& alias) {
			// prevent empty alias
			if (alias.empty())
				alias = unique_string_short();
		}
	}
}
