//
// lecui.cpp - lecui implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "lecui.h"
#include "versioninfo.h"
#include "form_common.h"

#include <Windows.h>
#include <ctime>

// DllMain function
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}
	return TRUE;
}

namespace liblec {
	namespace lecui {
		std::string version() {
			return lecuiname + std::string(" ") + lecuiversion + std::string(" ") + lecuidate;
		}

		rect::rect() :
			rect(0.f, 0.f, 0.f, 0.f) {}

		rect::rect(const float left, const float right, const float top, const float bottom):
			_left(left), _right(right), _top(top), _bottom(bottom) {}

		float& rect::left() { return _left; }
		float rect::get_left() const { return _left; }

		rect& rect::left(const float& left) {
			_left = left;
			return *this;
		}

		float& rect::right() { return _right; }
		float rect::get_right() const { return _right; }

		rect& rect::right(const float& right) {
			_right = right;
			return *this;
		}

		float& rect::top() { return _top; }
		float rect::get_top() const { return _top; }

		rect& rect::top(const float& top) {
			_top = top;
			return *this;
		}

		float& rect::bottom() { return _bottom; }
		float rect::get_bottom() const { return _bottom; }

		rect& rect::bottom(const float& bottom) {
			_bottom = bottom;
			return *this;
		}

		float rect::width() const { return _right - _left; }
		float rect::height() const { return _bottom - _top; }
		size rect::size() const { return { width(), height() }; }

		rect& rect::width(float width) {
			_right = _left + width;
			return *this;
		}

		rect& rect::height(float height) {
			_bottom = _top + height;
			return *this;
		}

		rect& rect::size(lecui::size size) {
			width(size.width);
			height(size.height);
			return *this;
		}

		rect& rect::size(float width, float height) {
			_right = _left + width;
			_bottom = _top + height;
			return *this;
		}

		rect& rect::set(float x, float y, float cx, float cy) {
			_left = x; _top = y; _right = _left + cx; _bottom = _top + cy;
			return *this;
		}

		rect& rect::move(float x, float y) {
			const auto cx = width();
			const auto cy = height();
			set(x, y, cx, cy);
			return *this;
		}

		rect& rect::place(const rect& rect_reference,
			const float& perc_h, const float& perc_v) {
			pos_rect(rect_reference, *this, perc_h, perc_v);
			return *this;
		}

		rect& rect::snap_to(const rect& rect_reference, snap_type type, const float& clearance) {
			auto& rect = *this;
			const auto width_reference = rect_reference._right - rect_reference._left;
			const auto height_reference = rect_reference._bottom - rect_reference._top;

			const auto width = rect.width();
			const auto height = rect.height();

			switch (type) {
			case snap_type::bottom:
				rect._top = rect_reference._bottom + clearance;
				rect._left = rect_reference._left + (width_reference - width) / 2.f;
				break;

			case snap_type::bottom_right:
				rect._top = rect_reference._bottom + clearance;
				rect._left = rect_reference._right - width;
				break;

			case snap_type::top_left:
				rect._top = rect_reference._top - (height + clearance);
				rect._left = rect_reference._left;
				break;

			case snap_type::top:
				rect._top = rect_reference._top - (height + clearance);
				rect._left = rect_reference._left + (width_reference - width) / 2.f;
				break;

			case snap_type::top_right:
				rect._top = rect_reference._top - (height + clearance);
				rect._left = rect_reference._right - width;
				break;

			case snap_type::right_top:
				rect._left = rect_reference._right + clearance;
				rect._top = rect_reference._top;
				break;

			case snap_type::right:
				rect._left = rect_reference._right + clearance;
				rect._top = rect_reference._top + (height_reference - height) / 2.f;
				break;

			case snap_type::right_bottom:
				rect._left = rect_reference._right + clearance;
				rect._top = rect_reference._bottom - height;
				break;

			case snap_type::left_top:
				rect._left = rect_reference._left - (width + clearance);
				rect._top = rect_reference._top;
				break;

			case snap_type::left:
				rect._left = rect_reference._left - (width + clearance);
				rect._top = rect_reference._top + (height_reference - height) / 2.f;
				break;

			case snap_type::left_bottom:
				rect._left = rect_reference._left - (width + clearance);
				rect._top = rect_reference._bottom - height;
				break;

			case snap_type::bottom_left:
			default:
				rect._top = rect_reference._bottom + clearance;
				rect._left = rect_reference._left;
				break;
			}

			rect.width(width);
			rect.height(height);
			return *this;
		}

