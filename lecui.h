/*
** lecui.h - lecui interface
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

#pragma once

#if defined(LECUI_EXPORTS)
	#define lecui_api __declspec(dllexport)
#else
	#define lecui_api __declspec(dllimport)

	// for visual styles (message box, open file, save file)
	#pragma comment(linker, "\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
	language='*'\"")

	#if defined(_WIN64)
		#if defined(_DEBUG)
			#pragma comment(lib, "lecui64d.lib")
		#else
			#pragma comment(lib, "lecui64.lib")
		#endif
	#else
		#if defined(_DEBUG)
			#pragma comment(lib, "lecui32d.lib")
		#else
			#pragma comment(lib, "lecui32.lib")
		#endif
	#endif
#endif

#include <string>
#include <vector>

namespace liblec {
	namespace lecui {
		/// <summary>
		/// Get the version of the lecui library.
		/// </summary>
		/// 
		/// <returns>
		/// Returns the version number as a string in the form "lecui 1.0.0 04 Jan 2019"
		/// </returns>
		std::string lecui_api version();

		struct point {
			long x = 0;
			long y = 0;
		};

		struct size {
			unsigned long width = 0;
			unsigned long height = 0;
		};

		class lecui_api rect {
		public:
			long left = 0;
			long right = 0;
			long top = 0;
			long bottom = 0;

			long width() { return right - left; }
			long height() { return bottom - top; }
			void width(long width) { right = left + width; }
			void height(long height) { bottom = top + height; }
			void size(long width, long height) { right = left + width; bottom = top + height; }
			void position(long x, long y) { left = x; top = y; }
			void set(long x, long y, long cx, long cy) {
				left = x; top = y; right = left + cx; bottom = top + cy;
			}
		};

		struct color {
			unsigned short red = 0;
			unsigned short green = 0;
			unsigned short blue = 0;
			unsigned short alpha = 255;
		};

		struct file_type {
			std::string extension = "bmp";
			std::string description = "Bitmap Files";
		};

		struct open_file_params {
			std::vector<file_type> file_types;
			bool include_all_supported_types = true;
			std::string title = "Open File";
		};

		struct save_file_params {
			std::vector<file_type> file_types;
			bool include_all_supported_types = true;
			std::string title = "Save File";
		};

		struct list_column {
			std::string name;
			unsigned short width = 80;
		};
	}
}
