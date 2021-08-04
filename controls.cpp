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
				_fm(fm) {}
			form& _fm;
		};

		controls::controls(form& fm) : _d(*(new impl(fm))) {}
		controls::~controls() { delete& _d; }

		controls& controls::allow_resize(bool enable) {
			_d._fm._d._allow_resizing = enable;
			return *this;
		}

		controls& controls::allow_minimize(bool enable) {
			_d._fm._d._allow_minimize = enable;
			return *this;
		}

		controls& controls::make_top_most(bool make_top_most) {
			_d._fm._d._top_most = make_top_most;
			return *this;
		}

		controls& controls::start_hidden(bool hidden) {
			_d._fm._d._start_hidden = hidden;
			return *this;
		}

		class dimensions::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		dimensions::dimensions(form& fm) :
			_d(*(new impl(fm))) {}

		dimensions::~dimensions() { delete& _d; }

		lecui::rect dimensions::measure_label(const std::string& formatted_text,
			const std::string& font, const float font_size,
			bool center_h, bool center_v, const lecui::rect max_rect) {
			auto rect = convert_rect(max_rect);
			rect = widgets::measure_label(_d._fm._d._p_directwrite_factory, formatted_text,
				font, font_size, center_h, center_v, rect);
			return convert_rect(rect);
		}

		lecui::rect dimensions::working_area() {
			const auto rect = _d._fm._d.get_working_area(GetDesktopWindow());
			return lecui::rect{ static_cast<float>(rect.left), static_cast<float>(rect.right),
				static_cast<float>(rect.top), static_cast<float>(rect.bottom) };
		}

		lecui::rect dimensions::cursor_rect() {
			const auto point = _d._fm._d.get_cursor_position();
			const auto size = _d._fm._d.get_cursor_size();
			const auto _dpi_scale = _d._fm._d._dpi_scale;

			lecui::rect rect;
			rect
				.left((point.x / _dpi_scale))
				.top((point.y / _dpi_scale))
				.width(size.width / _dpi_scale)
				.height(size.height / _dpi_scale);

			return rect;
		}

		dimensions& dimensions::set_size(const lecui::size& size) {
			auto old_size = _d._fm._d._size;

			if (size.get_width()) {
				_d._fm._d._size.width(size.get_width());

				// force minimum width to conform (it cannot be greater)
				_d._fm._d._min_size.width(smallest(_d._fm._d._size.get_width(), _d._fm._d._min_size.get_width()));
			}

			if (size.get_height()) {
				_d._fm._d._size.height(size.get_height());

				// force minimum height to conform (it cannot be greater)
				_d._fm._d._min_size.height(smallest(_d._fm._d._size.get_height(), _d._fm._d._min_size.height()));
			}

			if (IsWindow(_d._fm._d._hWnd)) {
				RECT rc;
				GetWindowRect(_d._fm._d._hWnd, &rc);

				MoveWindow(_d._fm._d._hWnd, rc.left, rc.top,
					static_cast<LONG>(size.get_width() * _d._fm._d._dpi_scale),
					static_cast<LONG>(size.get_height() * _d._fm._d._dpi_scale),
					TRUE);

				// move the control buttons
				const auto change_in_width = _d._fm._d._size.width() - old_size.width();
				const auto change_in_height = _d._fm._d._size.height() - old_size.height();

				if (_d._fm._d._p_close_button.get()) {
					_d._fm._d._p_close_button->specs().rect().left() += change_in_width;
					_d._fm._d._p_close_button->specs().rect().right() += change_in_width;
					_d._fm._d._p_close_button->specs().rect().top() += change_in_height;
					_d._fm._d._p_close_button->specs().rect().bottom() += change_in_height;
				}

				if (_d._fm._d._p_maximize_button.get()) {
					_d._fm._d._p_maximize_button->specs().rect().left() += change_in_width;
					_d._fm._d._p_maximize_button->specs().rect().right() += change_in_width;
					_d._fm._d._p_maximize_button->specs().rect().top() += change_in_height;
					_d._fm._d._p_maximize_button->specs().rect().bottom() += change_in_height;
				}

				if (_d._fm._d._p_minimize_button.get()) {
					_d._fm._d._p_minimize_button->specs().rect().left() += change_in_width;
					_d._fm._d._p_minimize_button->specs().rect().right() += change_in_width;
					_d._fm._d._p_minimize_button->specs().rect().top() += change_in_height;
					_d._fm._d._p_minimize_button->specs().rect().bottom() += change_in_height;
				}
			}

			return *this;
		}

		const size dimensions::get_size() {
			if (IsWindow(_d._fm._d._hWnd)) {
				RECT rc;
				GetWindowRect(_d._fm._d._hWnd, &rc);
				unscale_RECT(rc, _d._fm._d._dpi_scale);
				const size _size = { static_cast<float>(rc.right - rc.left), static_cast<float>(rc.bottom - rc.top) };
				return _size;
			}
			else
				return _d._fm._d._size;
		}

		const lecui::point dimensions::get_position() {
			if (IsWindow(_d._fm._d._hWnd)) {
				RECT rc;
				GetWindowRect(_d._fm._d._hWnd, &rc);
				unscale_RECT(rc, _d._fm._d._dpi_scale);
				return { static_cast<float>(rc.left), static_cast<float>(rc.top) };
			}
			else
				return { 0.f, 0.f };
		}

		dimensions& dimensions::set_minimum(const lecui::size& size) {
			if (size.get_width())		// do not allow minimum width to be greater than current window width
				_d._fm._d._min_size.width(smallest(size.get_width(), _d._fm._d._size.get_width()));

			if (size.get_height())	// do not allow minimum height to be greater than current window height
				_d._fm._d._min_size.height(smallest(size.get_height(), _d._fm._d._size.get_height()));

			return *this;
		}

		const size& dimensions::get_minimum() {
			return _d._fm._d._min_size;
		}
	}
}
