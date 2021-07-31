//
// pane.cpp - pane container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		namespace containers {
			/// <summary>Pane container builder.</summary>
			class pane_builder {
			public:
				/// <summary>Pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="content_margin">The margin to use inside the pane.</param>
				/// <remarks>This constructs the container with an internally generated random
				/// alias.</remarks>
				pane_builder(containers::page& page, const float& content_margin = 10.f);

				/// <summary>Pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the pane in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_pane".</param>
				/// <param name="content_margin">The margin to use inside the pane.</param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a pane leads to undefined behavior.</remarks>
				pane_builder(containers::page& page, const std::string& alias, const float& content_margin = 10.f);
				~pane_builder();

				[[nodiscard]]
				pane& get();

				[[nodiscard]]
				static pane& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				pane_builder() = delete;
				pane_builder(const pane_builder&) = delete;
				pane_builder& operator=(const pane_builder&) = delete;
			};
		}

		class containers::pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::pane& p,
				const std::string& alias) :
				_page(page), _p(p) {
				_p
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::pane))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::pane_border));
			}
			containers::page& _page;
			containers::pane& _p;
		};

		containers::pane_builder::pane_builder(containers::page& page, const float& content_margin) :
			pane_builder(page, "", content_margin) {}

		containers::pane_builder::pane_builder(containers::page& page,
			const std::string& alias, const float& content_margin) :
			_d(*(new impl(page, page._d_page.add_pane(alias, content_margin), alias))) {}

		containers::pane_builder::~pane_builder() { delete& _d; }

		containers::pane& containers::pane_builder::get() {
			return _d._p;
		}

		containers::pane& containers::pane_builder::get(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);

					auto& impl = results.page._d_page.get_pane_impl(results.widget.alias());
					return impl._p_panes.at(impl._current_pane);
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					auto& impl = results.page._d_page.get_pane_impl(results.widget.alias());
					return impl._p_panes.at(impl._current_pane);
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		std::string& containers::pane_specs::text() { return _text; }

		containers::pane_specs& containers::pane_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::pane_specs::tooltip() { return _tooltip; }

		containers::pane_specs& containers::pane_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& containers::pane_specs::rect() { return _rect; }

		containers::pane_specs& containers::pane_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& containers::pane_specs::on_resize() { return _on_resize; }

		containers::pane_specs& containers::pane_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& containers::pane_specs::cursor() { return _cursor; }

		containers::pane_specs& containers::pane_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::pane_specs::font() { return _font; }

		containers::pane_specs& containers::pane_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::pane_specs::font_size() { return _font_size; }

		containers::pane_specs& containers::pane_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::pane_specs::color_text() { return _color_text; }

		containers::pane_specs& containers::pane_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::pane_specs::color_fill() { return _color_fill; }

		containers::pane_specs& containers::pane_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::pane_specs::color_hot() { return _color_hot; }

		containers::pane_specs& containers::pane_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::pane_specs::color_selected() { return _color_selected; }

		containers::pane_specs& containers::pane_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::pane_specs::color_disabled() { return _color_disabled; }

		containers::pane_specs& containers::pane_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::pane_specs::border() { return _border; }

		containers::pane_specs& containers::pane_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::pane_specs::color_border() { return _color_border; }

		containers::pane_specs& containers::pane_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::pane_specs::corner_radius_x() { return _corner_radius_x; }

		containers::pane_specs& containers::pane_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::pane_specs::corner_radius_y() { return _corner_radius_y; }

		containers::pane_specs& containers::pane_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		containers::pane::pane(form& fm, const std::string& alias) :
			page(fm, alias) {}

		const lecui::size containers::pane::size() {
			lecui::size s;
			s.width(rect().width() - 2.f * _d_page._content_margin);
			s.height(rect().height() - 2.f * _d_page._content_margin);
			return s;
		}

		containers::pane& liblec::lecui::containers::pane::add(containers::page& page, const std::string& alias, const float& content_margin) {
			return containers::pane_builder(page, alias, content_margin).get();
		}

		containers::pane& containers::pane::get(form& fm, const std::string& path) {
			return containers::pane_builder::get(fm, path);
		}
	}
}
