//
// controls.cpp - controls implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "controls.h"
#include "form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class controls::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		controls::controls(form& fm) : d_(*(new impl(fm))) {}
		controls::~controls() { delete& d_; }

		void controls::resize(bool enable) { d_.fm_.d_.allow_resizing_ = enable; }
		void controls::minimize(bool enable) { d_.fm_.d_.allow_minimize_ = enable; }
		void controls::top_most(bool make_top_most) { d_.fm_.d_.top_most_ = make_top_most; }

		class dimensions::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		dimensions::dimensions(form& fm) :
			d_(*(new impl(fm))) {}

		dimensions::~dimensions() { delete& d_; }

		lecui::rect dimensions::measure_label(const std::string& formatted_text,
			const std::string& font, const float font_size,
			bool center_h, bool center_v, const lecui::rect max_rect) {
			auto rect = convert_rect(max_rect);
			rect = widgets::measure_label(d_.fm_.d_.p_directwrite_factory_, formatted_text,
				font, font_size, center_h, center_v, rect);
			return convert_rect(rect);
		}

		lecui::rect dimensions::working_area() {
			const auto rect = d_.fm_.d_.get_working_area(GetDesktopWindow());
			return lecui::rect{ static_cast<float>(rect.left), static_cast<float>(rect.right),
				static_cast<float>(rect.top), static_cast<float>(rect.bottom) };
		}

		lecui::rect dimensions::cursor_rect() {
			const auto point = d_.fm_.d_.get_cursor_position();
			const auto size = d_.fm_.d_.get_cursor_size();
			const auto dpi_scale_ = d_.fm_.d_.dpi_scale_;

			lecui::rect rect;
			rect.left = (point.x / dpi_scale_);
			rect.top = (point.y / dpi_scale_);
			rect.width(size.width / dpi_scale_);
			rect.height(size.height / dpi_scale_);

			return rect;
		}

		void dimensions::size(const lecui::size& size) {
			if (size.width) {
				d_.fm_.d_.size_.width = size.width;

				// force minimum width to conform (it cannot be greater)
				d_.fm_.d_.min_size_.width = smallest(d_.fm_.d_.size_.width, d_.fm_.d_.min_size_.width);
			}

			if (size.height) {
				d_.fm_.d_.size_.height = size.height;

				// force minimum height to conform (it cannot be greater)
				d_.fm_.d_.min_size_.height = smallest(d_.fm_.d_.size_.height, d_.fm_.d_.min_size_.height);
			}
		}

		const size& dimensions::size() {
			return d_.fm_.d_.size_;
		}

		void dimensions::minimum(const lecui::size& size) {
			if (size.width)		// do not allow minimum width to be greater than current window width
				d_.fm_.d_.min_size_.width = smallest(size.width, d_.fm_.d_.size_.width);

			if (size.height)	// do not allow minimum height to be greater than current window height
				d_.fm_.d_.min_size_.height = smallest(size.height, d_.fm_.d_.size_.height);
		}

		const size& dimensions::minimum() {
			return d_.fm_.d_.min_size_;
		}
	}
}
