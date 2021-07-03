//
// image_view.cpp - image view widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../image_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::image_view_specs::operator==(const image_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(file_ == param.file_) &&
				(png_resource_ == param.png_resource_) &&
				(quality_ == param.quality_) &&
				(enlarge_if_smaller_ == param.enlarge_if_smaller_) &&
				(keep_aspect_ratio_ == param.keep_aspect_ratio_);
		}

		bool widgets::image_view_specs::operator!=(const image_view_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::image_view_specs::text() { return text_; }

		widgets::image_view_specs& widgets::image_view_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::image_view_specs::tooltip() { return tooltip_; }

		widgets::image_view_specs& widgets::image_view_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::image_view_specs::rect() { return rect_; }

		widgets::image_view_specs& widgets::image_view_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::image_view_specs::on_resize() { return on_resize_; }

		widgets::image_view_specs& widgets::image_view_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::image_view_specs::cursor() { return cursor_; }

		widgets::image_view_specs& widgets::image_view_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::image_view_specs::font() { return font_; }

		widgets::image_view_specs& widgets::image_view_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::image_view_specs::font_size() { return font_size_; }

		widgets::image_view_specs& widgets::image_view_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::image_view_specs::color_text() { return color_text_; }

		widgets::image_view_specs& widgets::image_view_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::image_view_specs::color_fill() { return color_fill_; }

		widgets::image_view_specs& widgets::image_view_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::image_view_specs::color_hot() { return color_hot_; }

		widgets::image_view_specs& widgets::image_view_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::image_view_specs::color_selected() { return color_selected_; }

		widgets::image_view_specs& widgets::image_view_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::image_view_specs::color_disabled() { return color_disabled_; }

		widgets::image_view_specs& widgets::image_view_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::image_view_specs::border() { return border_; }

		widgets::image_view_specs& widgets::image_view_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::image_view_specs::color_border() { return color_border_; }

		widgets::image_view_specs& widgets::image_view_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& widgets::image_view_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::image_view_specs& widgets::image_view_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::image_view_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::image_view_specs& widgets::image_view_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		int& widgets::image_view_specs::png_resource() { return png_resource_; }

		widgets::image_view_specs& widgets::image_view_specs::png_resource(const int& png_resource) {
			png_resource_ = png_resource;
			return *this;
		}

		std::string& widgets::image_view_specs::file() { return file_; }

		widgets::image_view_specs& widgets::image_view_specs::file(const std::string& file) {
			file_ = file;
			return *this;
		}

		lecui::image_quality& widgets::image_view_specs::quality() { return quality_; }

		widgets::image_view_specs& widgets::image_view_specs::quality(const image_quality& quality) {
			quality_ = quality;
			return *this;
		}

		bool& widgets::image_view_specs::enlarge_if_smaller() { return enlarge_if_smaller_; }

		widgets::image_view_specs& widgets::image_view_specs::enlarge_if_smaller(const bool& enlarge_if_smaller) {
			enlarge_if_smaller_ = enlarge_if_smaller;
			return *this;
		}

		bool& widgets::image_view_specs::keep_aspect_ratio() { return keep_aspect_ratio_; }

		widgets::image_view_specs& widgets::image_view_specs::keep_aspect_ratio(const bool& keep_aspect_ratio) {
			keep_aspect_ratio_ = keep_aspect_ratio;
			return *this;
		}

		class widgets::image_view_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_image_view(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_selected));
			}
			containers::page& page_;
			image_view_specs& specs_;
		};

		widgets::image_view_builder::image_view_builder(containers::page& page) :
			image_view_builder(page, "") {}

		widgets::image_view_builder::image_view_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::image_view_builder::~image_view_builder() { delete& d_; }

		widgets::image_view_specs&
			widgets::image_view_builder::specs() {
			return d_.specs_;
		}

		widgets::image_view_specs&
			widgets::image_view_builder::operator()() {
			return specs();
		}

		widgets::image_view_specs&
			widgets::image_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
