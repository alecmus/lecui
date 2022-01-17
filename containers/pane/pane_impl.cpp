//
// pane_impl.cpp - pane_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "pane_impl.h"
#include "../../containers/page/page_impl.h"
#include "../../appearance.h"

namespace liblec {
	namespace lecui {
		std::string widgets::pane_impl::tree_pane_alias_prefix() {
			return std::string("lecui::containers::tree_pane::");
		}

		std::string widgets::pane_impl::html_pane_alias_prefix() {
			return std::string("lecui::containers::html_pane::");
		}

		std::string widgets::pane_impl::html_controls_pane_alias_prefix() {
			return std::string("lecui::containers::html_controls_pane::");
		}

		std::string widgets::pane_impl::time_pane_alias_prefix() {
			return std::string("lecui::containers::time_pane::");
		}

		std::string widgets::pane_impl::date_pane_alias_prefix() {
			return std::string("lecui::containers::date_pane::");
		}

		std::string widgets::pane_impl::icon_pane_alias_prefix() {
			return std::string("lecui::containers::icon_pane::");
		}

		std::string widgets::pane_impl::table_pane_alias_prefix() {
			return std::string("lecui::containers::table_pane::");
		}

		widgets::pane_impl::pane_impl(containers::page& page,
			const std::string& alias, const float& content_margin,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_disabled(nullptr),
			_margin(12.f),
			_content_margin(content_margin),
			_rect_client_area({ 0.f, 0.f, 0.f, 0.f }),
			_rect_pane({ 0.f, 0.f, 0.f, 0.f }),
			_p_directwrite_factory(p_directwrite_factory) {
			try {
				const std::string pane_name = "pane";
				_p_panes.try_emplace(pane_name, page._d_page.get_form(), pane_name);
				_current_pane = pane_name;
				auto& page_impl = _p_panes.at(pane_name)._d_page;

				// specify direct2d factory (used internally for geometries and stuff)
				page_impl.direct2d_factory(page._d_page.direct2d_factory());

				// specify directwrite factory (used internally for text rendering)
				page_impl.directwrite_factory(page._d_page.directwrite_factory());

				// specify iwic imaging factory (used internally for image rendering)
				page_impl.iwic_factory(page._d_page.iwic_factory());

				// set pane content margin
				page_impl._content_margin = _content_margin;

				// specify parent
				page_impl.parent(page);

				const float thickness = 10.f;	// thickness of scroll bars
				rect rect_client_area = { 0.f, 100.f, 0.f, 100.f };	// to-do: find a solution to this (coz at this point the user hasn't specified the size)

				// set page size
				page_impl.size({ rect_client_area.width(), rect_client_area.height() });
				page_impl.width(page_impl.width() - (2.f * _content_margin));
				page_impl.height(page_impl.height() - (2.f * _content_margin));

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

				rectangle.on_resize()
					.width_rate(100.f).height_rate(100.f);

				// capture pointer to pane rect and page
				_p_page_impl = &page_impl;
				_p_pane_rect = &rectangle.rect();

				// define reference rect for scroll bars
				lecui::rect ref_rect = rectangle.rect();
				ref_rect.left() -= _content_margin;
				ref_rect.right() += _content_margin;
				ref_rect.top() -= _content_margin;
				ref_rect.bottom() += _content_margin;

				appearance _apprnc(page._d_page.get_form());

				// initialize the page's horizontal scroll bar
				{
					auto& _specs = page_impl.h_scrollbar().specs();
					_specs.on_resize()
						.width_rate(100.f)
						.y_rate(100.f);

					auto width = rect_client_area.right() - rect_client_area.left() - 2.f * _content_margin;

					if (thickness > _content_margin)
						width -= (thickness - _content_margin);

					_specs.rect()
						.width(width)
						.height(thickness)
						.place(ref_rect, 50.f, 100.f);

					_specs
						.color_fill(defaults::color(_apprnc.theme(), item::scrollbar))
						.color_scrollbar_border(defaults::color(_apprnc.theme(), item::scrollbar_border))
						.color_hot(defaults::color(_apprnc.theme(), item::scrollbar_hover))
						.color_hot_pressed(defaults::color(_apprnc.theme(), item::scrollbar_pressed));
				}

				// initialize the page's vertical scroll bar
				{
					auto& _specs = page_impl.v_scrollbar().specs();
					_specs.on_resize()
						.height_rate(100.f)
						.x_rate(100.f);

					auto height = rect_client_area.bottom() - rect_client_area.top() - 2.f * _content_margin;

					if (thickness > _content_margin)
						height -= (thickness - _content_margin);

					_specs.rect()
						.width(thickness)
						.height(height)
						.place(ref_rect, 100.f, 50.f);

					_specs
						.color_fill(defaults::color(_apprnc.theme(), item::scrollbar))
						.color_scrollbar_border(defaults::color(_apprnc.theme(), item::scrollbar_border))
						.color_hot(defaults::color(_apprnc.theme(), item::scrollbar_hover))
						.color_hot_pressed(defaults::color(_apprnc.theme(), item::scrollbar_pressed));
				}
			}
			catch (const std::exception& e) {
				log(e.what());	// this should never happen, but added as a failsafe anyway
			}
		}

		widgets::pane_impl::~pane_impl() { discard_resources(); }

		widgets::widget_type
			widgets::pane_impl::type() {
			return lecui::widgets::widget_type::pane;
		}

		HRESULT widgets::pane_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			auto& _specs = _p_panes.at(_current_pane);

			if (!_size_initialized) {
				if (_p_page_impl) {
					lecui::size s;
					s.width(_specs.rect().width() - 2.f * _content_margin);
					s.height(_specs.rect().height() - 2.f * _content_margin);

					// pane rect changed
					const auto& width_change = - (_p_page_impl->width() - s.width());
					const auto& height_change = -(_p_page_impl->height() - s.height());

					// adjust pane rect accordingly

					// page dimensions
					_p_page_impl->width(_p_page_impl->width() + width_change);
					_p_page_impl->height(_p_page_impl->height() + height_change);

					// pane dimensions
					_p_pane_rect->width(_p_pane_rect->width() + width_change);
					_p_pane_rect->height(_p_pane_rect->height() + height_change);

					// scroll bars

					// horizontal scrollbar
					_p_page_impl->h_scrollbar().specs().rect().right(_p_page_impl->h_scrollbar().specs().rect().right() + width_change);

					_p_page_impl->h_scrollbar().specs().rect().top(_p_page_impl->h_scrollbar().specs().rect().top() + height_change);
					_p_page_impl->h_scrollbar().specs().rect().bottom(_p_page_impl->h_scrollbar().specs().rect().bottom() + height_change);

					// vertical scrollbar
					_p_page_impl->v_scrollbar().specs().rect().bottom(_p_page_impl->v_scrollbar().specs().rect().bottom() + height_change);

					_p_page_impl->v_scrollbar().specs().rect().left(_p_page_impl->v_scrollbar().specs().rect().left() + width_change);
					_p_page_impl->v_scrollbar().specs().rect().right(_p_page_impl->v_scrollbar().specs().rect().right() + width_change);
				}

				_size_initialized = true;
			}

			_specs_old = _specs;
			_is_static = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);

