//
// image_view.cpp - image view widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../image_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::image_view::operator==(const image_view& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_file == param._file) &&
				(_png_resource == param._png_resource) &&
				(_quality == param._quality) &&
				(_enlarge_if_smaller == param._enlarge_if_smaller) &&
				(_keep_aspect_ratio == param._keep_aspect_ratio);
		}

		bool widgets::image_view::operator!=(const image_view& param) {
			return !operator==(param);
		}

		std::string& widgets::image_view::text() { return _text; }

		widgets::image_view& widgets::image_view::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::image_view::tooltip() { return _tooltip; }

		widgets::image_view& widgets::image_view::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::badge_specs& widgets::image_view::badge() { return _badge; }

		widgets::image_view& widgets::image_view::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::image_view::rect() { return _rect; }

		widgets::image_view& widgets::image_view::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::image_view::on_resize() { return _on_resize; }

		widgets::image_view& widgets::image_view::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::image_view::cursor() { return _cursor; }

		widgets::image_view& widgets::image_view::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::image_view::font() { return _font; }

		widgets::image_view& widgets::image_view::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::image_view::font_size() { return _font_size; }

		widgets::image_view& widgets::image_view::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::image_view::color_text() { return _color_text; }

		widgets::image_view& widgets::image_view::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::image_view::color_fill() { return _color_fill; }

		widgets::image_view& widgets::image_view::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::image_view::color_hot() { return _color_hot; }

		widgets::image_view& widgets::image_view::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::image_view::color_selected() { return _color_selected; }

		widgets::image_view& widgets::image_view::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::image_view::color_disabled() { return _color_disabled; }

		widgets::image_view& widgets::image_view::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::image_view::border() { return _border; }

		widgets::image_view& widgets::image_view::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::image_view::color_border() { return _color_border; }

		widgets::image_view& widgets::image_view::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::image_view::corner_radius_x() { return _corner_radius_x; }

		widgets::image_view& widgets::image_view::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::image_view::corner_radius_y() { return _corner_radius_y; }

		widgets::image_view& widgets::image_view::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		int& widgets::image_view::png_resource() { return _png_resource; }

		widgets::image_view& widgets::image_view::png_resource(const int& png_resource) {
			_png_resource = png_resource;
			return *this;
		}

		std::string& widgets::image_view::file() { return _file; }

		widgets::image_view& widgets::image_view::file(const std::string& file) {
			_file = file;
			return *this;
		}

		lecui::image_quality& widgets::image_view::quality() { return _quality; }

		widgets::image_view& widgets::image_view::quality(const image_quality& quality) {
			_quality = quality;
			return *this;
		}

		bool& widgets::image_view::enlarge_if_smaller() { return _enlarge_if_smaller; }

		widgets::image_view& widgets::image_view::enlarge_if_smaller(const bool& enlarge_if_smaller) {
			_enlarge_if_smaller = enlarge_if_smaller;
			return *this;
		}

		bool& widgets::image_view::keep_aspect_ratio() { return _keep_aspect_ratio; }

		widgets::image_view& widgets::image_view::keep_aspect_ratio(const bool& keep_aspect_ratio) {
			_keep_aspect_ratio = keep_aspect_ratio;
			return *this;
		}

		namespace widgets {
			/// <summary>Image view widget builder.</summary>
			class image_view_builder {
			public:
				/// <summary>Image view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				image_view_builder(containers::page& page);

				/// <summary>Image view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "avatar".</param>
				image_view_builder(containers::page& page, const std::string& alias);
				~image_view_builder();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				[[nodiscard]]
				image_view& specs();

				/// <summary>Get the specifications of the image view.</summary>
				/// <returns>A reference to the image view specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				image_view& operator()();

				/// <summary>Get the specifications of an image.</summary>
				/// <param name="fm">The form containing the image.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/pane_two/avatar".</param>
				/// <returns>A reference to the image specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static image_view& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				image_view_builder() = delete;
				image_view_builder(const image_view_builder&) = delete;
				image_view_builder& operator=(const image_view_builder&) = delete;
			};
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
			image_view& _specs;
		};

		widgets::image_view_builder::image_view_builder(containers::page& page) :
			image_view_builder(page, "") {}

		widgets::image_view_builder::image_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::image_view_builder::~image_view_builder() { delete& _d; }

		widgets::image_view&
			widgets::image_view_builder::specs() {
			return _d._specs;
		}

		widgets::image_view&
			widgets::image_view_builder::operator()() {
			return specs();
		}

		widgets::image_view&
			widgets::image_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_image_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_image_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::image_view& liblec::lecui::widgets::image_view::add(containers::page& page, const std::string& alias) {
			return image_view_builder(page, alias).specs();
		}

		widgets::image_view& widgets::image_view::get(form& fm, const std::string& path) {
			return image_view_builder::specs(fm, path);
		}
	}
}
