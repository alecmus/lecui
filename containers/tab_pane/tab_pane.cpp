//
// tab_pane.cpp - tab pane implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../tab_pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::tab_pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::tab_pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_pane))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_pane_border))
					.color_tabs(defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_bar))
					.color_tabs_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_pane_border))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_selected));
			}
			containers::page& page_;
			containers::tab_pane_specs& specs_;
		};

		containers::tab_pane_builder::tab_pane_builder(containers::page& page) :
			tab_pane_builder(page, "") {}

		containers::tab_pane_builder::tab_pane_builder(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_tab_pane(alias), alias))) {}

		containers::tab_pane_builder::~tab_pane_builder() { delete& d_; }

		containers::tab_pane_specs& containers::tab_pane_builder::specs() {
			return d_.specs_;
		}

		containers::tab_pane_specs& containers::tab_pane_builder::operator()() {
			return specs();
		}

		containers::tab_pane_specs&
			containers::tab_pane_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		void containers::tab_pane_builder::select(const std::string& tab_name) {
			auto& tab_pane_ = d_.page_.d_page_.get_tab_pane(d_.specs_.alias());
			tab_pane_.current_tab_ = tab_name;
		}

		class containers::tab_builder::impl {
		public:
			impl(containers::tab_pane_builder& tp, const std::string& tab_name) :
				tp_(tp),
				page_(add(tp, tab_name)) {}
			containers::tab_pane_builder& tp_;
			containers::page& page_;

			static containers::page& add(tab_pane_builder& tp, const std::string& tab_name) {
				auto& tab_pane_ = tp.d_.page_.d_page_.get_tab_pane(tp.d_.specs_.alias());

				if (tab_pane_.p_tabs_.count(tab_name)) {
					log("library usage error containers::tab::impl::add");

					/// to-do: find a mechanism that makes sense ...
					return tab_pane_.p_tabs_.at(tab_name);
				}

				tab_pane_.p_tabs_.try_emplace(tab_name, tp.d_.page_.d_page_.fm_, tab_name);
				tab_pane_.tab_order_.push_back(tab_name);
				auto& page_impl = tab_pane_.p_tabs_.at(tab_name).d_page_;

				// specify direct2d factory (used internally for geometries and stuff)
				page_impl.direct2d_factory(tp.d_.page_.d_page_.direct2d_factory());

				// specify directwrite factory (used internally for text rendering)
				page_impl.directwrite_factory(tp.d_.page_.d_page_.directwrite_factory());

				// specify iwic imaging factory (used internally for image rendering)
				page_impl.iwic_factory(tp.d_.page_.d_page_.iwic_factory());

				// specify parent
				page_impl.parent(tp.d_.page_);

				const float thickness = 10.f;
				const float margin = 10.f;
				const float page_tolerance_ = 10.f;
				float tab_height_ = tab_pane_.tab_height();
				rect rect_client_area = tab_pane_().rect();

				bool tabs_perpendicular = false;

				switch (tp.d_.specs_.tab_side()) {
				case tab_pane_specs::side::left:
				case tab_pane_specs::side::right:
					if (tp.d_.specs_.caption_orientation() == tab_pane_specs::orientation::horizontal)
						tabs_perpendicular = true;
					break;

				case tab_pane_specs::side::top:
				case tab_pane_specs::side::bottom:
				default: 
					if (tp.d_.specs_.caption_orientation() == tab_pane_specs::orientation::vertical)
						tabs_perpendicular = true;
					break;
				}

				if (tabs_perpendicular && !tab_pane_.tab_height_set()) {
					if (!tab_pane_.specs().caption_reserve().empty()) {
						log(tab_name + ": using caption reserve to compute dimensions");
						// compute the longest tab caption in the reserve
						for (const auto& alias : tab_pane_.specs().caption_reserve()) {
							D2D1_RECT_F max_rect = { 0.f, 0.f, tab_pane_.specs().rect().height(), tab_pane_.caption_bar_height() };
							auto caption_rect = widgets::measure_label(tp.d_.page_.d_page_.p_directwrite_factory_, alias, tab_pane_.specs().font(),
								tab_pane_.specs().font_size(), false, true, max_rect);

							tab_height_ = largest(tab_height_, caption_rect.right - caption_rect.left + 3.f * tab_pane_.padding());
						}
					}
					else {
						log(tab_name + ": WARNING - no caption reserve for perpendicular tab captions!");
						// use current caption to set tab height
						D2D1_RECT_F max_rect = { 0.f, 0.f, tab_pane_.specs().rect().height(), tab_pane_.caption_bar_height() };
						auto caption_rect = widgets::measure_label(tp.d_.page_.d_page_.p_directwrite_factory_, tab_name, tab_pane_.specs().font(),
							tab_pane_.specs().font_size(), false, true, max_rect);

						tab_height_ = largest(tab_height_, caption_rect.right - caption_rect.left + 3.f * tab_pane_.padding());
					}

					tab_pane_.set_tab_height(tab_height_);
				}

				const auto caption_bar_height_ = tab_pane_.caption_bar_height();

				// initialize the page's horizontal scroll bar
				{
					auto& specs_ = page_impl.h_scrollbar().specs();
					specs_.on_resize().perc_width = 100.f;
					specs_.on_resize().perc_y = 100.f;

					switch (tp.d_.specs_.tab_side()) {
					case tab_pane_specs::side::left:
					case tab_pane_specs::side::right:
						specs_.rect()
							.left(0.f)
							.right((rect_client_area.right() - rect_client_area.left()) - (margin + thickness) - caption_bar_height_)
							.bottom((rect_client_area.bottom() - rect_client_area.top()) - page_tolerance_)
							.top(specs_.rect().bottom() - thickness);
						break;

					case tab_pane_specs::side::top:
					case tab_pane_specs::side::bottom:
					default:
						specs_.rect()
							.left(0.f)
							.right((rect_client_area.right() - rect_client_area.left()) - (margin + thickness))
							.bottom((rect_client_area.bottom() - rect_client_area.top()) - (caption_bar_height_ + page_tolerance_))
							.top(specs_.rect().bottom() - thickness);
						break;
					}

					specs_
						.color_fill(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
						.color_scrollbar_border(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
						.color_hot(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
						.color_hot_pressed(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
				}

				// initialize the page's vertical scroll bar
				{
					auto& specs_ = page_impl.v_scrollbar().specs();
					specs_.on_resize().perc_height = 100.f;
					specs_.on_resize().perc_x = 100.f;

					switch (tp.d_.specs_.tab_side()) {
					case tab_pane_specs::side::left:
					case tab_pane_specs::side::right:
						specs_.rect()
							.top(0.f)
							.bottom((rect_client_area.bottom() - rect_client_area.top()) - (margin + thickness))
							.right((rect_client_area.right() - rect_client_area.left()) - margin - caption_bar_height_)
							.left(specs_.rect().right() - thickness);
						break;

					case tab_pane_specs::side::top:
					case tab_pane_specs::side::bottom:
					default:
						specs_.rect()
							.top(0.f)
							.bottom((rect_client_area.bottom() - rect_client_area.top()) - (margin + thickness) - caption_bar_height_)
							.right((rect_client_area.right() - rect_client_area.left()) - margin)
							.left(specs_.rect().right() - thickness);
						break;
					}

					specs_
						.color_fill(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
						.color_scrollbar_border(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
						.color_hot(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
						.color_hot_pressed(defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
				}

				// set page size
				page_impl.size({ rect_client_area.width(), rect_client_area.height() });

				switch (tp.d_.specs_.tab_side()) {
				case tab_pane_specs::side::left:
				case tab_pane_specs::side::right:
					page_impl.width(page_impl.width() - (2.f * page_tolerance_ + caption_bar_height_));
					page_impl.height(page_impl.height() - (2.f * page_tolerance_));
					break;

				case tab_pane_specs::side::top:
				case tab_pane_specs::side::bottom:
				default:
					page_impl.width(page_impl.width() - (2.f * page_tolerance_));
					page_impl.height(page_impl.height() - (2.f * page_tolerance_ + caption_bar_height_));
					break;
				}

				// add an invisible rect to bound the page. This is essential for scroll bars
				// to work appropriately when contents don't reach the page borders
				auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
				rectangle.color_fill().alpha(0);

				// make it transparent
				rectangle
					.color_border({ 255, 0, 0, 0 })
					.color_hot({ 255, 0, 0, 0 })
					.corner_radius_x(15.f)
					.corner_radius_y(15.f)

					// set its dimensions to exactly match the page
					.rect().size(page_impl.size());

				rectangle.on_resize().perc_width = 100.f;
				rectangle.on_resize().perc_height = 100.f;

				// return reference to page so caller can add widgets to it
				return tab_pane_.p_tabs_.at(tab_name);
			}
		};

		containers::tab_builder::tab_builder(containers::tab_pane_builder& tp, const std::string& tab_name) :
			d_(*(new impl(tp, tab_name))) {}

		containers::tab_builder::~tab_builder() { delete& d_; }

		containers::page& containers::tab_builder::get() {
			return d_.page_;
		}

		containers::page&
			containers::tab_builder::get(form& fm, const std::string& path) {
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

		std::string& containers::tab_pane_specs::text() { return text_; }

		containers::tab_pane_specs& containers::tab_pane_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& containers::tab_pane_specs::tooltip() { return tooltip_; }

		containers::tab_pane_specs& containers::tab_pane_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& containers::tab_pane_specs::rect() { return rect_; }

		containers::tab_pane_specs& containers::tab_pane_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& containers::tab_pane_specs::on_resize() { return on_resize_; }

		containers::tab_pane_specs& containers::tab_pane_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::tab_pane_specs::cursor() { return cursor_; }

		containers::tab_pane_specs& containers::tab_pane_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& containers::tab_pane_specs::font() { return font_; }

		containers::tab_pane_specs& containers::tab_pane_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& containers::tab_pane_specs::font_size() { return font_size_; }

		containers::tab_pane_specs& containers::tab_pane_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& containers::tab_pane_specs::color_text() { return color_text_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& containers::tab_pane_specs::color_fill() { return color_fill_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& containers::tab_pane_specs::color_hot() { return color_hot_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& containers::tab_pane_specs::color_selected() { return color_selected_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& containers::tab_pane_specs::color_disabled() { return color_disabled_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& containers::tab_pane_specs::border() { return border_; }

		containers::tab_pane_specs& containers::tab_pane_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_border() { return color_border_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		float& containers::tab_pane_specs::corner_radius_x() { return corner_radius_x_; }

		containers::tab_pane_specs& containers::tab_pane_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& containers::tab_pane_specs::corner_radius_y() { return corner_radius_y_; }

		containers::tab_pane_specs& containers::tab_pane_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		containers::tab_pane_specs::side& containers::tab_pane_specs::tab_side() { return tab_side_; }

		containers::tab_pane_specs& containers::tab_pane_specs::tab_side(const side& tab_side) {
			tab_side_ = tab_side;
			return *this;
		}

		std::vector<std::string>& containers::tab_pane_specs::caption_reserve() { return caption_reserve_; }

		containers::tab_pane_specs& containers::tab_pane_specs::caption_reserve(const std::vector<std::string>& caption_reserve) {
			caption_reserve_ = caption_reserve;
			return *this;
		}

		containers::tab_pane_specs::orientation& containers::tab_pane_specs::caption_orientation() { return caption_orientation_; }

		containers::tab_pane_specs& containers::tab_pane_specs::caption_orientation(const orientation& caption_orientation) {
			caption_orientation_ = caption_orientation;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_tabs() { return color_tabs_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_tabs(const color& color_tabs) {
			color_tabs_ = color_tabs;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_tabs_border() { return color_tabs_border_; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_tabs_border(const color& color_tabs_border) {
			color_tabs_border_ = color_tabs_border;
			return *this;
		}

		float& containers::tab_pane_specs::tabs_border() { return tabs_border_; }

		containers::tab_pane_specs& containers::tab_pane_specs::tabs_border(const float& tabs_border) {
			tabs_border_ = tabs_border;
			return *this;
		}
	}
}
