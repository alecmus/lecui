//
// status_pane.cpp - status_pane container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../status_pane.h"
#include "../side_pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		namespace containers {
			class status_pane_builder {
			public:
				status_pane_builder(form& fm, status_pane_specs specs);
				~status_pane_builder();

				/// <summary>Get the status pane container page.</summary>
				/// <returns>A reference to the status pane container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The status pane, however,
				/// does not have scroll bars like other containers. If you really need a status
				/// pane to have scroll bars (though it's unimaginable why one would need to do
				/// this) add a single pane within it that fills it up entirely.</remarks>
				[[nodiscard]]
				containers::status_pane& get();

				/// <summary>Get the status pane container page of an existing status pane.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="alias">The status pane alias, e.g. "status::right".</param>
				/// <returns>A reference to the status pane container page.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_status_pane_page).</remarks>
				[[nodiscard]]
				static containers::status_pane& get(form& fm, const std::string& alias);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				status_pane_builder() = delete;
				status_pane_builder(const status_pane_builder&) = delete;
				status_pane_builder& operator=(const status_pane_builder&) = delete;
			};

			const std::string status_pane_alias(status_pane_specs::pane_location location) {
				std::string _alias;

				switch (location) {
				case containers::status_pane_specs::pane_location::top: {
					_alias = "status::top";
				} break;
				case containers::status_pane_specs::pane_location::left: {
					_alias = "status::left";
				} break;
				case containers::status_pane_specs::pane_location::right: {
					_alias = "status::right";
				} break;

				case containers::status_pane_specs::pane_location::bottom:
				default: {
					_alias = "status::bottom";
				} break;
				}

				return _alias;
			}

			float& status_pane_specs::thickness() { return _thickness; }

			status_pane_specs& status_pane_specs::thickness(const float& thickness) {
				_thickness = thickness;
				return *this;
			}

			bool& status_pane_specs::floating() { return _floating; }

			status_pane_specs& status_pane_specs::floating(const bool& floating) {
				_floating = floating;
				return *this;
			}

			status_pane_specs::pane_location& status_pane_specs::location() { return _location; }

			status_pane_specs& status_pane_specs::location(const status_pane_specs::pane_location location) {
				_location = location;
				return *this;
			}
			status_pane_base::status_pane_base(form& fm, const std::string& alias) :
				page(fm, alias) {}
			status_pane_base::~status_pane_base() {}
		}

		class containers::status_pane_builder::impl {
		public:
			impl(form& fm, status_pane_specs specs) :
				_fm(fm) {
				_alias = status_pane_alias(specs.location());

				if (_fm._d._p_status_pane_specs.find(_alias) == _fm._d._p_status_pane_specs.end())
					_fm._d._p_status_pane_specs.insert(std::make_pair(_alias, specs));
			}

			form& _fm;
			std::string _alias;
			bool _floating;
		};

		containers::status_pane_builder::status_pane_builder(form& fm, status_pane_specs specs) :
			_d(*new impl(fm, specs)) {}
		containers::status_pane_builder::~status_pane_builder() {
			delete& _d;
		}

		containers::status_pane& containers::status_pane_builder::get() {
			if (_d._fm._d._p_status_panes.count(_d._alias)) {
				log("library usage error page::add_status_bar");

				/// to-do: find a mechanism that makes sense ...
				return _d._fm._d._p_status_panes.at(_d._alias);
			}

			auto& specs = _d._fm._d._p_status_pane_specs.at(_d._alias);

			if (specs._side_pane) {
				_d._fm._d._side_pane_present = true;
				_d._fm._d._side_pane_thickness = specs.thickness();
			}

			_d._fm._d._p_status_panes.try_emplace(_d._alias, _d._fm, specs);
			auto& page_impl = _d._fm._d._p_status_panes.at(_d._alias)._d_page;

			lecui::rect rect_client;
			rect_client.size(_d._fm._d._size);
			rect_client.width(rect_client.width() - (2.f * _d._fm._d._content_margin));
			rect_client.height(rect_client.height() - (2.f * _d._fm._d._content_margin + (specs._side_pane ? 0.f : _d._fm._d._caption_bar_height)));

			if (_d._fm._d._side_pane_present)
				rect_client.left(_d._fm._d._side_pane_thickness);

			lecui::rect rect = rect_client;
			lecui::resize_params on_resize;

			switch (specs.location()) {

			case containers::status_pane_specs::pane_location::top: {
				rect.height(specs.thickness());
				rect.place(rect_client, 50.f, 0.f);
				on_resize = resize_params()
					.width_rate(100.f);
			} break;
			case containers::status_pane_specs::pane_location::left: {
				rect.width(specs.thickness());
				rect.place(rect_client, 0.f, 50.f);
				on_resize = resize_params()
					.height_rate(100.f);
			} break;
			case containers::status_pane_specs::pane_location::right: {
				rect.width(specs.thickness());
				rect.place(rect_client, 100.f, 50.f);
				on_resize = resize_params()
					.height_rate(100.f);
			} break;

			case containers::status_pane_specs::pane_location::bottom:
			default: {
				rect.height(specs.thickness());
				on_resize = resize_params()
					.width_rate(100.f);
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
				.corner_radius_x(0.f).corner_radius_y(0.f)
				.on_resize(on_resize)
				.rect().size(page_impl.size());

			// return reference to page so caller can add widgets to it
			return _d._fm._d._p_status_panes.at(_d._alias);
		}

		containers::status_pane& containers::status_pane_builder::get(form& fm, const std::string& alias) {
			return fm._d._p_status_panes.at(alias);
		}

		// status pane

		containers::status_pane::status_pane(form& fm, status_pane_specs specs) :
			status_pane_base(fm, status_pane_alias(specs.location())) {}

		containers::status_pane& containers::status_pane::add(form& fm, status_pane_specs specs) {
			return containers::status_pane_builder(fm, specs).get();
		}

		containers::status_pane& containers::status_pane::get(form& fm, const std::string& alias) {
			return containers::status_pane_builder::get(fm, alias);
		}

		// side pane

		containers::side_pane::side_pane(form& fm, const float& thickness) :
			status_pane_base(fm, status_pane_alias(status_pane_specs::pane_location::left)) {}

		containers::status_pane& containers::side_pane::add(form& fm, const float& thickness) {
			status_pane_specs specs;
			specs
				.floating(false)
				.location(containers::status_pane_specs::pane_location::left)
				.thickness(thickness);

			// the magic line
			specs._side_pane = true;

			return containers::status_pane_builder(fm, specs).get();
		}

		containers::status_pane& containers::side_pane::get(form& fm) {
			return containers::status_pane_builder::get(fm, status_pane_alias(status_pane_specs::pane_location::left));
		}
	}
}
