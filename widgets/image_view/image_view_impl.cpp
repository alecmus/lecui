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
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_bitmap(nullptr),
			_p_IWICFactory(p_IWICFactory),
			_old_size({ 0.f, 0.f })	{}

		widgets::image_view_impl::~image_view_impl() { discard_resources(); }

		widgets::widget_type
			widgets::image_view_impl::type() {
			return lecui::widgets::widget_type::image_view;
		}

		HRESULT widgets::image_view_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = (_specs.events().click == nullptr && _specs.events().action == nullptr);
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
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

		void widgets::image_view_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_bitmap);
		}

		D2D1_RECT_F&
			widgets::image_view_impl::render(ID2D1HwndRenderTarget* p_render_target,
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

			if (!render || !_visible)
				return _rect;

			D2D1_ROUNDED_RECT rounded_rect{ _rect,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _p_brush_fill);

			const size current_size = { _rect.right - _rect.left, _rect.bottom - _rect.top };

			if ((current_size.width != _old_size.width) ||
				current_size.height != _old_size.height)
				safe_release(&_p_bitmap);

			if (!_p_bitmap) {
				// make a dpi scaled target size
				const size target_size{ current_size.width * get_dpi_scale(), current_size.height * get_dpi_scale() };

				if (_specs.png_resource())	// png resource takes precedence
					load_bitmap_resource(p_render_target, _p_IWICFactory,
						_page._d_page.get_form()._d._resource_module_handle, _specs.png_resource(), "PNG",
						&_p_bitmap, target_size, _specs.enlarge_if_smaller(), _specs.keep_aspect_ratio(), _specs.quality());
				if (!_specs.file().empty() && !_p_bitmap)
					load_bitmap_file(p_render_target, _p_IWICFactory, convert_string(_specs.file()).c_str(),
						&_p_bitmap, target_size, _specs.enlarge_if_smaller(), _specs.keep_aspect_ratio(), _specs.quality());
			}

			if (_p_bitmap) {
				// retrieve the size of the bitmap and define suitably sized rectangle for the image
				const auto size = _p_bitmap->GetSize();
				auto rect_image = D2D1::RectF(0, 0, size.width, size.height);
				fit_rect(_rect, rect_image, false, true, true);

				// draw the bitmap
				p_render_target->DrawBitmap(_p_bitmap, rect_image);
			}

			p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_border, _specs.border());

			if (!_is_static && _is_enabled) {
				if (_hit || _pressed)
					p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_hot, _pressed ?
						1.75f : 1.f);

				if (_selected)
					p_render_target->DrawRoundedRectangle(&rounded_rect, _p_brush_selected, _pressed ?
						1.75f : 1.f);
			}

			_old_size = current_size;

			return _rect;
		}

		widgets::image_view_specs&
			widgets::image_view_impl::specs() { return _specs; }

		widgets::image_view_specs&
			widgets::image_view_impl::operator()() { return specs(); }
	}
}
