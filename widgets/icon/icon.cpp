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
				(_png_resource == param._png_resource) &&
				(_file == param._file) &&
				(_color_border == param._color_border) &&
				(_color_text_description == param._color_text_description);
		}

		bool widgets::icon_specs::operator!=(const icon_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::icon_specs::text() { return _text; }

		widgets::icon_specs& widgets::icon_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::icon_specs::tooltip() { return _tooltip; }

		widgets::icon_specs& widgets::icon_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::icon_specs::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::icon_specs& widgets::icon_specs::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		widgets::resize_params& widgets::icon_specs::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::icon_specs& widgets::icon_specs::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::icon_specs::cursor() { return _cursor; }

		widgets::icon_specs& widgets::icon_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::icon_specs::font() { return _font; }

		widgets::icon_specs& widgets::icon_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::icon_specs::font_size() { return _font_size; }

		widgets::icon_specs& widgets::icon_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::icon_specs::color_text() { return _color_text; }

		widgets::icon_specs& widgets::icon_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::icon_specs::color_fill() { return _color_fill; }

		widgets::icon_specs& widgets::icon_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::icon_specs::color_hot() { return _color_hot; }

		widgets::icon_specs& widgets::icon_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::icon_specs::color_selected() { return _color_selected; }

		widgets::icon_specs& widgets::icon_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::icon_specs::color_disabled() { return _color_disabled; }

		widgets::icon_specs& widgets::icon_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::icon_specs::border() { return _border; }

		widgets::icon_specs& widgets::icon_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::icon_specs::color_border() { return _color_border; }

		widgets::icon_specs& widgets::icon_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::icon_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::icon_specs& widgets::icon_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::icon_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::icon_specs& widgets::icon_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		float& widgets::icon_specs::max_image_size() { return _max_image_size; }

		widgets::icon_specs& widgets::icon_specs::max_image_size(const float& max_image_size) {
			_max_image_size = max_image_size;
			return *this;
		}

		lecui::color& widgets::icon_specs::color_text_description() { return _color_text_description; }

		widgets::icon_specs& widgets::icon_specs::color_text_description(const color& color_text_description) {
			_color_text_description = color_text_description;
			return *this;
		}

		float& widgets::icon_specs::font_size_description() { return _font_size_description; }

		widgets::icon_specs& widgets::icon_specs::font_size_description(const float& font_size_description) {
			_font_size_description = font_size_description;
			return *this;
		}

		int& widgets::icon_specs::png_resource() { return _png_resource; }

		widgets::icon_specs& widgets::icon_specs::png_resource(const int& png_resource) {
			_png_resource = png_resource;
			return *this;
		}

		std::string& widgets::icon_specs::file() { return _file; }

		widgets::icon_specs& widgets::icon_specs::file(const std::string& file) {
			_file = file;
			return *this;
		}

		liblec::lecui::image_quality& widgets::icon_specs::quality() { return _quality; }

		widgets::icon_specs& widgets::icon_specs::quality(const image_quality& quality) {
			_quality = quality;
			return *this;
		}

		std::string& widgets::icon_specs::description() { return _description; }

		widgets::icon_specs& widgets::icon_specs::description(const std::string& description) {
			_description = description;
			return *this;
		}

		widgets::icon_specs::icon_text_position& widgets::icon_specs::text_position() { return _text_position; }

		widgets::icon_specs& widgets::icon_specs::text_position(const icon_text_position& text_position) {
			_text_position = text_position;
			return *this;
		}

		class widgets::icon_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_icon(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::icon))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::icon_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::icon_hot))
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::icon_text))
					.color_text_description(defaults::color(_page._d_page._fm._d._theme, item::icon_description_text));
			}
			containers::page& _page;
			icon_specs& _specs;
		};

		widgets::icon_builder::icon_builder(containers::page& page) :
			icon_builder(page, "") {}

		widgets::icon_builder::icon_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::icon_builder::~icon_builder() { delete& _d; }

		widgets::icon_specs&
			widgets::icon_builder::specs() {
			return _d._specs;
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
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
