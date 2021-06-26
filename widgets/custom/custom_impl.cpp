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
			p_directwrite_factory_(p_directwrite_factory),
			p_iwic_factory_(p_iwic_factory) {}

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
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor());

			if (specs_.on_create_resources != nullptr)
				specs_.on_create_resources(p_render_target, p_directwrite_factory_, p_iwic_factory_);

			resources_created_ = true;
			return S_OK;
		}

		void widgets::custom_impl::discard_resources() {
			resources_created_ = false;
			if (specs_.on_discard_resources != nullptr)
				specs_.on_discard_resources();
		}

		D2D1_RECT_F& widgets::custom_impl::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect(), specs_.on_resize(), change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (render && (specs_.on_render != nullptr))
				specs_.on_render(&rect_, is_enabled_, hit_, pressed_, selected_);

			return rect_;
		}

		widgets::custom::custom_specs&
			widgets::custom_impl::specs() {
			return specs_;
		}

		widgets::custom::custom_specs&
			widgets::custom_impl::operator()() {
			return specs();
		}
	}
}
