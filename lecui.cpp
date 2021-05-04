/*
** lecui.cpp - lecui implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

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

		float rect::width() { return right - left; }
		float rect::height() { return bottom - top; }
		size rect::size() { return { width(), height() }; }
		void rect::width(float width) { right = left + width; }
		void rect::height(float height) { bottom = top + height; }
		void rect::size(lecui::size size) { width(size.width); height(size.height); }
		void rect::size(float width, float height) { right = left + width; bottom = top + height; }

		void rect::set(float x, float y, float cx, float cy) {
			left = x; top = y; right = left + cx; bottom = top + cy;
		}

		void rect::move(float x, float y) {
			const auto cx = width();
			const auto cy = height();
			set(x, y, cx, cy);
		}

		void rect::place(const rect& rect_reference,
			const float& perc_h, const float& perc_v) {
			pos_rect(rect_reference, *this, perc_h, perc_v);
		}

		void rect::snap_to(const rect& rect_reference, snap_type type, const float& clearance) {
			auto& rect = *this;
			const auto width_reference = rect_reference.right - rect_reference.left;
			const auto height_reference = rect_reference.bottom - rect_reference.top;

			const auto width = rect.width();
			const auto height = rect.height();

			switch (type) {
			case snap_type::bottom:
				rect.top = rect_reference.bottom + clearance;
				rect.left = rect_reference.left + (width_reference - width) / 2.f;
				break;

			case snap_type::bottom_right:
				rect.top = rect_reference.bottom + clearance;
				rect.left = rect_reference.right - width;
				break;

			case snap_type::top_left:
				rect.top = rect_reference.top - (height + clearance);
				rect.left = rect_reference.left;
				break;

			case snap_type::top:
				rect.top = rect_reference.top - (height + clearance);
				rect.left = rect_reference.left + (width_reference - width) / 2.f;
				break;

			case snap_type::top_right:
				rect.top = rect_reference.top - (height + clearance);
				rect.left = rect_reference.right - width;
				break;

			case snap_type::right_top:
				rect.left = rect_reference.right + clearance;
				rect.top = rect_reference.top;
				break;

			case snap_type::right:
				rect.left = rect_reference.right + clearance;
				rect.top = rect_reference.top + (height_reference - height) / 2.f;
				break;

			case snap_type::right_bottom:
				rect.left = rect_reference.right + clearance;
				rect.top = rect_reference.bottom - height;
				break;

			case snap_type::left_top:
				rect.left = rect_reference.left - (width + clearance);
				rect.top = rect_reference.top;
				break;

			case snap_type::left:
				rect.left = rect_reference.left - (width + clearance);
				rect.top = rect_reference.top + (height_reference - height) / 2.f;
				break;

			case snap_type::left_bottom:
				rect.left = rect_reference.left - (width + clearance);
				rect.top = rect_reference.bottom - height;
				break;

			case snap_type::bottom_left:
			default:
				rect.top = rect_reference.bottom + clearance;
				rect.left = rect_reference.left;
				break;
			}

			rect.width(width);
			rect.height(height);
		}

		bool rect::operator==(const rect& param) {
			return
				(left == param.left) &&
				(right == param.right) &&
				(top == param.top) &&
				(bottom == param.bottom);
		}

		bool rect::operator!=(const rect& param) {
			return !operator==(param);
		}

		bool color::operator==(const color& param) {
			return
				(red == param.red) &&
				(green == param.green) &&
				(blue == param.blue) &&
				(alpha == param.alpha);
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
	}
}
