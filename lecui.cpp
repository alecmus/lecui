//
// lecui.cpp - lecui implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			return lecuiname + std::string(" ") + lecuiversion + std::string(", ") + lecuidate;
		}

		point::point() :
			point(0.f, 0.f) {}

		point::point(const float x, const float y) :
			_x(x), _y(y) {}

		float& point::x() { return _x; }
		float point::get_x() const { return _x; }

		point& point::x(const float& x) {
			_x = x;
			return *this;
		}

		float& point::y() { return _y; }
		float point::get_y() const { return _y; }

		point& point::y(const float& y) {
			_y = y;
			return *this;
		}

		size::size() :
			size(0.f, 0.f) {}

		size::size(const float width, const float height) :
			_width(width), _height(height) {}

		float& size::width() { return _width; }
		float size::get_width() const { return _width; }

		size& size::width(const float& width) {
			_width = width;
			return *this;
		}

		float& size::height() { return _height; }
		float size::get_height() const { return _height; }

		size& size::height(const float& height) {
			_height = height;
			return *this;
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
			width(size.width());
			height(size.height());
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

		rect& rect::clip_to(const rect& rect_reference, const float& clearance) {
			auto& rect = *this;

			const auto left = rect_reference.get_left() + clearance;
			const auto right = rect_reference.get_right() - clearance;
			const auto top = rect_reference.get_top() + clearance;
			const auto bottom = rect_reference.get_bottom() - clearance;

			if (rect._left < left)
				rect._left = left;

			if (rect._left > right)
				rect._left = right;

			if (rect._right > right)
				rect._right = right;

			if (rect._right < left)
				rect._right = left;

			if (rect._top < top)
				rect._top = top;

			if (rect._top > bottom)
				rect._top = bottom;

			if (rect._bottom > bottom)
				rect._bottom = bottom;

			if (rect._bottom < top)
				rect._bottom = top;

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

		resize_params::resize_params() :
			resize_params(0.f, 0.f, 0.f, 0.f) {}

		resize_params::resize_params(const float x_rate,
			const float y_rate, const float width_rate, const float height_rate) :
			_x_rate(x_rate), _y_rate(y_rate), _width_rate(width_rate), _height_rate(height_rate) {}

		float& resize_params::x_rate() { return _x_rate; }
		float resize_params::get_x_rate() const { return _x_rate; }

		resize_params& resize_params::x_rate(const float& x_rate) {
			_x_rate = x_rate;
			return *this;
		}

		float& resize_params::y_rate() { return _y_rate; }
		float resize_params::get_y_rate() const { return _y_rate; }

		resize_params& resize_params::y_rate(const float& y_rate) {
			_y_rate = y_rate;
			return *this;
		}

		float& resize_params::width_rate() { return _width_rate; }
		float resize_params::get_width_rate() const { return _width_rate; }

		resize_params& resize_params::width_rate(const float& width_rate) {
			_width_rate = width_rate;
			return *this;
		}

		float& resize_params::height_rate() { return _height_rate; }
		float resize_params::get_height_rate() const { return _height_rate; }

		resize_params& resize_params::height_rate(const float& height_rate) {
			_height_rate = height_rate;
			return *this;
		}

		float& resize_params::min_x() { return _min_x; }
		const float resize_params::get_min_x() const { return _min_x; }

		resize_params& resize_params::min_x(const float& min_x) {
			_min_x = min_x;
			return *this;
		}

		float& resize_params::min_y() { return _min_y; }
		float resize_params::get_min_y() const { return _min_y; }

		resize_params& resize_params::min_y(const float& min_y) {
			_min_y = min_y;
			return *this;
		}

		float& resize_params::max_x() { return _max_x; }
		float resize_params::get_max_x() const { return _max_x; }

		resize_params& resize_params::max_x(const float& max_x) {
			_max_x = max_x;
			return *this;
		}

		float& resize_params::max_y() { return _max_y; }
		float resize_params::get_max_y() const { return _max_y; }

		resize_params& resize_params::max_y(const float& max_y) {
			_max_y = max_y;
			return *this;
		}

		float& resize_params::min_width() { return _min_width; }
		float resize_params::get_min_width() const { return _min_width; }

		resize_params& resize_params::min_width(const float& min_width) {
			_min_width = min_width;
			return *this;
		}

		float& resize_params::min_height() { return _min_height; }
		float resize_params::get_min_height() const { return _min_height; }

		resize_params& resize_params::min_height(const float& min_height) {
			_min_height = min_height;
			return *this;
		}

		float& resize_params::max_width() { return _max_width; }
		float resize_params::get_max_width() const { return _max_width; }

		resize_params& resize_params::max_width(const float& max_width) {
			_max_width = max_width;
			return *this;
		}

		float& resize_params::max_height() { return _max_height; }
		float resize_params::get_max_height() const { return _max_height; }

		resize_params& resize_params::max_height(const float& max_height) {
			_max_height = max_height;
			return *this;
		}

		bool resize_params::operator==(const resize_params& param) {
			return
				(_x_rate == param._x_rate) &&
				(_y_rate == param._y_rate) &&
				(_width_rate == param._width_rate) &&
				(_height_rate == param._height_rate) &&
				(_min_x == param._min_x) &&
				(_min_y == param._min_y) &&
				(_max_x == param._max_x) &&
				(_max_y == param._max_y) &&
				(_min_width == param._min_width) &&
				(_min_height == param._min_height) &&
				(_max_width == param._max_width) &&
				(_max_height == param._max_height);
		}

		bool resize_params::operator!=(const resize_params& param) {
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

		color& color::darken(const float& percentage) {
			const unsigned short r_target = 0;
			const unsigned short g_target = 0;
			const unsigned short b_target = 0;

			_red = unsigned short((float)(_red - r_target) * (100.f - percentage) / 100.f);
			_green = unsigned short((float)(_green - g_target) * (100.f - percentage) / 100.f);
			_blue = unsigned short((float)(_blue - b_target) * (100.f - percentage) / 100.f);

			return *this;
		}

		color& color::lighten(const float& percentage) {
			const unsigned short r_target = 255;
			const unsigned short g_target = 255;
			const unsigned short b_target = 255;

			_red = _red + int((double)(r_target - _red) * percentage / 100.f);
			_green = _green + int((double)(g_target - _green) * percentage / 100.f);
			_blue = _blue + int((double)(b_target - _blue) * percentage / 100.f);

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

		std::string& open_file_params::title() { return _title; }
		const std::string& open_file_params::get_title() const { return _title; }
		open_file_params& open_file_params::title(const std::string& title) {
			_title = title;
			return *this;
		}

		std::vector<file_type>& open_file_params::file_types() { return _file_types; }
		const std::vector<file_type>& open_file_params::get_file_types() const { return _file_types; }
		open_file_params& open_file_params::file_types(const std::vector<file_type>& file_types) {
			_file_types = file_types;
			return *this;
		}

		std::string& open_file_params::default_type() { return _default_type; }
		const std::string& open_file_params::get_default_type() const { return _default_type; }
		open_file_params& open_file_params::default_type(const std::string& default_type) {
			_default_type = default_type;
			return *this;
		}

		bool& open_file_params::include_all_supported_types() { return _include_all_supported_types; }
		const bool& open_file_params::get_include_all_supported_types() const { return _include_all_supported_types; }
		open_file_params& open_file_params::include_all_supported_types(const bool& include_all_supported_types) {
			_include_all_supported_types = include_all_supported_types;
			return *this;
		}

		bool& open_file_params::allow_multi_select() { return _allow_multi_select; }
		const bool& open_file_params::get_allow_multi_select() const { return _allow_multi_select; }
		open_file_params& open_file_params::allow_multi_select(const bool& allow_multi_select) {
			_allow_multi_select = allow_multi_select;
			return *this;
		}
		
		std::string& save_file_params::title() { return _title; }
		const std::string& save_file_params::get_title() const { return _title; }
		save_file_params& save_file_params::title(const std::string& title) {
			_title = title;
			return *this;
		}

		std::vector<file_type>& save_file_params::file_types() { return _file_types; }
		const std::vector<file_type>& save_file_params::get_file_types() const { return _file_types; }
		save_file_params& save_file_params::file_types(const std::vector<file_type>& file_types) {
			_file_types = file_types;
			return *this;
		}

		std::string& save_file_params::default_type() { return _default_type; }
		const std::string& save_file_params::get_default_type() const { return _default_type; }
		save_file_params& save_file_params::default_type(const std::string& default_type) {
			_default_type = default_type;
			return *this;
		}

		bool& save_file_params::include_all_files() { return _include_all_files; }
		const bool& save_file_params::get_include_all_files() const { return _include_all_files; }
		save_file_params& save_file_params::include_all_files(const bool& include_all_files) {
			_include_all_files = include_all_files;
			return *this;
		}
	}
}
