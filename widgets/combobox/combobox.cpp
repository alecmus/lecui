//
// combobox.cpp - combobox widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../combobox.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::combobox::operator==(const combobox& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_editable == param._editable) &&
				(_color_border == param._color_border) &&
				(_color_fill_editable == param._color_fill_editable) &&
				(_color_caret == param._color_caret) &&
				(_color_dropdown == param._color_dropdown) &&
				(_color_dropdown_hot == param._color_dropdown_hot) &&
				(_color_dropdown_arrow == param._color_dropdown_arrow) &&
				(_color_dropdown_arrow_hot == param._color_dropdown_arrow_hot);
		}

		bool widgets::combobox::operator!=(const combobox& param) {
			return !operator==(param);
		}

		std::string& widgets::combobox::text() { return _text; }

		widgets::combobox& widgets::combobox::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::combobox::tooltip() { return _tooltip; }

		widgets::combobox& widgets::combobox::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::combobox::rect() { return _rect; }

		widgets::combobox& widgets::combobox::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::combobox::on_resize() { return _on_resize; }

		widgets::combobox& widgets::combobox::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::combobox::cursor() { return _cursor; }

		widgets::combobox& widgets::combobox::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::combobox::font() { return _font; }

		widgets::combobox& widgets::combobox::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::combobox::font_size() { return _font_size; }

		widgets::combobox& widgets::combobox::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::combobox::color_text() { return _color_text; }

		widgets::combobox& widgets::combobox::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::combobox::color_fill() { return _color_fill; }

		widgets::combobox& widgets::combobox::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::combobox::color_hot() { return _color_hot; }

		widgets::combobox& widgets::combobox::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::combobox::color_selected() { return _color_selected; }

		widgets::combobox& widgets::combobox::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::combobox::color_disabled() { return _color_disabled; }

		widgets::combobox& widgets::combobox::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::combobox::border() { return _border; }

		widgets::combobox& widgets::combobox::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::combobox::color_border() { return _color_border; }

		widgets::combobox& widgets::combobox::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::combobox::corner_radius_x() { return _corner_radius_x; }

		widgets::combobox& widgets::combobox::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::combobox::corner_radius_y() { return _corner_radius_y; }

		widgets::combobox& widgets::combobox::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		bool& widgets::combobox::editable() { return _editable; }

		widgets::combobox& widgets::combobox::editable(const bool& editable) {
			_editable = editable;
			return *this;
		}

		lecui::color& widgets::combobox::color_fill_editable() { return _color_fill_editable; }

		widgets::combobox& widgets::combobox::color_fill_editable(const color& color_fill_editable) {
			_color_fill_editable = color_fill_editable;
			return *this;
		}

		lecui::color& widgets::combobox::color_caret() { return _color_caret; }

		widgets::combobox& widgets::combobox::color_caret(const color& color_caret) {
			_color_caret = color_caret;
			return *this;
		}

		lecui::color& widgets::combobox::color_dropdown() { return _color_dropdown; }

		widgets::combobox& widgets::combobox::color_dropdown(const color& color_dropdown) {
			_color_dropdown = color_dropdown;
			return *this;
		}

		lecui::color& widgets::combobox::color_dropdown_hot() { return _color_dropdown_hot; }

		widgets::combobox& widgets::combobox::color_dropdown_hot(const color& color_dropdown_hot) {
			_color_dropdown_hot = color_dropdown_hot;
			return *this;
		}

		lecui::color& widgets::combobox::color_dropdown_arrow() { return _color_dropdown_arrow; }

		widgets::combobox& widgets::combobox::color_dropdown_arrow(const color& color_dropdown_arrow) {
			_color_dropdown_arrow = color_dropdown_arrow;
			return *this;
		}

		lecui::color& widgets::combobox::color_dropdown_arrow_hot() { return _color_dropdown_arrow_hot; }

		widgets::combobox& widgets::combobox::color_dropdown_arrow_hot(const color& color_dropdown_arrow_hot) {
			_color_dropdown_arrow_hot = color_dropdown_arrow_hot;
			return *this;
		}

		std::vector<widgets::combobox::combobox_item>& widgets::combobox::items() { return _items; }

		widgets::combobox& widgets::combobox::items(const std::vector<combobox_item>& items) {
			_items = items;
			return *this;
		}

		lecui::image_quality& widgets::combobox::quality() { return _quality; }

		widgets::combobox& widgets::combobox::quality(const image_quality& quality) {
			_quality = quality;
			return *this;
		}

		widgets::combobox& widgets::combobox::sort(const sort_options& sort) {
			_sort = sort;
			return *this;
		}

		bool& widgets::combobox::force_numerical_sort() { return _force_numerical_sort; }

		widgets::combobox& widgets::combobox::force_numerical_sort(const bool& force_numerical_sort) {
			_force_numerical_sort = force_numerical_sort;
			return *this;
		}

		std::string& widgets::combobox::selected() { return _selected; }

		widgets::combobox& widgets::combobox::selected(const std::string& selected) {
			_selected = selected;
			return *this;
		}

		namespace widgets {
			/// <summary>Combobox widget builder.</summary>
			class combobox_builder {
			public:
				/// <summary>Combobox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				combobox_builder(containers::page& page);

				/// <summary>Combobox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "title".</param>
				combobox_builder(containers::page& page, const std::string& alias);
				~combobox_builder();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				[[nodiscard]]
				combobox& specs();

				/// <summary>Get the combobox specifications.</summary>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				combobox& operator()();

				/// <summary>Get the specifications of a combobox.</summary>
				/// <param name="fm">The form containing the combobox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/title".</param>
				/// <returns>A reference to the combobox specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static combobox& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				combobox_builder() = delete;
				combobox_builder(const combobox_builder&) = delete;
				combobox_builder& operator=(const combobox_builder&) = delete;
			};
		}

		class widgets::combobox_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_combobox(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::combobox))
					.color_fill_editable(defaults::color(_page._d_page._fm._d._theme, item::combobox_editable))
					.color_caret(defaults::color(_page._d_page._fm._d._theme, item::combobox_caret))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::combobox_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::combobox_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::combobox_selected))
					.color_dropdown(defaults::color(_page._d_page._fm._d._theme, item::combobox_dropdown))
					.color_dropdown_hot(defaults::color(_page._d_page._fm._d._theme, item::combobox_dropdown_hot))
					.color_dropdown_arrow(defaults::color(_page._d_page._fm._d._theme, item::combobox_dropdown_arrow))
					.color_dropdown_arrow_hot(defaults::color(_page._d_page._fm._d._theme, item::combobox_dropdown_arrow_hot));
			}
			containers::page& _page;
			combobox& _specs;
		};

		widgets::combobox_builder::combobox_builder(containers::page& page) :
			combobox_builder(page, "") {}

		widgets::combobox_builder::combobox_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::combobox_builder::~combobox_builder() { delete& _d; }

		widgets::combobox&
			widgets::combobox_builder::specs() {
			return _d._specs;
		}

		widgets::combobox&
			widgets::combobox_builder::operator()() {
			return specs();
		}

		widgets::combobox&
			widgets::combobox_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_combobox_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_combobox_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::combobox& liblec::lecui::widgets::combobox::add(containers::page& page, const std::string& alias) {
			return combobox_builder(page, alias).specs();
		}

		widgets::combobox& widgets::combobox::get(form& fm, const std::string& path) {
			return combobox_builder::specs(fm, path);
		}
	}
}
