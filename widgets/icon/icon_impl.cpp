//
// icon_impl.cpp - icon_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "icon_impl.h"
#include "../../form_impl/form_impl.h"
#include "../../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		std::string widgets::icon_impl::alias_icon() {
			return std::string("lecui::widgets::icon::icon");
		}

		std::string widgets::icon_impl::alias_image() {
			return std::string("lecui::widgets::icon::image");
		}

		std::string widgets::icon_impl::alias_text() {
			return std::string("lecui::widgets::icon::text");
		}

		std::string widgets::icon_impl::alias_description() {
			return std::string("lecui::widgets::icon::description");
		}

		widgets::icon_impl::icon_impl(containers::page& page,
			const std::string& alias) :
			widget_impl(page, alias),
			_p_brush_fill(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr) {}

		widgets::icon_impl::~icon_impl() { discard_resources(); }

		widgets::widget_type
			widgets::icon_impl::type() {
			return lecui::widgets::widget_type::icon;
		}

		HRESULT widgets::icon_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);

			_resources_created = true;
			return hr;
		}

		void widgets::icon_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
		}

		D2D1_RECT_F&
			widgets::icon_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;

				try {
					if (_rectangle_specs.has_value()) {
						// update rectangle widget
						_rectangle_specs.value().get()
							.corner_radius_x(_specs.corner_radius_x())
							.corner_radius_y(_specs.corner_radius_y())
							.border(_specs.border())
							.color_fill(_specs.color_fill())
							.color_border(_specs.color_border())
							.color_hot(_specs.color_hot());
					}

					if (_image_view_specs.has_value()) {
						// update image view widget
						_image_view_specs.value().get()
							.png_resource(_specs.png_resource())
							.file(_specs.file());
					}

					if (_label_specs.has_value()) {
						// update text widget
						_label_specs.value().get()
							.text(_specs.text())
							.color_text(_specs.color_text())
							.font(_specs.font())
							.font_size(_specs.font_size());
					}

					if (_description_specs.has_value()) {
						// update description widget
						_description_specs.value().get()
							.text(_specs.description())
							.color_text(_specs.color_text_description())
							.font(_specs.font())
							.font_size(_specs.font_size_description());
					}

					// schedule a refresh
					_page._d_page.get_form()._d._schedule_refresh = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// use _specs._rect not _specs.rect() and _specs._on_resize not _specs.on_resize() due to redirection to special pane
			_rect = position(_specs._rect, _specs._on_resize, change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			return _rect;
		}

		widgets::icon&
			widgets::icon_impl::specs() { return _specs; }

		widgets::icon&
			widgets::icon_impl::operator()() { return specs(); }

		void widgets::icon_impl::set_icon_specs(rectangle& rectangle,
			image_view& image_view,
			label& text, label& description) {
			_rectangle_specs = rectangle;
			_image_view_specs = image_view;
			_label_specs = text;
			_description_specs = description;
		}
	}
}