		bool rect::operator==(const rect& param) {
			return
				(_left == param._left) &&
				(_right == param._right) &&
				(_top == param._top) &&
				(_bottom == param._bottom);
		}

		bool rect::operator!=(const rect& param) {
			return !operator==(param);
		}

		color::color() :
			color(0, 0, 0, 255) {}

		color::color(const unsigned short red,
			const unsigned short green,
			const unsigned short blue,
			const unsigned short alpha) :
			_red(red), _green(green), _blue(blue), _alpha(alpha) {
			_red = smallest(_red, (unsigned short)255);
			_red = largest(_red, (unsigned short)0);
			_green = smallest(_green, (unsigned short)255);
			_green = largest(_green, (unsigned short)0);
			_blue = smallest(_blue, (unsigned short)255);
			_blue = largest(_blue, (unsigned short)0);
			_alpha = smallest(_alpha, (unsigned short)255);
			_alpha = largest(_alpha, (unsigned short)0);
		}

		unsigned short& color::red() { return _red; }
		unsigned short color::get_red() const { return _red; }
		color& color::red(const unsigned short& red) {
			_red = red;
			_red = smallest(_red, (unsigned short)255);
			_red = largest(_red, (unsigned short)0);
			return *this;
		}

		unsigned short& color::green() { return _green; }
		unsigned short color::get_green() const { return _green; }
		color& color::green(const unsigned short& green) {
			_green = green;
			_green = smallest(_green, (unsigned short)255);
			_green = largest(_green, (unsigned short)0);
			return *this;
		}

		unsigned short& color::blue() { return _blue; }
		unsigned short color::get_blue() const { return _blue; }
		color& color::blue(const unsigned short& blue) {
			_blue = blue;
			_blue = smallest(_blue, (unsigned short)255);
			_blue = largest(_blue, (unsigned short)0);
			return *this;
		}

		unsigned short& color::alpha() { return _alpha; }
		unsigned short color::get_alpha() const { return _alpha; }
		color& color::alpha(const unsigned short& alpha) {
			_alpha = alpha;
			_alpha = smallest(_alpha, (unsigned short)255);
			_alpha = largest(_alpha, (unsigned short)0);
			return *this;
		}

		bool color::operator==(const color& param) {
			return
				(_red == param._red) &&
				(_green == param._green) &&
				(_blue == param._blue) &&
				(_alpha == param._alpha);
		}

		bool color::operator!=(const color& param) {
			return !operator==(param);
		}

		bool table_column::operator==(const table_column& param) {
			return
				(name == param.name) &&
				(width == param.width);
		}

		bool table_column::operator!=(const table_column& param) {
			return !operator==(param);
		}

		time::time() :
			time("") {}

		time::time(const std::string s) {
			if (s.empty()) {
				// get current local time
				std::time_t time_temp = ::time(0);
				std::tm time_out = { };
				localtime_s(&time_out, &time_temp);

				hour = (unsigned short)time_out.tm_hour;
				minute = (unsigned short)time_out.tm_min;
				second = (unsigned short)time_out.tm_sec;
			}
			else {
				try {
					std::stringstream ss;
					ss << s.substr(0, 2);	// parse hour
					ss >> hour;				// write hour
					ss.clear();
					ss << s.substr(3, 2);	// parse minute
					ss >> minute;			// write minute
					ss.clear();
					ss << s.substr(6, 2);	// parse second
					ss >> second;			// write second
				}
				catch (const std::exception& e) { log(e.what()); }
			}

			// impose limits
			hour = smallest(hour, (unsigned short)23);
			minute = smallest(minute, (unsigned short)59);
			second = smallest(second, (unsigned short)59);
		}

		std::string time::to_string() {
			return (hour < 10 ? "0" + std::to_string(hour) : std::to_string(hour)) + ":" +
				(minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute)) + ":" +
				(second < 10 ? "0" + std::to_string(second) : std::to_string(second));
		}

		int get::integer(const std::any& value) {
			return std::any_cast<int>(value);
		}

		double get::real(const std::any& value) {
			if (value.type() == typeid(float))
				return std::any_cast<float>(value);
			else
				return std::any_cast<double>(value);
		}

		std::string get::text(const std::any& value) {
			if (value.type() == typeid(const char*))
				return std::string(std::any_cast<const char*>(value));
			else
				return std::any_cast<std::string>(value);
		}
}
}