			// create badge resources
			create_badge_resources(_specs.badge(), p_render_target, _p_directwrite_factory, _badge_resources);

			_resources_created = true;
			return hr;
		}

		void widgets::pane_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_disabled);

			// discard badge resources
			discard_badge_resources(_badge_resources);
		}

		D2D1_RECT_F&
			widgets::pane_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			auto& _specs = _p_panes.at(_current_pane);
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_initial_capture_done) {
				_rect_original = _specs.rect();
				_rect_previous = _specs.rect();
				_initial_capture_done = true;
			}

			if (_specs.rect() != _rect_previous && _p_page_impl && _p_pane_rect) {
				log("rect changed: " + _alias);

				// pane rect changed
				const auto& width_change = _specs.rect().width() - _rect_previous.width();
				const auto& height_change = _specs.rect().height() - _rect_previous.height();

				_rect_previous = _specs.rect();

				// adjust pane rect accordingly

				// page dimensions
				_p_page_impl->width(_p_page_impl->width() + width_change);
				_p_page_impl->height(_p_page_impl->height() + height_change);

				// pane dimensions
				_p_pane_rect->width(_p_pane_rect->width() + width_change);
				_p_pane_rect->height(_p_pane_rect->height() + height_change);

				// scroll bars

				// horizontal scrollbar
				_p_page_impl->h_scrollbar().specs().rect().right(_p_page_impl->h_scrollbar().specs().rect().right() + width_change);

				_p_page_impl->h_scrollbar().specs().rect().top(_p_page_impl->h_scrollbar().specs().rect().top() + height_change);
				_p_page_impl->h_scrollbar().specs().rect().bottom(_p_page_impl->h_scrollbar().specs().rect().bottom() + height_change);

				// vertical scrollbar
				_p_page_impl->v_scrollbar().specs().rect().bottom(_p_page_impl->v_scrollbar().specs().rect().bottom() + height_change);

				_p_page_impl->v_scrollbar().specs().rect().left(_p_page_impl->v_scrollbar().specs().rect().left() + width_change);
				_p_page_impl->v_scrollbar().specs().rect().right(_p_page_impl->v_scrollbar().specs().rect().right() + width_change);
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect_pane = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect_pane.left -= offset.x;
			_rect_pane.right -= offset.x;
			_rect_pane.top -= offset.y;
			_rect_pane.bottom -= offset.y;

			// no reponse to hit testing, even though for scroll bar at form level
			// we need to return the entire region through _rect_pane
			_rect = { 0.f, 0.f, 0.f, 0.f };

			_rect_client_area = _rect_pane;

			if (!render || !_visible)
				return _rect_pane;

			D2D1_ROUNDED_RECT rounded_rect{ _rect_client_area,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect,
				_is_enabled ? _p_brush_fill : _p_brush_disabled);
			p_render_target->DrawRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_border : _p_brush_disabled, _specs.border());

			// draw the badge
			draw_badge(_specs.badge(), _rect_pane, p_render_target, _p_directwrite_factory, _badge_resources);

			return _rect_pane;
		}

		containers::pane_specs&
			widgets::pane_impl::specs() { return _p_panes.at(_current_pane);; }

		containers::pane_specs&
			widgets::pane_impl::operator()() { return specs(); }

		const D2D1_RECT_F& widgets::pane_impl::client_area() { return _rect_client_area; }

		const D2D1_RECT_F& widgets::pane_impl::pane_area() { return _rect_pane; }

		const float& widgets::pane_impl::content_margin() { return _content_margin; }

		const D2D1_SIZE_F widgets::pane_impl::programmatic_change_in_size() {
			auto& _specs = _p_panes.at(_current_pane);
			const auto& width_change = _specs.rect().width() - _rect_original.width();
			const auto& height_change = _specs.rect().height() - _rect_original.height();
			return { width_change, height_change };
		}

		bool widgets::pane_impl::contains() { return false; }
	}
}
