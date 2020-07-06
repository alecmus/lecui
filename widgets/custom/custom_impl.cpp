/*
** custom_impl.cpp - custom_impl implementation
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

#include "custom_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::custom::custom(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory, IWICImagingFactory* p_iwic_factory) :
			widget(page, alias),
			p_directwrite_factory_(p_directwrite_factory),
			p_iwic_factory_(p_iwic_factory) {}

		widgets_impl::custom::~custom() {
			// DO NOT call discard_resources() here. Let the client do that
			// in their own destructor (which will be called far before this one).
			// Trying to discard resources here will result in an access violation
			// if the custom widget is not a static object, and we certainly do
			// not want any widget to be a static object!
		}

		widgets_impl::widget_type
			widgets_impl::custom::type() {
			return lecui::widgets_impl::widget_type::custom;
		}

		HRESULT widgets_impl::custom::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			if (specs_.on_create_resources != nullptr)
				specs_.on_create_resources(p_render_target, p_directwrite_factory_, p_iwic_factory_);

			resources_created_ = true;
			return S_OK;
		}

		void widgets_impl::custom::discard_resources() {
			resources_created_ = false;
			if (specs_.on_discard_resources != nullptr)
				specs_.on_discard_resources();
		}

		D2D1_RECT_F& widgets_impl::custom::render(ID2D1HwndRenderTarget* p_render_target,
			const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (render && (specs_.on_render != nullptr))
				specs_.on_render(&rect_, is_enabled_, hit_, pressed_, selected_);

			return rect_;
		}

		void widgets_impl::custom::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::custom::custom_specs&
			widgets_impl::custom::specs() {
			return specs_;
		}

		widgets::custom::custom_specs&
			widgets_impl::custom::operator()() {
			return specs();
		}
	}
}
