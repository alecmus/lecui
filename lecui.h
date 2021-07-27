//
// lecui.h - lecui interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

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

#if defined(_MSC_VER)
// Turn off annoying "needs to have dll-interface" warning
#pragma warning(disable:4251)
#endif

#include <string>
#include <vector>
#include <map>
#include <any>

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

		/// <summary>A point.</summary>
		struct point {
			/// <summary>The x-coordinate of the point, from the top-left.</summary>
			float x = 0.f;

			/// <summary>The y-coordinate of the point, from the top-left.</summary>
			float y = 0.f;
		};

		/// <summary>Size of a rectangular structure.</summary>
		class lecui_api size {
			float _width = 0.f;
			float _height = 0.f;

		public:
			/// <summary>Size constructor. Initializes with a size of 0.f x 0.f.</summary>
			size();

			/// <summary>Size constructor.</summary>
			/// <param name="width">The width of the rectangular structure, in pixels.</param>
			/// <param name="height">The height of the rectangular structure, in pixels.</param>
			size(const float width, const float height);

			/// <summary>Get or set the width of the rectangular structure, in pixels.</summary>
			/// <returns>A reference to the property.</returns>
			float& width();

			/// <summary>Get or set the width of the rectangular structure, in pixels.</summary>
			/// <returns>The width, in pixels.</returns>
			float get_width() const;

			/// <summary>Set the width of a rectangular structure, in pixels.</summary>
			/// <param name="width">The width, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			size& width(const float& width);

			/// <summary>Get or set the height of the rectangular structure, in pixels.</summary>
			/// <returns>A reference to the property.</returns>
			float& height();

			/// <summary>Get or set the height of the rectangular structure, in pixels.</summary>
			/// <returns>The height, in pixels.</returns>
			float get_height() const;

			/// <summary>Set the height of a rectangular structure, in pixels.</summary>
			/// <param name="height">The height, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			size& height(const float& height);
		};

		/// <summary>Rectangle class.</summary>
		class lecui_api rect {
			float _left = 0.f;
			float _right = 0.f;
			float _top = 0.f;
			float _bottom = 0.f;

		public:
			/// <summary>Rectangle constructor. Makes a zero size rectangle with zero width and height.</summary>
			rect();

			/// <summary>Rectangle constructor.</summary>
			/// <param name="left">The left coordinate, in pixels.</param>
			/// <param name="right">The right coordinate, in pixels.</param>
			/// <param name="top">The top coordinate, in pixels.</param>
			/// <param name="bottom">The bottom coordinate, in pixels.</param>
			rect(const float left, const float right, const float top, const float bottom);

			/// <summary>Get or set the left coordinate.</summary>
			/// <returns>A reference to the left coordinate, in pixels.</returns>
			float& left();

			/// <summary>Get the left coordinate.</summary>
			/// <returns>The left coordinate, in pixels.</returns>
			float get_left() const;

			/// <summary>Set the left coordinate.</summary>
			/// <param name="left">The left coordinate, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			rect& left(const float& left);

			/// <summary>Get or set the right coordinate.</summary>
			/// <returns>A reference to the right coordinate, in pixels.</returns>
			float& right();

			/// <summary>Get the right coordinate.</summary>
			/// <returns>The right coordinate, in pixels.</returns>
			float get_right() const;

			/// <summary>Set the right coordinate.</summary>
			/// <param name="left">The right coordinate, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			rect& right(const float& right);

			/// <summary>Get or set the top coordinate.</summary>
			/// <returns>A reference to the top coordinate, in pixels.</returns>
			float& top();

			/// <summary>Get the top coordinate.</summary>
			/// <returns>The top coordinate, in pixels.</returns>
			float get_top() const;

			/// <summary>Set the top coordinate.</summary>
			/// <param name="left">The top coordinate, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			rect& top(const float& top);

			/// <summary>Get or set the bottom coordinate.</summary>
			/// <returns>A reference to the bottom coordinate, in pixels.</returns>
			float& bottom();

			/// <summary>Get the bottom coordinate.</summary>
			/// <returns>The bottom coordinate, in pixels.</returns>
			float get_bottom() const;

			/// <summary>Set the bottom coordinate.</summary>
			/// <param name="left">The bottom coordinate, in pixels.</param>
			/// <returns>A reference to the modified object.</returns>
			rect& bottom(const float& bottom);

			/// <summary>Get the rectangle's width.</summary>
			/// <returns>The width.</returns>
			[[nodiscard]] float width() const;

			/// <summary>Get the rectangle's height.</summary>
			/// <returns>The height.</returns>
			[[nodiscard]] float height() const;

			/// <summary>Get the rectangle's size.</summary>
			/// <returns>The size.</returns>
			[[nodiscard]] size size() const;

			/// <summary>Set the rectangle's width.</summary>
			/// <param name="width">The width to set it to.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			rect& width(float width);

			/// <summary>Set the rectangle's height.</summary>
			/// <param name="height">The height to set it to.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			rect& height(float height);

			/// <summary>Set the rectangle's size (both width and height).</summary>
			/// <param name="size">The size to set it to.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			rect& size(lecui::size size);

			/// <summary>Set the rectangle's size (both width and height).</summary>
			/// <param name="width">The width to set.</param>
			/// <param name="height">The height to set.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The top left corner is used as an anchor.</remarks>
			rect& size(float width, float height);

			/// <summary>Set the rectangle dimensions and position.</summary>
			/// <param name="x">The x-coordinate.</param>
			/// <param name="y">The y-coordinate.</param>
			/// <param name="cx">The width.</param>
			/// <param name="cy">The height.</param>
			/// <returns>A reference to the modified object.</returns>
			rect& set(float x, float y, float cx, float cy);

			/// <summary>Move the rectangle using it's top left corner.</summary>
			/// <param name="x">The new x-coordinate of the top-left corner.</param>
			/// <param name="y">The new y-coordinate of the top-left corner.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>The size of the rectangle remains unchanged. The rectangle is
			/// effectively translated to the new coodinates.</remarks>
			rect& move(float x, float y);

			/// <summary>Place this rectangle within or over another.</summary>
			/// <param name="rect_reference">The rectangle to place it on or over.</param>
			/// <param name="perc_h">How far along vertically to place it, as a percentage. 50%
			/// means this rectangle will be centered vertically.</param>
			/// <param name="perc_v">How far along horizontally to place it, as a percentage. 50%
			/// means this rectangle will be centered horizontally.</param>
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>This is a handy method for faster coding and easier maintenance.</remarks>
			rect& place(const rect& rect_reference, const float& perc_h, const float& perc_v);

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
			/// <returns>A reference to the modified object.</returns>
			/// <remarks>This is a handy method for faster coding and easier maintenance.</remarks>
			rect& snap_to(const rect& rect_reference, snap_type type, const float& clearance);

			bool operator==(const rect&);
			bool operator!=(const rect&);
		};

		/// <summary>Resize parameters.</summary>
		/// <remarks>Affects how the widget behaves when the immediate container's
		/// dimensions change.</remarks>
		class lecui_api resize_params {
			float _x_rate = 0.f;
			float _y_rate = 0.f;
			float _width_rate = 0.f;
			float _height_rate = 0.f;
			float _min_x = 0.f;
			float _min_y = 0.f;
			float _max_x = 0.f;
			float _max_y = 0.f;
			float _min_width = 0.f;
			float _min_height = 0.f;
			float _max_width = 0.f;
			float _max_height = 0.f;

		public:
			/// <summary>Constructor.</summary>
			/// <remarks>Initializes with all fields set to zero.</remarks>
			resize_params();

			/// <summary>Constructor that allows initializing the most important properties.</summary>
			/// <param name="x_rate">The percentage rate for following the container's right border.</param>
			/// <param name="y_rate">The percentage rate for following the container's bottom border.</param>
			/// <param name="width_rate">The percentage rate for following the container's change in width.</param>
			/// <param name="height_rate">The percentage rate for following the container's change in 
			/// height.</param>
			/// <remarks>Initializes the rest of the fields set to zero.</remarks>
			resize_params(const float x_rate, const float y_rate, const float width_rate, const float height_rate);

			/// <summary>Get or set the percentage rate for following the container's right border.
			/// 0 = doesn't move horizontally, 100 = moves same number of pixels
			/// horizontally that the container's right border has moved.</summary>
			/// <returns>A reference to the property.</returns>
			float& x_rate();

			/// <summary>Get the percentage rate for following the container's right border.
			/// 0 = doesn't move horizontally, 100 = moves same number of pixels
			/// horizontally that the container's right border has moved.</summary>
			/// <returns>The value of the property.</returns>
			float get_x_rate() const;

			/// <summary>Set the percentage rate for following the container's right border.
			/// 0 = doesn't move horizontally, 100 = moves same number of pixels
			/// horizontally that the container's right border has moved.</summary>
			/// <param name="x_rate">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& x_rate(const float& x_rate);

			/// <summary>Get or set the percentage rate for following the container's bottom border.
			/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
			/// that the container's bottom border has moved.</summary>
			/// <returns>A reference to the property.</returns>
			float& y_rate();

			/// <summary>Get the percentage rate for following the container's bottom border.
			/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
			/// that the container's bottom border has moved.</summary>
			/// <returns>The value of the property.</returns>
			float get_y_rate() const;

			/// <summary>Set the percentage rate for following the container's bottom border.
			/// 0 = doesn't move vertically, 100 = moves same number of pixels vertically
			/// that the container's bottom border has moved.</summary>
			/// <param name="y_rate">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& y_rate(const float& y_rate);

			/// <summary>Get or set the percentage rate for following the container's change in width.
			/// 0 = doesn't follow change in container's width, 100 = width changes at 
			/// same rate as that of container.</summary>
			/// <returns>A reference to the property.</returns>
			float& width_rate();

			/// <summary>Get the percentage rate for following the container's change in width.
			/// 0 = doesn't follow change in container's width, 100 = width changes at 
			/// same rate as that of container.</summary>
			/// <returns>The value of the property.</returns>
			float get_width_rate() const;

			/// <summary>Set the percentage rate for following the container's change in width.
			/// 0 = doesn't follow change in container's width, 100 = width changes at 
			/// same rate as that of container.</summary>
			/// <param name="width_rate">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& width_rate(const float& width_rate);

			/// <summary>Get or set the percentage rate for following the container's change in 
			/// height. 0 = doesn't follow change in container's height, 100 = height 
			/// changes at same rate as that of container.</summary>
			/// <returns>A reference to the property.</returns>
			float& height_rate();

			/// <summary>Get the percentage rate for following the container's change in 
			/// height. 0 = doesn't follow change in container's height, 100 = height 
			/// changes at same rate as that of container.</summary>
			/// <returns>The value of the property.</returns>
			float get_height_rate() const;

			/// <summary>Set the percentage rate for following the container's change in 
			/// height. 0 = doesn't follow change in container's height, 100 = height 
			/// changes at same rate as that of container.</summary>
			/// <param name="height_rate">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& height_rate(const float& height_rate);

			/// <summary>Get or set the minimum x-coordinate permitted as widget moves.</summary>
			/// <returns>A reference to the property.</returns>
			float& min_x();

			/// <summary>Get the minimum x-coordinate permitted as widget moves.</summary>
			/// <returns>The value of the property.</returns>
			const float get_min_x() const;

			/// <summary>Set the minimum x-coordinate permitted as widget moves.</summary>
			/// <param name="min_x">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& min_x(const float& min_x);

			/// <summary>Get or set the minimum y-coordinate permitted as widget moves.</summary>
			/// <returns>A reference to the property.</returns>
			float& min_y();

			/// <summary>Get minimum y-coordinate permitted as widget moves.</summary>
			/// <returns>The value of the property.</returns>
			float get_min_y() const;

			/// <summary>Set the minimum y-coordinate permitted as widget moves.</summary>
			/// <param name="min_y">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& min_y(const float& min_y);

			/// <summary>Get or set the maximum x-coordinate permitted as widget moves.</summary>
			/// <returns>A reference to the property.</returns>
			float& max_x();

			/// <summary>Get the maximum x-coordinate permitted as widget moves.</summary>
			/// <returns>The value of the property.</returns>
			float get_max_x() const;

			/// <summary>Set the maximum x-coordinate permitted as widget moves.</summary>
			/// <param name="max_x">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& max_x(const float& max_x);

			/// <summary>Get or set the maximum y-coordinate permitted as widget moves.</summary>
			/// <returns>A reference to the property.</returns>
			float& max_y();

			/// <summary>Get maximum y-coordinate permitted as widget moves.</summary>
			/// <returns>The value of the property.</returns>
			float get_max_y() const;

			/// <summary>Set the maximum y-coordinate permitted as widget moves.</summary>
			/// <param name="max_y">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& max_y(const float& max_y);

			/// <summary>Get or set the minimum width permitted as widget is resized.</summary>
			/// <returns>A reference to the property.</returns>
			float& min_width();

			/// <summary>Get the minimum width permitted as widget is resized.</summary>
			/// <returns>The value of the property.</returns>
			float get_min_width() const;

			/// <summary>Set the minimum width permitted as widget is resized.</summary>
			/// <param name="min_width">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& min_width(const float& min_width);

			/// <summary>Get or set the minimum height permitted as widget is resized.</summary>
			/// <returns>A reference to the property.</returns>
			float& min_height();

			/// <summary>Get the minimum height permitted as widget is resized.</summary>
			/// <returns>The value of the property.</returns>
			float get_min_height() const;

			/// <summary>Set the minimum height permitted as widget is resized.</summary>
			/// <param name="min_height">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& min_height(const float& min_height);

			/// <summary>Get or set the maximum width permitted as widget is resized.</summary>
			/// <returns>A reference to the property.</returns>
			float& max_width();

			/// <summary>Get the maximum width permitted as widget is resized.</summary>
			/// <returns>The value of the property.</returns>
			float get_max_width() const;

			/// <summary>Set the maximum width permitted as widget is resized.</summary>
			/// <param name="max_width">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& max_width(const float& max_width);

			/// <summary>Get or set the maximum height permitted as widget is resized.</summary>
			/// <returns>A reference to the property.</returns>
			float& max_height();

			/// <summary>Get the maximum height permitted as widget is resized.</summary>
			/// <returns>The value of the property.</returns>
			float get_max_height() const;

			/// <summary>Set the maximum height permitted as widget is resized.</summary>
			/// <param name="max_height">The value of the property.</param>
			/// <returns>A reference to the modified object.</returns>
			resize_params& max_height(const float& max_height);

			bool operator==(const resize_params&);
			bool operator!=(const resize_params&);
		};

		/// <summary>RGBA color on a standard 0 to 255 scale. For the alpha channel 0 is transparent and 255 is opaque.</summary>
		class lecui_api color {
			unsigned short _red = 0;
			unsigned short _green = 0;
			unsigned short _blue = 0;
			unsigned short _alpha = 255;

		public:
			/// <summary>Color constructor. Makes opaque black (0, 0, 0, 255).</summary>
			color();

			/// <summary>Color constructor.</summary>
			/// <param name="red">The red channel, on a scale of 0 to 255.</param>
			/// <param name="green">The green channel, on a scale of 0 to 255.</param>
			/// <param name="blue">The blue channel, on a scale of 0 to 255.</param>
			/// <param name="alpha">The alpha channel, on a scale of 0 to 255.</param>
			color(const unsigned short red,
				const unsigned short green,
				const unsigned short blue,
				const unsigned short alpha);

			/// <summary>Get or set the red property.</summary>
			/// <returns>A reference to the property, from 0 to 255 inclusive.</returns>
			unsigned short& red();

			/// <summary>Get the red property.</summary>
			/// <returns>The property, from 0 to 255 inclusive.</returns>
			unsigned short get_red() const;

			/// <summary>Set the red property.</summary>
			/// <param name="red">The property, from 0 to 255 inclusive.</param>
			/// <returns>A reference to the modified object.</returns>
			color& red(const unsigned short& red);
			
			/// <summary>Get or set the green property.</summary>
			/// <returns>A reference to the property, from 0 to 255 inclusive.</returns>
			unsigned short& green();

			/// <summary>Get the green property.</summary>
			/// <returns>The property, from 0 to 255 inclusive.</returns>
			unsigned short get_green() const;

			/// <summary>Set the green property.</summary>
			/// <param name="green">The property, from 0 to 255 inclusive.</param>
			/// <returns>A reference to the modified object.</returns>
			color& green(const unsigned short& green);

			/// <summary>Get or set the blue property.</summary>
			/// <returns>A reference to the property, from 0 to 255 inclusive.</returns>
			unsigned short& blue();

			/// <summary>Get the blue property.</summary>
			/// <returns>The property, from 0 to 255 inclusive.</returns>
			unsigned short get_blue() const;

			/// <summary>Set the blue property.</summary>
			/// <param name="blue">The property, from 0 to 255 inclusive.</param>
			/// <returns>A reference to the modified object.</returns>
			color& blue(const unsigned short& blue);

			/// <summary>Get or set the alpha property.</summary>
			/// <returns>A reference to the property, from 0 to 255 inclusive.</returns>
			unsigned short& alpha();

			/// <summary>Get the alpha property.</summary>
			/// <returns>The property, from 0 to 255 inclusive.</returns>
			unsigned short get_alpha() const;

			/// <summary>Set the alpha property.</summary>
			/// <param name="alpha">The property, from 0 to 255 inclusive.</param>
			/// <returns>A reference to the modified object.</returns>
			color& alpha(const unsigned short& alpha);

			bool operator==(const color&);
			bool operator!=(const color&);
		};

		/// <summary>Sort options.</summary>
		enum class sort_options {
			/// <summary>Sort in ascending order.</summary>
			ascending,

			/// <summary>Sort in descending order.</summary>
			descending,

			/// <summary>No sorting order specified.</summary>
			none,
		};

		/// <summary>File type.</summary>
		struct file_type {
			/// <summary>The file extension, excluding the dot, e.g. "png".</summary>
			std::string extension = "png";

			/// <summary>The description of the file type, e.g. "PNG Image".</summary>
			std::string description = "PNG Image";
		};

		/// <summary>Open file parameters.</summary>
		struct open_file_params {
			/// <summary>The list of file types to open, as defined in <see cref="file_type"></see>.</summary>
			std::vector<file_type> file_types;

			/// <summary>Whether to include and "all supported file types" entry to allow viewing
			/// of all supported file types instead of being forced to select at least one.</summary>
			bool include_all_supported_types = true;

			/// <summary>The title of the dialog box, e.g. "Open Image".</summary>
			std::string title = "Open File";
		};

		/// <summary>Save file parameters.</summary>
		struct save_file_params {
			/// <summary>The list of file types to open, as defined in <see cref="file_type"></see>.</summary>
			std::vector<file_type> file_types;

			/// <summary>Whether to include and "all supported file types" entry to allow viewing
			/// of all supported file types instead of being forced to select at least one.</summary>
			bool include_all_supported_types = true;

			/// <summary>The title of the dialog box, e.g. "Save Document".</summary>
			std::string title = "Save File";
		};

		/// <summary>Table column.</summary>
		struct table_column {
			/// <summary>The name of the column. Has to be unique.</summary>
			std::string name;

			/// <summary>The width of the column, in pixels.</summary>
			unsigned short width = 80;

			/// <summary>The number of decimal places to round off the entries in this column to when displaying (if they are floats or doubles).</summary>
			int precision = 2;

			bool operator==(const table_column&);
			bool operator!=(const table_column&);
		};

		/// <summary>Table row; a list of (column, value) pairs. The map's "key" is
		/// the column name and it's "value" is the value under that column for the given row, e.g.
		/// { { "Name", "John" }, { "Surname", "Doe" }, { "Height", 172 } }
		/// </summary>
		using table_row = std::map<std::string, std::any>;

		/// <summary>Time class.</summary>
		class lecui_api time {
		public:
			/// <summary>The hour, from 0 to 23 inclusive.</summary>
			unsigned short hour = 0;

			/// <summary>The minute, from 0 to 59 inclusive.</summary>
			unsigned short minute = 0;

			/// <summary>The second, from 0 to 59 inclusive.</summary>
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

		/// <summary>Date class.</summary>
		struct date {
			/// <summary>The day of the month.</summary>
			unsigned short day = 1;

			/// <summary>The month of the year, from 1 to 12 inclusive.</summary>
			unsigned short month = 1;

			/// <summary>The year.</summary>
			unsigned short year = 2020;
		};

		/// <summary>Image render quality. Used when resampling images.</summary>
		enum class image_quality {
			/// <summary>Low image quality, for faster rendering.</summary>
			low,

			/// <summary>Medium image quality, for a balance between quality and rendering speed.</summary>
			medium,

			/// <summary>High image quality, for best image quality possibly at the cost of rendering speed.</summary>
			high,
		};

		/// <summary>Helper class for extracting values from a std::any. Strictly added to
		/// enable more terse code and make the code more readable.</summary>
		/// <remarks>If the std::any actually contains a different data type than what you expect
		/// the methods in this class will throw, so make sure to use a try-catch block in the calling
		/// code.</remarks>
		class lecui_api get {
		public:
			/// <summary>Extract integer value.</summary>
			/// <param name="value">The std::any containing the value.</param>
			/// <returns>The integer.</returns>
			static int integer(const std::any& value);

			/// <summary>Extract double value.</summary>
			/// <param name="value">The std::any containing the value.</param>
			/// <returns>The double.</returns>
			static double real(const std::any& value);

			/// <summary>Extract text.</summary>
			/// <param name="value">The std::any containing the text.</param>
			/// <returns>The text.</returns>
			static std::string text(const std::any& value);
		};
	}
}
