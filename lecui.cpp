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
			left_(left), right_(right), top_(top), bottom_(bottom) {}

		float& rect::left() { return left_; }
		float rect::get_left() const { return left_; }

		rect& rect::left(const float& left) {
			left_ = left;
			return *this;
		}

		float& rect::right() { return right_; }
		float rect::get_right() const { return right_; }

		rect& rect::right(const float& right) {
			right_ = right;
			return *this;
		}

		float& rect::top() { return top_; }
		float rect::get_top() const { return top_; }

		rect& rect::top(const float& top) {
			top_ = top;
			return *this;
		}

		float& rect::bottom() { return bottom_; }
		float rect::get_bottom() const { return bottom_; }

		rect& rect::bottom(const float& bottom) {
			bottom_ = bottom;
			return *this;
		}

		float rect::width() const { return right_ - left_; }
		float rect::height() const { return bottom_ - top_; }
		size rect::size() const { return { width(), height() }; }

		rect& rect::width(float width) {
			right_ = left_ + width;
			return *this;
		}

		rect& rect::height(float height) {
			bottom_ = top_ + height;
			return *this;
		}

		rect& rect::size(lecui::size size) {
			width(size.width);
			height(size.height);
			return *this;
		}

		rect& rect::size(float width, float height) {
			right_ = left_ + width;
			bottom_ = top_ + height;
			return *this;
		}

		rect& rect::set(float x, float y, float cx, float cy) {
			left_ = x; top_ = y; right_ = left_ + cx; bottom_ = top_ + cy;
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
			const auto width_reference = rect_reference.right_ - rect_reference.left_;
			const auto height_reference = rect_reference.bottom_ - rect_reference.top_;

			const auto width = rect.width();
			const auto height = rect.height();

			switch (type) {
			case snap_type::bottom:
				rect.top_ = rect_reference.bottom_ + clearance;
				rect.left_ = rect_reference.left_ + (width_reference - width) / 2.f;
				break;

			case snap_type::bottom_right:
				rect.top_ = rect_reference.bottom_ + clearance;
				rect.left_ = rect_reference.right_ - width;
				break;

			case snap_type::top_left:
				rect.top_ = rect_reference.top_ - (height + clearance);
				rect.left_ = rect_reference.left_;
				break;

			case snap_type::top:
				rect.top_ = rect_reference.top_ - (height + clearance);
				rect.left_ = rect_reference.left_ + (width_reference - width) / 2.f;
				break;

			case snap_type::top_right:
				rect.top_ = rect_reference.top_ - (height + clearance);
				rect.left_ = rect_reference.right_ - width;
				break;

			case snap_type::right_top:
				rect.left_ = rect_reference.right_ + clearance;
				rect.top_ = rect_reference.top_;
				break;

			case snap_type::right:
				rect.left_ = rect_reference.right_ + clearance;
				rect.top_ = rect_reference.top_ + (height_reference - height) / 2.f;
				break;

			case snap_type::right_bottom:
				rect.left_ = rect_reference.right_ + clearance;
				rect.top_ = rect_reference.bottom_ - height;
				break;

			case snap_type::left_top:
				rect.left_ = rect_reference.left_ - (width + clearance);
				rect.top_ = rect_reference.top_;
				break;

			case snap_type::left:
				rect.left_ = rect_reference.left_ - (width + clearance);
				rect.top_ = rect_reference.top_ + (height_reference - height) / 2.f;
				break;

			case snap_type::left_bottom:
				rect.left_ = rect_reference.left_ - (width + clearance);
				rect.top_ = rect_reference.bottom_ - height;
				break;

			case snap_type::bottom_left:
			default:
				rect.top_ = rect_reference.bottom_ + clearance;
				rect.left_ = rect_reference.left_;
				break;
			}

			rect.width(width);
			rect.height(height);
			return *this;
		}

		bool rect::operator==(const rect& param) {
			return
				(left_ == param.left_) &&
				(right_ == param.right_) &&
				(top_ == param.top_) &&
				(bottom_ == param.bottom_);
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
			red_(red), green_(green), blue_(blue), alpha_(alpha) {
			red_ = smallest(red_, (unsigned short)255);
			red_ = largest(red_, (unsigned short)0);
			green_ = smallest(green_, (unsigned short)255);
			green_ = largest(green_, (unsigned short)0);
			blue_ = smallest(blue_, (unsigned short)255);
			blue_ = largest(blue_, (unsigned short)0);
			alpha_ = smallest(alpha_, (unsigned short)255);
			alpha_ = largest(alpha_, (unsigned short)0);
		}

		unsigned short& color::red() { return red_; }
		unsigned short color::get_red() const { return red_; }
		color& color::red(const unsigned short& red) {
			red_ = red;
			red_ = smallest(red_, (unsigned short)255);
			red_ = largest(red_, (unsigned short)0);
			return *this;
		}

		unsigned short& color::green() { return green_; }
		unsigned short color::get_green() const { return green_; }
		color& color::green(const unsigned short& green) {
			green_ = green;
			green_ = smallest(green_, (unsigned short)255);
			green_ = largest(green_, (unsigned short)0);
			return *this;
		}

		unsigned short& color::blue() { return blue_; }
		unsigned short color::get_blue() const { return blue_; }
		color& color::blue(const unsigned short& blue) {
			blue_ = blue;
			blue_ = smallest(blue_, (unsigned short)255);
			blue_ = largest(blue_, (unsigned short)0);
			return *this;
		}

		unsigned short& color::alpha() { return alpha_; }
		unsigned short color::get_alpha() const { return alpha_; }
		color& color::alpha(const unsigned short& alpha) {
			alpha_ = alpha;
			alpha_ = smallest(alpha_, (unsigned short)255);
			alpha_ = largest(alpha_, (unsigned short)0);
			return *this;
		}

		bool color::operator==(const color& param) {
			return
				(red_ == param.red_) &&
				(green_ == param.green_) &&
				(blue_ == param.blue_) &&
				(alpha_ == param.alpha_);
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
