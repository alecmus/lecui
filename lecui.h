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
		/// <summary>Get the version of the lecui library.</summary>
		/// <returns>Returns the version number as a string in the form
		/// "lecui 1.0.0 04 Jan 2019".</returns>
		std::string lecui_api version();

		/// <summary>Make a unique string.</summary>
		/// <returns>A unique string, e.g. 705c03bb-992f-40f1-841d-621bacc13eee.</returns>
		/// <remarks>The strings are guaranteed to be universally unique (universally unique
		/// identifier, or uuid for short).</remarks>
		std::string lecui_api unique_string();

		/// <summary>Make a short unique string.</summary>
		/// <returns>A short unique string, e.g. e5dc5cfb.</returns>
		/// <remarks>The shortening is made possible through the truncation of an internally
		/// generated universally unique string; as such, these strings are not guaranteed to be
		/// universally unique. The probability of a clash, however, is infinitesimal.</remarks>
		std::string lecui_api unique_string_short();

		struct point {
			float x = 0.f;
			float y = 0.f;
		};

		struct size {
			float width = 0.f;
			float height = 0.f;
		};

		class lecui_api rect {
		public:
			float left = 0.f;
			float right = 0.f;
			float top = 0.f;
			float bottom = 0.f;

			/// <summary>Get the rectangle's width.</summary>
			/// <returns>The width.</returns>
			[[nodiscard]] float width();

			/// <summary>Get the rectangle's height.</summary>
			/// <returns>The height.</returns>
			[[nodiscard]] float height();

			/// <summary>Get the rectangle's size.</summary>
			/// <returns>The size.</returns>
			[[nodiscard]] size size();

			/// <summary>Set the rectangle's width.</summary>
			/// <param name="width">The width to set it to.</param>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			void width(float width);

			/// <summary>Set the rectangle's height.</summary>
			/// <param name="height">The height to set it to.</param>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			void height(float height);

			/// <summary>Set the rectangle's size (both width and height).</summary>
			/// <param name="size">The size to set it to.</param>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			void size(lecui::size size);

			/// <summary>Set the rectangle's size (both width and height).</summary>
			/// <param name="width">The width to set.</param>
			/// <param name="height">The height to set.</param>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			void size(float width, float height);

			/// <summary>Set the rectangle dimensions and position.</summary>
			/// <param name="x">The x-coordinate.</param>
			/// <param name="y">The y-coordinate.</param>
			/// <param name="cx">The width.</param>
			/// <param name="cy">The height.</param>
			void set(float x, float y, float cx, float cy);

			/// <summary>Move the rectangle using it's top left corner.</summary>
			/// <param name="x">The new x-coordinate of the top-left corner.</param>
			/// <param name="y">The new y-coordinate of the top-left corner.</param>
			/// <remarks>The size of the rectangle remains unchanged. The rectangle is
			/// effectively translated to the new coodinates.</remarks>
			void move(float x, float y);

			/// <summary>Place this rectangle within or over another.</summary>
			/// <param name="rect_reference">The rectangle to place it on or over.</param>
			/// <param name="perc_h">How far along vertically to place it, as a percentage. 50%
			/// means this rectangle will be centered vertically.</param>
			/// <param name="perc_v">How far along horizontally to place it, as a percentage. 50%
			/// means this rectangle will be centered horizontally.</param>
			/// <remarks>This is a handy method for faster coding and easier maintenance.</remarks>
			void place(const rect& rect_reference, const float& perc_h, const float& perc_v);

			enum class snap_type {
				bottom_left,
				bottom,
				bottom_right,
				top_left,
				top,
				top_right,
				right_top,
				right,
				right_bottom,
				left_top,
				left,
				left_bottom,
			};

			/// <summary>Snap this rectangle to another, for quick positioning.</summary>
			/// <param name="rect_reference">The rectangle to snap to.</param>
			/// <param name="type">The snap type.</param>
			/// <param name="clearance">The clearance to use in the snapping operation.</param>
			/// <remarks>This is a handy method for faster coding and easier maintenance.</remarks>
			void snap_to(const rect& rect_reference, snap_type type, const float& clearance);

			bool operator==(const rect&);
			bool operator!=(const rect&);
		};

		struct color {
			unsigned short red = 0;
			unsigned short green = 0;
			unsigned short blue = 0;
			unsigned short alpha = 255;

			bool operator==(const color&);
			bool operator!=(const color&);
		};

		enum class sort_options {
			ascending,
			descending,
			none,
		};

		struct file_type {
			std::string extension = "png";
			std::string description = "PNG Image";
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

		struct table_column {
			std::string name;
			unsigned short width = 80;

			bool operator==(const table_column&);
			bool operator!=(const table_column&);
		};

		class lecui_api time {
		public:
			unsigned short hour = 0;
			unsigned short minute = 0;
			unsigned short second = 0;

			/// <summary>Constructor.</summary>
			/// <remarks>Sets time using the current local time.</remarks>
			time();

			/// <summary>Constructor.</summary>
			/// <param name="s">Time string, in the form "HH:mm:ss". Leave empty to set using
			/// the current local time.</param>
			time(const std::string s);

			/// <summary>Get time in the form of a string.</summary>
			/// <returns>Returns the time in the form "HH:mm:ss"</returns>
			std::string to_string();
		};

		struct date {
			unsigned short day = 1;
			unsigned short month = 1;
			unsigned short year = 2020;
		};
	}
}
