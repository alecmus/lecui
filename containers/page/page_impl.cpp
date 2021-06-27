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

		void containers::page::impl::parent(containers::page& p) {
			parent_ = p;
		}

		std::optional<std::reference_wrapper<containers::page>>
			containers::page::impl::parent() {
			return parent_;
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
				auto& rect = rectangles_.at(widgets::rectangle_impl::page_rect_alias());
				contains_ = rect.contains(point);
			}
			catch (const std::exception&) {}

			hit_ = contains_;

			if (!contains_)
				scrollbar_set_ = false;

			return contains_;
		}

		containers::tab_pane_specs&
			containers::page::impl::add_tab_pane(std::string alias) {
			check_alias(alias);
			if (tab_panes_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, tab_panes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			tab_panes_.at(alias).specs().alias(alias);
			return tab_panes_.at(alias).specs();
		}

		containers::pane_specs&
			containers::page::impl::add_pane(std::string alias) {
			check_alias(alias);
			if (panes_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, panes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			panes_.at(alias).specs().alias(alias);
			return panes_.at(alias).specs();
		}

		widgets::rectangle_specs&
			containers::page::impl::add_rectangle(std::string alias) {
			check_alias(alias);
			if (rectangles_.try_emplace(alias, pg_, alias, h_scrollbar_, v_scrollbar_).second) {
				widgets_.emplace(alias, rectangles_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			rectangles_.at(alias).specs().alias(alias);
			return rectangles_.at(alias).specs();
		}

		widgets::label_specs&
			containers::page::impl::add_label(std::string alias) {
			check_alias(alias);
			if (labels_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, labels_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			labels_.at(alias).specs().alias(alias);
			return labels_.at(alias).specs();
		}

		containers::group_specs&
			containers::page::impl::add_group(std::string alias) {
			check_alias(alias);
			if (groups_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, groups_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			groups_.at(alias).specs().alias(alias);
			return groups_.at(alias).specs();
		}

		widgets::button_specs&
			containers::page::impl::add_button(std::string alias) {
			check_alias(alias);
			if (buttons_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, buttons_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			buttons_.at(alias).specs().alias(alias);
			return buttons_.at(alias).specs();
		}

		widgets::toggle_specs&
			containers::page::impl::add_toggle(std::string alias) {
			check_alias(alias);
			if (toggles_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, toggles_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			toggles_.at(alias).specs().alias(alias);
			return toggles_.at(alias).specs();
		}

		widgets::table_view_specs&
			containers::page::impl::add_table_view(std::string alias) {
			check_alias(alias);
			if (table_views_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, table_views_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			table_views_.at(alias).specs().alias(alias);
			return table_views_.at(alias).specs();
		}

		widgets::custom_specs&
			containers::page::impl::add_custom(std::string alias) {
			check_alias(alias);
			if (customs_.try_emplace(alias, pg_, alias, p_directwrite_factory_, p_iwic_factory_).second) {
				widgets_.emplace(alias, customs_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			customs_.at(alias).specs().alias(alias);
			return customs_.at(alias).specs();
		}

		widgets::image_view_specs&
			containers::page::impl::add_image_view(std::string alias) {
			check_alias(alias);
			if (image_views_.try_emplace(alias, pg_, alias, p_iwic_factory_).second) {
				widgets_.emplace(alias, image_views_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			image_views_.at(alias).specs().alias(alias);
			return image_views_.at(alias).specs();
		}

		widgets::progress_indicator_specs&
			containers::page::impl::add_progress_indicator(std::string alias) {
			check_alias(alias);
			if (progress_indicators_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, progress_indicators_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			progress_indicators_.at(alias).specs().alias(alias);
			return progress_indicators_.at(alias).specs();
		}

		widgets::progress_bar_specs&
			containers::page::impl::add_progress_bar(std::string alias) {
			check_alias(alias);
			if (progress_bars_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, progress_bars_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			progress_bars_.at(alias).specs().alias(alias);
			return progress_bars_.at(alias).specs();
		}

		widgets::checkbox_specs&
			containers::page::impl::add_checkbox(std::string alias) {
			check_alias(alias);
			if (checkboxes_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, checkboxes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			checkboxes_.at(alias).specs().alias(alias);
			return checkboxes_.at(alias).specs();
		}

		widgets::text_field_specs&
			containers::page::impl::add_text_field(std::string alias) {
			check_alias(alias);
			if (text_fields_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, text_fields_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			text_fields_.at(alias).specs().alias(alias);
			return text_fields_.at(alias).specs();
		}

		widgets::tree_view_specs&
			containers::page::impl::add_tree(std::string alias) {
			check_alias(alias);
			if (trees_.try_emplace(alias, pg_, alias, p_direct2d_factory_, p_directwrite_factory_).second) {
				widgets_.emplace(alias, trees_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			trees_.at(alias).specs().alias(alias);
			return trees_.at(alias).specs();
		}

		widgets::slider_specs&
			containers::page::impl::add_slider(std::string alias) {
			check_alias(alias);
			if (sliders_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, sliders_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			sliders_.at(alias).specs().alias(alias);
			return sliders_.at(alias).specs();
		}

		widgets::html_editor_specs&
			containers::page::impl::add_html_editor(std::string alias) {
			check_alias(alias);
			if (html_editors_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, html_editors_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			html_editors_.at(alias).specs().alias(alias);
			return html_editors_.at(alias).specs();
		}

		widgets::combobox_specs&
			containers::page::impl::add_combobox(std::string alias) {
			check_alias(alias);
			if (comboboxes_.try_emplace(alias, pg_, alias, p_directwrite_factory_).second) {
				widgets_.emplace(alias, comboboxes_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			comboboxes_.at(alias).specs().alias(alias);
			return comboboxes_.at(alias).specs();
		}

		widgets::line_specs&
			containers::page::impl::add_line(std::string alias) {
			check_alias(alias);
			if (lines_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, lines_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			lines_.at(alias).specs().alias(alias);
			return lines_.at(alias).specs();
		}

		widgets::time_specs&
			containers::page::impl::add_time(std::string alias) {
			check_alias(alias);
			if (times_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, times_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			times_.at(alias).specs().alias(alias);
			return times_.at(alias).specs();
		}

		widgets::date_specs&
			containers::page::impl::add_date(std::string alias) {
			check_alias(alias);
			if (dates_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, dates_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			dates_.at(alias).specs().alias(alias);
			return dates_.at(alias).specs();
		}

		widgets::icon_specs&
			containers::page::impl::add_icon(std::string alias) {
			check_alias(alias);
			if (icons_.try_emplace(alias, pg_, alias).second) {
				widgets_.emplace(alias, icons_.at(alias));
				widgets_order_.emplace_back(alias);
			}
			icons_.at(alias).specs().alias(alias);
			return icons_.at(alias).specs();
		}

		std::map<std::string, widgets::widget_impl&>&
			containers::page::impl::widgets() { return widgets_; }

		const std::vector<std::string>&
			containers::page::impl::widgets_order() { return widgets_order_; }

		widgets::h_scrollbar_impl&
			containers::page::impl::h_scrollbar() { return h_scrollbar_; }
		widgets::v_scrollbar_impl&
			containers::page::impl::v_scrollbar() { return v_scrollbar_; }

		widgets::tab_pane_impl&
			containers::page::impl::get_tab_pane(const std::string& alias) { return tab_panes_.at(alias); }

		widgets::pane_impl&
			containers::page::impl::get_pane(const std::string& alias) { return panes_.at(alias); }

		widgets::rectangle_impl&
			containers::page::impl::get_rectangle(const std::string& alias) { return rectangles_.at(alias); }

		widgets::button_impl&
			containers::page::impl::get_button(const std::string& alias) { return buttons_.at(alias); }

		widgets::label_impl&
			containers::page::impl::get_label(const std::string& alias) { return labels_.at(alias); }

		widgets::group_impl&
			containers::page::impl::get_group(const std::string& alias) { return groups_.at(alias); }

		widgets::toggle_impl&
			containers::page::impl::get_toggle(const std::string& alias) { return toggles_.at(alias); }

		widgets::table_view_impl&
			containers::page::impl::get_table_view(const std::string& alias) { return table_views_.at(alias); }

		widgets::custom_impl&
			containers::page::impl::get_custom(const std::string& alias) { return customs_.at(alias); }

		widgets::image_view_impl&
			containers::page::impl::get_image_view(const std::string& alias) { return image_views_.at(alias); }

		widgets::progress_indicator_impl&
			containers::page::impl::get_progress_indicator(const std::string& alias) { return progress_indicators_.at(alias); }

		widgets::progress_bar_impl&
			containers::page::impl::get_progress_bar(const std::string& alias) { return progress_bars_.at(alias); }

		widgets::checkbox_impl&
			containers::page::impl::get_checkbox(const std::string& alias) { return checkboxes_.at(alias); }

		widgets::text_field_impl&
			containers::page::impl::get_text_field(const std::string& alias) { return text_fields_.at(alias); }

		widgets::tree_view_impl&
			containers::page::impl::get_tree(const std::string& alias) { return trees_.at(alias); }

		widgets::slider_impl&
			containers::page::impl::get_slider(const std::string& alias) { return sliders_.at(alias); }

		widgets::html_editor_impl&
			containers::page::impl::get_html_editor(const std::string& alias) { return html_editors_.at(alias); }

		widgets::combobox_impl&
			containers::page::impl::get_combobox(const std::string& alias) { return comboboxes_.at(alias); }

		widgets::line_impl&
			containers::page::impl::get_line(const std::string& alias) { return lines_.at(alias); }

		widgets::time_impl&
			containers::page::impl::get_time(const std::string& alias) { return times_.at(alias); }

		widgets::date_impl&
			containers::page::impl::get_date(const std::string& alias) { return dates_.at(alias); }

		widgets::icon_impl&
			containers::page::impl::get_icon(const std::string& alias) { return icons_.at(alias); }

		bool
			containers::page::impl::close_widget(const std::string& alias,
				widgets::widget_type type,
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
				case widgets::widget_type::rectangle:
					rectangles_.erase(alias_);
					break;
				case widgets::widget_type::label:
					labels_.erase(alias_);
					break;
				case widgets::widget_type::group:
					groups_.erase(alias_);
					break;
				case widgets::widget_type::tab_pane:
					tab_panes_.erase(alias_);
					break;
				case widgets::widget_type::button:
					buttons_.erase(alias_);
					break;
				case widgets::widget_type::toggle:
					toggles_.erase(alias_);
					break;
				case widgets::widget_type::table_view:
					table_views_.erase(alias_);
					break;
				case widgets::widget_type::custom:
					customs_.erase(alias_);
					break;
				case widgets::widget_type::pane:
					panes_.erase(alias_);
					break;
				case widgets::widget_type::image_view:
					image_views_.erase(alias_);
					break;
				case widgets::widget_type::progress_indicator:
					progress_indicators_.erase(alias_);
					break;
				case widgets::widget_type::progress_bar:
					progress_bars_.erase(alias_);
					break;
				case widgets::widget_type::checkbox:
					checkboxes_.erase(alias_);
					break;
				case widgets::widget_type::text_field:
					text_fields_.erase(alias_);
					break;
				case widgets::widget_type::tree_view:
					trees_.erase(alias_);
					break;
				case widgets::widget_type::slider:
					sliders_.erase(alias_);
					break;
				case widgets::widget_type::html_editor:
					html_editors_.erase(alias_);
					break;
				case widgets::widget_type::combobox:
					comboboxes_.erase(alias_);
					break;
				case widgets::widget_type::line:
					lines_.erase(alias_);
					break;
				case widgets::widget_type::time:
					times_.erase(alias_);
					break;
				case widgets::widget_type::date:
					dates_.erase(alias_);
					break;
				case widgets::widget_type::icon:
					icons_.erase(alias_);
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
				auto& rect = rectangles_.at(widgets::rectangle_impl::page_rect_alias());
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
