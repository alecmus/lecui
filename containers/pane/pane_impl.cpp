//
// pane_impl.cpp - pane_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "pane_impl.h"
#include "../../containers/page/page_impl.h"

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
			const std::string& alias) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_disabled(nullptr),
			_margin(12.f),
			_rect_client_area({ 0.f, 0.f, 0.f, 0.f }),
			_rect_pane({ 0.f, 0.f, 0.f, 0.f }) {}

		widgets::pane_impl::~pane_impl() { discard_resources(); }

		widgets::widget_type
			widgets::pane_impl::type() {
			return lecui::widgets::widget_type::pane;
		}

		HRESULT widgets::pane_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
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

			_resources_created = true;
			return hr;
		}

		void widgets::pane_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_disabled);
		}

		D2D1_RECT_F&
			widgets::pane_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_initial_capture_done) {
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

			return _rect_pane;
		}

		containers::pane_specs&
			widgets::pane_impl::specs() { return _specs; }

		containers::pane_specs&
			widgets::pane_impl::operator()() { return specs(); }

		const D2D1_RECT_F& widgets::pane_impl::client_area() {
			return _rect_client_area;
		}

		const D2D1_RECT_F& widgets::pane_impl::pane_area() {
			return _rect_pane;
		}

		bool widgets::pane_impl::contains() { return false; }
	}
}
