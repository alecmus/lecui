//
// icon.cpp - icon widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../icon.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::icon_specs::operator==(const icon_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(png_resource_ == param.png_resource_) &&
				(file_ == param.file_) &&
				(color_border_ == param.color_border_) &&
				(color_text_description_ == param.color_text_description_);
		}

		bool widgets::icon_specs::operator!=(const icon_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::icon_specs::text() { return text_; }

		widgets::icon_specs& widgets::icon_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::icon_specs::tooltip() { return tooltip_; }

		widgets::icon_specs& widgets::icon_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::icon_specs::rect() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect();
			else
				return rect_;
		}

		widgets::icon_specs& widgets::icon_specs::rect(const lecui::rect& rect) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect(rect);
			else
				rect_ = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::icon_specs::on_resize() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize();
			else
				return on_resize_;
		}

		widgets::icon_specs& widgets::icon_specs::on_resize(const resize_params& on_resize) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize(on_resize);
			else
				on_resize_ = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::icon_specs::cursor() { return cursor_; }

		widgets::icon_specs& widgets::icon_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::icon_specs::font() { return font_; }

		widgets::icon_specs& widgets::icon_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::icon_specs::font_size() { return font_size_; }

		widgets::icon_specs& widgets::icon_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::icon_specs::color_text() { return color_text_; }

		widgets::icon_specs& widgets::icon_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::icon_specs::color_fill() { return color_fill_; }

		widgets::icon_specs& widgets::icon_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::icon_specs::color_hot() { return color_hot_; }

		widgets::icon_specs& widgets::icon_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::icon_specs::color_selected() { return color_selected_; }

		widgets::icon_specs& widgets::icon_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::icon_specs::color_disabled() { return color_disabled_; }

		widgets::icon_specs& widgets::icon_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::icon_specs::border() { return border_; }

		widgets::icon_specs& widgets::icon_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::icon_specs::color_border() { return color_border_; }

		widgets::icon_specs& widgets::icon_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::icon_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::icon_specs& widgets::icon_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::icon_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::icon_specs& widgets::icon_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		float& widgets::icon_specs::max_image_size() { return max_image_size_; }

		widgets::icon_specs& widgets::icon_specs::max_image_size(const float& max_image_size) {
			max_image_size_ = max_image_size;
			return *this;
		}

		lecui::color& widgets::icon_specs::color_text_description() { return color_text_description_; }

		widgets::icon_specs& widgets::icon_specs::color_text_description(const color& color_text_description) {
			color_text_description_ = color_text_description;
			return *this;
		}

		float& widgets::icon_specs::font_size_description() { return font_size_description_; }

		widgets::icon_specs& widgets::icon_specs::font_size_description(const float& font_size_description) {
			font_size_description_ = font_size_description;
			return *this;
		}

		int& widgets::icon_specs::png_resource() { return png_resource_; }

		widgets::icon_specs& widgets::icon_specs::png_resource(const int& png_resource) {
			png_resource_ = png_resource;
			return *this;
		}

		std::string& widgets::icon_specs::file() { return file_; }

		widgets::icon_specs& widgets::icon_specs::file(const std::string& file) {
			file_ = file;
			return *this;
		}

		liblec::lecui::image_quality& widgets::icon_specs::quality() { return quality_; }

		widgets::icon_specs& widgets::icon_specs::quality(const image_quality& quality) {
			quality_ = quality;
			return *this;
		}

		std::string& widgets::icon_specs::description() { return description_; }

		widgets::icon_specs& widgets::icon_specs::description(const std::string& description) {
			description_ = description;
			return *this;
		}

		widgets::icon_specs::icon_text_position& widgets::icon_specs::text_position() { return text_position_; }

		widgets::icon_specs& widgets::icon_specs::text_position(const icon_text_position& text_position) {
			text_position_ = text_position;
			return *this;
		}

		class widgets::icon_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_icon(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_hot))
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_text))
					.color_text_description(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_description_text));
			}
			containers::page& page_;
			icon_specs& specs_;
		};

		widgets::icon_builder::icon_builder(containers::page& page) :
			icon_builder(page, "") {}

		widgets::icon_builder::icon_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::icon_builder::~icon_builder() { delete& d_; }

		widgets::icon_specs&
			widgets::icon_builder::specs() {
			return d_.specs_;
		}

		widgets::icon_specs&
			widgets::icon_builder::operator()() {
			return specs();
		}

		widgets::icon_specs&
			widgets::icon_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
