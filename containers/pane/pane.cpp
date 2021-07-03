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
		class containers::pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::pane))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::pane_border));
			}
			containers::page& page_;
			containers::pane_specs& specs_;
		};

		containers::pane_builder::pane_builder(containers::page& page) :
			pane_builder(page, "") {}

		containers::pane_builder::pane_builder(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_pane(alias), alias))) {}

		containers::pane_builder::~pane_builder() { delete& d_; }

		containers::pane_specs& containers::pane_builder::specs() {
			return d_.specs_;
		}

		containers::pane_specs& containers::pane_builder::operator()() {
			return specs();
		}

		containers::pane_specs&
			containers::pane_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		containers::page& containers::pane_builder::get() {
			auto& pane_ = d_.page_.d_page_.get_pane(d_.specs_.alias());

			const std::string pane_name = "pane";

			if (pane_.p_panes_.count(pane_name))
				return pane_.p_panes_.at(pane_name);

			pane_.p_panes_.try_emplace(pane_name, d_.page_.d_page_.fm_, pane_name);
			pane_.current_pane_ = pane_name;
			auto& page_impl = pane_.p_panes_.at(pane_name).d_page_;

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(d_.page_.d_page_.direct2d_factory());

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(d_.page_.d_page_.directwrite_factory());

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(d_.page_.d_page_.iwic_factory());

			// specify parent
			page_impl.parent(d_.page_);

			const float thickness = 10.f;
			const float margin = 10.f;
			const float page_tolerance_ = 10.f;
			rect rect_client_area = pane_().rect();

			// initialize the page's horizontal scroll bar
			{
				auto& specs_ = page_impl.h_scrollbar().specs();
				specs_.on_resize().perc_width = 100.f;
				specs_.on_resize().perc_y = 100.f;

				specs_.rect()
					.left(0.f)
					.right((rect_client_area.right() - rect_client_area.left()) - (margin + thickness))
					.bottom((rect_client_area.bottom() - rect_client_area.top()) - page_tolerance_)
					.top(specs_.rect().bottom() - thickness);

				specs_
					.color_fill(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
					.color_scrollbar_border(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
					.color_hot(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
			}

			// initialize the page's vertical scroll bar
			{
				auto& specs_ = page_impl.v_scrollbar().specs();
				specs_.on_resize().perc_height = 100.f;
				specs_.on_resize().perc_x = 100.f;

				specs_.rect()
					.top(0)
					.bottom((rect_client_area.bottom() - rect_client_area.top()) - (margin + thickness))
					.right((rect_client_area.right() - rect_client_area.left()) - margin)
					.left(specs_.rect().right() - thickness);

				specs_
					.color_fill(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
					.color_scrollbar_border(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
					.color_hot(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
			}

			// set page size
			page_impl.size({ rect_client_area.width(), rect_client_area.height() });
			page_impl.width(page_impl.width() - (2.f * page_tolerance_));
			page_impl.height(page_impl.height() - (2.f * page_tolerance_));

			// add an invisible rect to bound the page. This is essential for scroll bars
			// to work appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
			rectangle.color_fill().alpha(0);

			// make it transparent
			rectangle.color_border({ 255, 0, 0, 0 }).color_hot({ 255, 0, 0, 0 })

				// set its dimensions to exactly match the page
				.corner_radius_x(15.f)
				.corner_radius_y(15.f)
				.rect().size(page_impl.size());

			rectangle.on_resize().perc_width = 100.f;
			rectangle.on_resize().perc_height = 100.f;

			// return reference to page so caller can add widgets to it
			return pane_.p_panes_.at(pane_name);
		}

		containers::page&
			containers::pane_builder::get(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			try {
				// check form pages
				if (idx == std::string::npos)
					return fm.d_.p_pages_.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm.d_.p_pages_.at(page_alias);
					return fm.d_.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}
			try {
				// check status panes
				if (idx == std::string::npos)
					return fm.d_.p_status_panes_.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					return fm.d_.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}

			throw std::invalid_argument("Invalid path");
		}

		std::string& containers::pane_specs::text() { return text_; }

		containers::pane_specs& containers::pane_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& containers::pane_specs::tooltip() { return tooltip_; }

		containers::pane_specs& containers::pane_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& containers::pane_specs::rect() { return rect_; }

		containers::pane_specs& containers::pane_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& containers::pane_specs::on_resize() { return on_resize_; }

		containers::pane_specs& containers::pane_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::pane_specs::cursor() { return cursor_; }

		containers::pane_specs& containers::pane_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& containers::pane_specs::font() { return font_; }

		containers::pane_specs& containers::pane_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& containers::pane_specs::font_size() { return font_size_; }

		containers::pane_specs& containers::pane_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& containers::pane_specs::color_text() { return color_text_; }

		containers::pane_specs& containers::pane_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& containers::pane_specs::color_fill() { return color_fill_; }

		containers::pane_specs& containers::pane_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& containers::pane_specs::color_hot() { return color_hot_; }

		containers::pane_specs& containers::pane_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& containers::pane_specs::color_selected() { return color_selected_; }

		containers::pane_specs& containers::pane_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& containers::pane_specs::color_disabled() { return color_disabled_; }

		containers::pane_specs& containers::pane_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& containers::pane_specs::border() { return border_; }

		containers::pane_specs& containers::pane_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& containers::pane_specs::color_border() { return color_border_; }

		containers::pane_specs& containers::pane_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& containers::pane_specs::corner_radius_x() { return corner_radius_x_; }

		containers::pane_specs& containers::pane_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& containers::pane_specs::corner_radius_y() { return corner_radius_y_; }

		containers::pane_specs& containers::pane_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}
	}
}
