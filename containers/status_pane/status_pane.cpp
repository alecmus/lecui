/*
** status_pane.cpp - status_pane container implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "..\status_pane.h"
#include "..\..\form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::status_pane::impl {
		public:
			impl(form& fm, status_pane::location location) :
				fm_(fm),
				location_(location) {
				switch (location) {
				case containers::status_pane::location::top: {
					alias_ = "status::top";
				} break;
				case containers::status_pane::location::left: {
					alias_ = "status::left";
				} break;
				case containers::status_pane::location::right: {
					alias_ = "status::right";
				} break;

				case containers::status_pane::location::bottom:
				default: {
					alias_ = "status::bottom";
				} break;
				}

				if (fm_.d_.p_status_pane_specs_.find(alias_) == fm_.d_.p_status_pane_specs_.end()) {
					status_pane_specs specs;
					fm_.d_.p_status_pane_specs_.insert(std::make_pair(alias_, specs));
				}
			}

			form& fm_;
			std::string alias_;
			status_pane::location location_;
		};

		containers::status_pane::status_pane(form& fm, status_pane::location location) :
			d_(*new impl(fm, location)) {}
		containers::status_pane::~status_pane() {
			delete& d_;
		}

		containers::status_pane::status_pane_specs& containers::status_pane::specs() {
			return d_.fm_.d_.p_status_pane_specs_.at(d_.alias_);
		}

		containers::status_pane::status_pane_specs& containers::status_pane::operator()() {
			return specs();
		}

		containers::status_pane::status_pane_specs&
			containers::status_pane::specs(form& fm, const std::string& alias) {
			return fm.d_.p_status_pane_specs_.at(alias);
		}

		containers::page& containers::status_pane::get() {
			if (d_.fm_.d_.p_status_panes_.count(d_.alias_)) {
				log("library usage error page::add_status_bar");

				/// to-do: find a mechanism that makes sense ...
				return d_.fm_.d_.p_status_panes_.at(d_.alias_);
			}

			d_.fm_.d_.p_status_panes_.try_emplace(d_.alias_, d_.fm_, d_.alias_);
			auto& page_impl = d_.fm_.d_.p_status_panes_.at(d_.alias_).d_page_;

			lecui::rect rect_client;
			rect_client.size(d_.fm_.d_.size_);
			rect_client.width(rect_client.width() - (2.f * d_.fm_.d_.page_tolerance_));
			rect_client.height(rect_client.height() - (2.f * d_.fm_.d_.page_tolerance_ + d_.fm_.d_.caption_bar_height_));

			lecui::rect rect = rect_client;
			lecui::widgets::specs::resize_params on_resize;

			auto& specs = this->specs();

			switch (d_.location_) {

			case containers::status_pane::location::top: {
				rect.height(specs.thickness);
				rect.place(rect_client, 50.f, 0.f);
				on_resize = { 0, 0, 100, 0 };
			} break;
			case containers::status_pane::location::left: {
				rect.width(specs.thickness);
				rect.place(rect_client, 0.f, 50.f);
				on_resize = { 0, 0, 0, 100 };
			} break;
			case containers::status_pane::location::right: {
				rect.width(specs.thickness);
				rect.place(rect_client, 100.f, 50.f);
				on_resize = { 100, 0, 0, 100 };
			} break;

			case containers::status_pane::location::bottom:
			default: {
				rect.height(specs.thickness);
				rect.place(rect_client, 50.f, 100.f);
				on_resize = { 0, 100, 100, 0 };
			} break;
			}

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(d_.fm_.d_.p_direct2d_factory_);

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(d_.fm_.d_.p_directwrite_factory_);

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(d_.fm_.d_.p_iwic_factory_);

			// set page size
			page_impl.size(rect.size());
			page_impl.width(page_impl.width() - (2.f * d_.fm_.d_.page_tolerance_));
			page_impl.height(page_impl.height() - (2.f * d_.fm_.d_.page_tolerance_));

			// add an invisible rect to bound the page. This is essential for scroll bars to work
			// appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets_impl::rectangle::page_rect_alias());
			rectangle.color_fill.alpha = 0;

			// make it transparent
			rectangle.color_border = { 255, 0, 0, 0 };
			rectangle.color_hot = { 255, 0, 0, 0 };

			// set its dimensions to exactly match the page
			rectangle.rect.size(page_impl.size());
			rectangle.corner_radius_x = 15.f;
			rectangle.corner_radius_y = 15.f;
			rectangle.on_resize = on_resize;

			// return reference to page so caller can add widgets to it
			return d_.fm_.d_.p_status_panes_.at(d_.alias_);
		}

		containers::page& containers::status_pane::get(form& fm, const std::string& alias) {
			return fm.d_.p_status_panes_.at(alias);
		}
	}
}
