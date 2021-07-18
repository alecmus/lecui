//
// status_pane.cpp - status_pane container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../status_pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::status_pane::impl {
		public:
			impl(form& fm, status_pane_specs::location location) :
				_fm(fm),
				_location(location) {
				switch (location) {
				case containers::status_pane_specs::location::top: {
					_alias = "status::top";
				} break;
				case containers::status_pane_specs::location::left: {
					_alias = "status::left";
				} break;
				case containers::status_pane_specs::location::right: {
					_alias = "status::right";
				} break;

				case containers::status_pane_specs::location::bottom:
				default: {
					_alias = "status::bottom";
				} break;
				}

				if (_fm._d._p_status_pane_specs.find(_alias) == _fm._d._p_status_pane_specs.end()) {
					status_pane_specs specs;
					_fm._d._p_status_pane_specs.insert(std::make_pair(_alias, specs));
				}
			}

			form& _fm;
			std::string _alias;
			status_pane_specs::location _location;
		};

		containers::status_pane::status_pane(form& fm, status_pane_specs::location location) :
			_d(*new impl(fm, location)) {}
		containers::status_pane::~status_pane() {
			delete& _d;
		}

		containers::status_pane_specs& containers::status_pane::specs() {
			return _d._fm._d._p_status_pane_specs.at(_d._alias);
		}

		containers::status_pane_specs& containers::status_pane::operator()() {
			return specs();
		}

		containers::status_pane_specs&
			containers::status_pane::specs(form& fm, const std::string& alias) {
			return fm._d._p_status_pane_specs.at(alias);
		}

		containers::page& containers::status_pane::get() {
			if (_d._fm._d._p_status_panes.count(_d._alias)) {
				log("library usage error page::add_status_bar");

				/// to-do: find a mechanism that makes sense ...
				return _d._fm._d._p_status_panes.at(_d._alias);
			}

			_d._fm._d._p_status_panes.try_emplace(_d._alias, _d._fm, _d._alias);
			auto& page_impl = _d._fm._d._p_status_panes.at(_d._alias)._d_page;

			lecui::rect rect_client;
			rect_client.size(_d._fm._d._size);
			rect_client.width(rect_client.width() - (2.f * _d._fm._d._content_margin));
			rect_client.height(rect_client.height() - (2.f * _d._fm._d._content_margin + _d._fm._d._caption_bar_height));

			lecui::rect rect = rect_client;
			lecui::widgets::specs::resize_params on_resize;

			auto& specs = this->specs();

			switch (_d._location) {

			case containers::status_pane_specs::location::top: {
				rect.height(specs.thickness());
				rect.place(rect_client, 50.f, 0.f);
				on_resize = { 0, 0, 100, 0 };
			} break;
			case containers::status_pane_specs::location::left: {
				rect.width(specs.thickness());
				rect.place(rect_client, 0.f, 50.f);
				on_resize = { 0, 0, 0, 100 };
			} break;
			case containers::status_pane_specs::location::right: {
				rect.width(specs.thickness());
				rect.place(rect_client, 100.f, 50.f);
				on_resize = { 100, 0, 0, 100 };
			} break;

			case containers::status_pane_specs::location::bottom:
			default: {
				rect.height(specs.thickness());
				rect.place(rect_client, 50.f, 100.f);
				on_resize = { 0, 100, 100, 0 };
			} break;
			}

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(_d._fm._d._p_direct2d_factory);

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(_d._fm._d._p_directwrite_factory);

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(_d._fm._d._p_iwic_factory);

			// set page size
			page_impl.size(rect.size());
			page_impl.width(page_impl.width() - (2.f * _d._fm._d._content_margin));
			page_impl.height(page_impl.height() - (2.f * _d._fm._d._content_margin));

			// add an invisible rect to bound the page. This is essential for scroll bars to work
			// appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
			rectangle.color_fill().alpha(0);

			// make it transparent
			rectangle.color_border({ 255, 0, 0, 0 }).color_hot({ 255, 0, 0, 0 })

				// set its dimensions to exactly match the page
				.corner_radius_x(15.f).corner_radius_y(15.f)
				.on_resize(on_resize)
				.rect().size(page_impl.size());

			// return reference to page so caller can add widgets to it
			return _d._fm._d._p_status_panes.at(_d._alias);
		}

		containers::page& containers::status_pane::get(form& fm, const std::string& alias) {
			return fm._d._p_status_panes.at(alias);
		}
	}
}
