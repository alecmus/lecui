//
// custom_impl.cpp - custom_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "custom_impl.h"

namespace liblec {
	namespace lecui {
		widgets::custom_impl::custom_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory, IWICImagingFactory* p_iwic_factory) :
			widget_impl(page, alias),
			_p_directwrite_factory(p_directwrite_factory),
			_p_iwic_factory(p_iwic_factory) {}

		widgets::custom_impl::~custom_impl() {
			// DO NOT call discard_resources() here. Let the client do that
			// in their own destructor (which will be called far before this one).
			// Trying to discard resources here will result in an access violation
			// if the custom widget is not a static object, and we certainly do
			// not want any widget to be a static object!
		}

		widgets::widget_type
			widgets::custom_impl::type() {
			return lecui::widgets::widget_type::custom;
		}

		HRESULT widgets::custom_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			if (_specs.on_create_resources != nullptr)
				_specs.on_create_resources(p_render_target, _p_directwrite_factory, _p_iwic_factory);

			_resources_created = true;
			return S_OK;
		}

		void widgets::custom_impl::discard_resources() {
			_resources_created = false;
			if (_specs.on_discard_resources != nullptr)
				_specs.on_discard_resources();
		}

		D2D1_RECT_F& widgets::custom_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (render && (_specs.on_render != nullptr))
				_specs.on_render(&_rect, _is_enabled, _hit, _pressed, _selected);

			return _rect;
		}

		widgets::custom_specs&
			widgets::custom_impl::specs() {
			return _specs;
		}

		widgets::custom_specs&
			widgets::custom_impl::operator()() {
			return specs();
		}
	}
}
