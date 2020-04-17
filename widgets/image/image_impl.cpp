/*
** image_impl.cpp - image_impl implementation
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

#include "image_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::image::image(const std::string& page_alias,
			const std::string& alias, IWICImagingFactory* p_IWICFactory) :
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_bitmap_(nullptr),
			p_IWICFactory_(p_IWICFactory) {
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::image::~image() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::image::type() {
			return lecui::widgets_impl::widget_type::image;
		}

		HRESULT widgets_impl::image::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr);
			h_cursor_ = get_cursor(specs_.cursor);

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);
			if (SUCCEEDED(hr) && !specs_.file.empty())
				load_bitmap_file(p_render_target, p_IWICFactory_, convert_string(specs_.file).c_str(),
					&p_bitmap_);

			resources_created_ = true;
			return hr;
		}

		void widgets_impl::image::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_bitmap_);
		}

		D2D1_RECT_F&
			widgets_impl::image::render(ID2D1HwndRenderTarget* p_render_target,
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

			if (!render || !visible_)
				return rect_;

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x, specs_.corner_radius_y };

			p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_fill_);

			if (p_bitmap_) {
				// retrieve the size of the bitmap and define suitably sized rectangle for the image
				const auto size = p_bitmap_->GetSize();
				auto rect_image = D2D1::RectF(0, 0, size.width, size.height);
				fit_rect(rect_, rect_image, false, false, true);

				// draw the bitmap
				p_render_target->DrawBitmap(p_bitmap_, rect_image);
			}

			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_border_, specs_.border);

			if (!is_static_ && is_enabled_) {
				if (hit_ || pressed_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ?
						1.75f : 1.f);

				if (selected_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ?
						1.75f : 1.f);
			}

			return rect_;
		}

		void widgets_impl::image::on_click() {
			if (specs_.events().click)
				specs_.events().click();
		}

		widgets::image::image_specs&
			widgets_impl::image::specs() { return specs_; }

		widgets::image::image_specs&
			widgets_impl::image::operator()() { return specs(); }
	}
}
