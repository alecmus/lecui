//
// group.cpp - group container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../group.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool containers::group::operator==(const group& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border);
		}

		bool containers::group::operator!=(const group& param) {
			return !operator==(param);
		}

		std::string& containers::group::text() { return _text; }

		containers::group& containers::group::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::group::tooltip() { return _tooltip; }

		containers::group& containers::group::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& containers::group::badge() { return _badge; }

		containers::group& containers::group::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& containers::group::rect() { return _rect; }

		containers::group& containers::group::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& containers::group::on_resize() { return _on_resize; }

		containers::group& containers::group::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& containers::group::cursor() { return _cursor; }

		containers::group& containers::group::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::group::font() { return _font; }

		containers::group& containers::group::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::group::font_size() { return _font_size; }

		containers::group& containers::group::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::group::color_text() { return _color_text; }

		containers::group& containers::group::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::group::color_fill() { return _color_fill; }

		containers::group& containers::group::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::group::color_hot() { return _color_hot; }

		containers::group& containers::group::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::group::color_selected() { return _color_selected; }

		containers::group& containers::group::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::group::color_disabled() { return _color_disabled; }

		containers::group& containers::group::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::group::border() { return _border; }

		containers::group& containers::group::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::group::color_border() { return _color_border; }

		containers::group& containers::group::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::group::corner_radius_x() { return _corner_radius_x; }

		containers::group& containers::group::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::group::corner_radius_y() { return _corner_radius_y; }

		containers::group& containers::group::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		float& containers::group::margin() { return _margin; }

		containers::group& containers::group::margin(const float& margin) {
			_margin = margin;
			return *this;
		}

		std::vector<std::string>& containers::group::widgets() { return _widgets; }

		containers::group& containers::group::widgets(const std::vector<std::string>& widgets) {
			_widgets = widgets;
			return *this;
		}

		namespace containers {
			/// <summary>Group container builder.</summary>
			class group_builder {
			public:
				/// <summary>Group builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				group_builder(containers::page& page);

				/// <summary>Group builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "left_group".</param>
				group_builder(containers::page& page, const std::string& alias);
				~group_builder();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				[[nodiscard]]
				group& specs();

				/// <summary>Add a group container.</summary>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				group& operator()();

				/// <summary>Get the specifications of a group.</summary>
				/// <param name="fm">The form containing the group.</param>
				/// <param name="path">The full path to the container, e.g.
				/// "sample_page/left_group".</param>
				/// <returns>A reference to the group specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static group& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				group_builder() = delete;
				group_builder(const group_builder&) = delete;
				group_builder& operator=(const group_builder&) = delete;
			};
		};

		class containers::group_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_group(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::group))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::group_border));
			}
			containers::page& _page;
			group& _specs;
		};

		containers::group_builder::group_builder(containers::page& page) :
			group_builder(page, "") {}

		containers::group_builder::group_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		containers::group_builder::~group_builder() { delete& _d; }

		containers::group&
			containers::group_builder::specs() {
			return _d._specs;
		}

		containers::group&
			containers::group_builder::operator()() {
			return specs();
		}

		containers::group&
			containers::group_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_group_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_group_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		containers::group& liblec::lecui::containers::group::add(containers::page& page, const std::string& alias) {
			return group_builder(page, alias).specs();
		}

		containers::group& liblec::lecui::containers::group::get(form& fm, const std::string& path) {
			return group_builder::specs(fm, path);
		}
	}
}
