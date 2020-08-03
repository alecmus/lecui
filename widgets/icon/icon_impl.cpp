/*
** icon_impl.cpp - icon_impl implementation
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
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr) {}

		widgets::icon_impl::~icon_impl() { discard_resources(); }

		widgets::widget_type
			widgets::icon_impl::type() {
			return lecui::widgets::widget_type::icon;
		}

		HRESULT widgets::icon_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);

			resources_created_ = true;
			return hr;
		}

		void widgets::icon_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
		}

		D2D1_RECT_F&
			widgets::icon_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;

				try {
					if (rectangle_specs_.has_value()) {
						// update rectangle specs
						rectangle_specs_.value().get().corner_radius_x = specs_.corner_radius_x;
						rectangle_specs_.value().get().corner_radius_y = specs_.corner_radius_y;
						rectangle_specs_.value().get().border = specs_.border;
						rectangle_specs_.value().get().color_fill = specs_.color_fill;
						rectangle_specs_.value().get().color_border = specs_.color_border;
						rectangle_specs_.value().get().color_hot = specs_.color_hot;
					}

					if (image_view_specs_.has_value()) {
						// update image view specs
						image_view_specs_.value().get().png_resource = specs_.png_resource;
						image_view_specs_.value().get().file = specs_.file;
					}

					if (label_specs_.has_value()) {
						// update text specs
						label_specs_.value().get().text = specs_.text;
					}

					if (description_specs_.has_value()) {
						// update description specs
						description_specs_.value().get().text = specs_.description;
					}

					// schedule a refresh
					page_.d_page_.get_form().d_.schedule_refresh_ = true;
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			return rect_;
		}

		widgets::icon::icon_specs&
			widgets::icon_impl::specs() { return specs_; }

		widgets::icon::icon_specs&
			widgets::icon_impl::operator()() { return specs(); }

		void widgets::icon_impl::set_icon_specs(rectangle::rectangle_specs& rectangle,
			image_view::image_view_specs& image_view,
			label::label_specs& label, label::label_specs& description) {
			rectangle_specs_ = rectangle;
			image_view_specs_ = image_view;
			label_specs_ = label;
			description_specs_ = description;
		}
	}
}
