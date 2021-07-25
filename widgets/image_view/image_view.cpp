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
				(_color_border == param._color_border) &&
				(_file == param._file) &&
				(_png_resource == param._png_resource) &&
				(_quality == param._quality) &&
				(_enlarge_if_smaller == param._enlarge_if_smaller) &&
				(_keep_aspect_ratio == param._keep_aspect_ratio);
		}

		bool widgets::image_view_specs::operator!=(const image_view_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::image_view_specs::text() { return _text; }

		widgets::image_view_specs& widgets::image_view_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::image_view_specs::tooltip() { return _tooltip; }

		widgets::image_view_specs& widgets::image_view_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::image_view_specs::rect() { return _rect; }

		widgets::image_view_specs& widgets::image_view_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::resize_params& widgets::image_view_specs::on_resize() { return _on_resize; }

		widgets::image_view_specs& widgets::image_view_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::image_view_specs::cursor() { return _cursor; }

		widgets::image_view_specs& widgets::image_view_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::image_view_specs::font() { return _font; }

		widgets::image_view_specs& widgets::image_view_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::image_view_specs::font_size() { return _font_size; }

		widgets::image_view_specs& widgets::image_view_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::image_view_specs::color_text() { return _color_text; }

		widgets::image_view_specs& widgets::image_view_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::image_view_specs::color_fill() { return _color_fill; }

		widgets::image_view_specs& widgets::image_view_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::image_view_specs::color_hot() { return _color_hot; }

		widgets::image_view_specs& widgets::image_view_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::image_view_specs::color_selected() { return _color_selected; }

		widgets::image_view_specs& widgets::image_view_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::image_view_specs::color_disabled() { return _color_disabled; }

		widgets::image_view_specs& widgets::image_view_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::image_view_specs::border() { return _border; }

		widgets::image_view_specs& widgets::image_view_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::image_view_specs::color_border() { return _color_border; }

		widgets::image_view_specs& widgets::image_view_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::image_view_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::image_view_specs& widgets::image_view_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::image_view_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::image_view_specs& widgets::image_view_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		int& widgets::image_view_specs::png_resource() { return _png_resource; }

		widgets::image_view_specs& widgets::image_view_specs::png_resource(const int& png_resource) {
			_png_resource = png_resource;
			return *this;
		}

		std::string& widgets::image_view_specs::file() { return _file; }

		widgets::image_view_specs& widgets::image_view_specs::file(const std::string& file) {
			_file = file;
			return *this;
		}

		lecui::image_quality& widgets::image_view_specs::quality() { return _quality; }

		widgets::image_view_specs& widgets::image_view_specs::quality(const image_quality& quality) {
			_quality = quality;
			return *this;
		}

		bool& widgets::image_view_specs::enlarge_if_smaller() { return _enlarge_if_smaller; }

		widgets::image_view_specs& widgets::image_view_specs::enlarge_if_smaller(const bool& enlarge_if_smaller) {
			_enlarge_if_smaller = enlarge_if_smaller;
			return *this;
		}

		bool& widgets::image_view_specs::keep_aspect_ratio() { return _keep_aspect_ratio; }

		widgets::image_view_specs& widgets::image_view_specs::keep_aspect_ratio(const bool& keep_aspect_ratio) {
			_keep_aspect_ratio = keep_aspect_ratio;
			return *this;
		}

		class widgets::image_view_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_image_view(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::image_view))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::image_view_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::image_view_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::image_view_selected));
			}
			containers::page& _page;
			image_view_specs& _specs;
		};

		widgets::image_view_builder::image_view_builder(containers::page& page) :
			image_view_builder(page, "") {}

		widgets::image_view_builder::image_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::image_view_builder::~image_view_builder() { delete& _d; }

		widgets::image_view_specs&
			widgets::image_view_builder::specs() {
			return _d._specs;
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
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
