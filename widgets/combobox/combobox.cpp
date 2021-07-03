//
// combobox.cpp - combobox widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../combobox.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::combobox_specs::operator==(const combobox_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(editable_ == param.editable_) &&
				(color_border_ == param.color_border_) &&
				(color_fill_editable_ == param.color_fill_editable_) &&
				(color_caret_ == param.color_caret_) &&
				(color_dropdown_ == param.color_dropdown_) &&
				(color_dropdown_hot_ == param.color_dropdown_hot_) &&
				(color_dropdown_arrow_ == param.color_dropdown_arrow_) &&
				(color_dropdown_arrow_hot_ == param.color_dropdown_arrow_hot_);
		}

		bool widgets::combobox_specs::operator!=(const combobox_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::combobox_specs::text() { return text_; }

		widgets::combobox_specs& widgets::combobox_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::combobox_specs::tooltip() { return tooltip_; }

		widgets::combobox_specs& widgets::combobox_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::combobox_specs::rect() { return rect_; }

		widgets::combobox_specs& widgets::combobox_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::combobox_specs::on_resize() { return on_resize_; }

		widgets::combobox_specs& widgets::combobox_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::combobox_specs::cursor() { return cursor_; }

		widgets::combobox_specs& widgets::combobox_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::combobox_specs::font() { return font_; }

		widgets::combobox_specs& widgets::combobox_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::combobox_specs::font_size() { return font_size_; }

		widgets::combobox_specs& widgets::combobox_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::combobox_specs::color_text() { return color_text_; }

		widgets::combobox_specs& widgets::combobox_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::combobox_specs::color_fill() { return color_fill_; }

		widgets::combobox_specs& widgets::combobox_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::combobox_specs::color_hot() { return color_hot_; }

		widgets::combobox_specs& widgets::combobox_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::combobox_specs::color_selected() { return color_selected_; }

		widgets::combobox_specs& widgets::combobox_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::combobox_specs::color_disabled() { return color_disabled_; }

		widgets::combobox_specs& widgets::combobox_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::combobox_specs::border() { return border_; }

		widgets::combobox_specs& widgets::combobox_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_border() { return color_border_; }

		widgets::combobox_specs& widgets::combobox_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::combobox_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::combobox_specs& widgets::combobox_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::combobox_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::combobox_specs& widgets::combobox_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		bool& widgets::combobox_specs::editable() { return editable_; }

		widgets::combobox_specs& widgets::combobox_specs::editable(const bool& editable) {
			editable_ = editable;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_fill_editable() { return color_fill_editable_; }

		widgets::combobox_specs& widgets::combobox_specs::color_fill_editable(const color& color_fill_editable) {
			color_fill_editable_ = color_fill_editable;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_caret() { return color_caret_; }

		widgets::combobox_specs& widgets::combobox_specs::color_caret(const color& color_caret) {
			color_caret_ = color_caret;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_dropdown() { return color_dropdown_; }

		widgets::combobox_specs& widgets::combobox_specs::color_dropdown(const color& color_dropdown) {
			color_dropdown_ = color_dropdown;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_dropdown_hot() { return color_dropdown_hot_; }

		widgets::combobox_specs& widgets::combobox_specs::color_dropdown_hot(const color& color_dropdown_hot) {
			color_dropdown_hot_ = color_dropdown_hot;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_dropdown_arrow() { return color_dropdown_arrow_; }

		widgets::combobox_specs& widgets::combobox_specs::color_dropdown_arrow(const color& color_dropdown_arrow) {
			color_dropdown_arrow_ = color_dropdown_arrow;
			return *this;
		}

		lecui::color& widgets::combobox_specs::color_dropdown_arrow_hot() { return color_dropdown_arrow_hot_; }

		widgets::combobox_specs& widgets::combobox_specs::color_dropdown_arrow_hot(const color& color_dropdown_arrow_hot) {
			color_dropdown_arrow_hot_ = color_dropdown_arrow_hot;
			return *this;
		}

		std::vector<widgets::combobox_specs::combobox_item>& widgets::combobox_specs::items() { return items_; }

		widgets::combobox_specs& widgets::combobox_specs::items(const std::vector<combobox_item>& items) {
			items_ = items;
			return *this;
		}

		lecui::image_quality& widgets::combobox_specs::quality() { return quality_; }

		widgets::combobox_specs& widgets::combobox_specs::quality(const image_quality& quality) {
			quality_ = quality;
			return *this;
		}

		widgets::combobox_specs& widgets::combobox_specs::sort(const sort_options& sort) {
			sort_ = sort;
			return *this;
		}

		bool& widgets::combobox_specs::force_numerical_sort() { return force_numerical_sort_; }

		widgets::combobox_specs& widgets::combobox_specs::force_numerical_sort(const bool& force_numerical_sort) {
			force_numerical_sort_ = force_numerical_sort;
			return *this;
		}

		std::string& widgets::combobox_specs::selected() { return selected_; }

		widgets::combobox_specs& widgets::combobox_specs::selected(const std::string& selected) {
			selected_ = selected;
			return *this;
		}

		class widgets::combobox_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_combobox(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox))
					.color_fill_editable(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_editable))
					.color_caret(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_caret))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_selected))
					.color_dropdown(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_dropdown))
					.color_dropdown_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_dropdown_hot))
					.color_dropdown_arrow(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_dropdown_arrow))
					.color_dropdown_arrow_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::combobox_dropdown_arrow_hot));
			}
			containers::page& page_;
			combobox_specs& specs_;
		};

		widgets::combobox_builder::combobox_builder(containers::page& page) :
			combobox_builder(page, "") {}

		widgets::combobox_builder::combobox_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::combobox_builder::~combobox_builder() { delete& d_; }

		widgets::combobox_specs&
			widgets::combobox_builder::specs() {
			return d_.specs_;
		}

		widgets::combobox_specs&
			widgets::combobox_builder::operator()() {
			return specs();
		}

		widgets::combobox_specs&
			widgets::combobox_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_combobox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_combobox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
