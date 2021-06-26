//
// image_view_impl.cpp - image_view_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "image_view_impl.h"
#include "../../containers/page/page_impl.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		widgets::image_view_impl::image_view_impl(containers::page& page,
			const std::string& alias, IWICImagingFactory* p_IWICFactory) :
			widget_impl(page, alias),
			p_brush_fill_(nullptr),
			p_brush_border_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_bitmap_(nullptr),
			p_IWICFactory_(p_IWICFactory),
			old_size_({ 0.f, 0.f })	{}

		widgets::image_view_impl::~image_view_impl() { discard_resources(); }

		widgets::widget_type
			widgets::image_view_impl::type() {
			return lecui::widgets::widget_type::image_view;
		}

		HRESULT widgets::image_view_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = (specs_.events().click == nullptr && specs_.events().action == nullptr);
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border()),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot()),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled()),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected()),
					&p_brush_selected_);

			resources_created_ = true;
			return hr;
		}

		void widgets::image_view_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_bitmap_);
		}

		D2D1_RECT_F&
			widgets::image_view_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			if (!render || !visible_)
				return rect_;

			D2D1_ROUNDED_RECT rounded_rect{ rect_,
				specs_.corner_radius_x(), specs_.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_fill_);

			const size current_size = { rect_.right - rect_.left, rect_.bottom - rect_.top };

			if ((current_size.width != old_size_.width) ||
				current_size.height != old_size_.height)
				safe_release(&p_bitmap_);

			if (!p_bitmap_) {
				if (specs_.png_resource())	// png resource takes precedence
					load_bitmap_resource(p_render_target, p_IWICFactory_,
						page_.d_page_.get_form().d_.resource_module_handle_, specs_.png_resource(), "PNG",
						&p_bitmap_, current_size, specs_.enlarge_if_smaller(), specs_.keep_aspect_ratio(), specs_.quality());
				if (!specs_.file().empty() && !p_bitmap_)
					load_bitmap_file(p_render_target, p_IWICFactory_, convert_string(specs_.file()).c_str(),
						&p_bitmap_, current_size, specs_.enlarge_if_smaller(), specs_.keep_aspect_ratio(), specs_.quality());
			}

			if (p_bitmap_) {
				// retrieve the size of the bitmap and define suitably sized rectangle for the image
				const auto size = p_bitmap_->GetSize();
				auto rect_image = D2D1::RectF(0, 0, size.width, size.height);
				fit_rect(rect_, rect_image, false, true, true);

				// draw the bitmap
				p_render_target->DrawBitmap(p_bitmap_, rect_image);
			}

			p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_border_, specs_.border());

			if (!is_static_ && is_enabled_) {
				if (hit_ || pressed_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_hot_, pressed_ ?
						1.75f : 1.f);

				if (selected_)
					p_render_target->DrawRoundedRectangle(&rounded_rect, p_brush_selected_, pressed_ ?
						1.75f : 1.f);
			}

			old_size_ = current_size;

			return rect_;
		}

		widgets::image_view::image_view_specs&
			widgets::image_view_impl::specs() { return specs_; }

		widgets::image_view::image_view_specs&
			widgets::image_view_impl::operator()() { return specs(); }
	}
}
