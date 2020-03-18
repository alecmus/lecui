/*
** form_common.h - common form functions
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

#include "../form.h"

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

static inline void log(const std::string& string) {
#ifdef _DEBUG
	std::string string_ = "-->" + string + "\n";
	printf(string_.c_str());
	OutputDebugStringA(string_.c_str());
#endif
}

template<class Interface>
static inline void safe_release(Interface** pp_interface_to_release) {
	if (*pp_interface_to_release != nullptr) {
		(*pp_interface_to_release)->Release();
		(*pp_interface_to_release) = nullptr;
	}
}

/// <summary>Adjust a rectangle to a given DPI scale.</summary>
/// <param name="rc">The coordinates of the rectangle under 96dpi.</param>
/// <param name="DPIScale">The ratio of the current dpi to 96dpi.</param>
/// <remarks>DPIScale is 1.0 when the current setting is 96dpi.</remarks>
static inline void scale_RECT(RECT& rc, const float& DPIScale) {
	int iUnscaledW = rc.right - rc.left;
	int iUnscaledH = rc.bottom - rc.top;

	rc.left = int(0.5f + rc.left * DPIScale);
	rc.top = int(0.5f + rc.top * DPIScale);
	rc.right = rc.left + int(0.5f + iUnscaledW * DPIScale);
	rc.bottom = rc.top + int(0.5f + iUnscaledH * DPIScale);
}

/// <summary>Adjust a rectangle to a given DPI scale.</summary>
/// <param name="rc">The coordinates of the rectangle under 96dpi.</param>
/// <param name="DPIScale">The ratio of the current dpi to 96dpi.</param>
/// <remarks>DPIScale is 1.0 when the current setting is 96dpi.</remarks>
static inline void scale_RECT(D2D1_RECT_F& rc, const float& DPIScale) {
	float iUnscaledW = rc.right - rc.left;
	float iUnscaledH = rc.bottom - rc.top;

	rc.left = rc.left * DPIScale;
	rc.top = rc.top * DPIScale;
	rc.right = rc.left + iUnscaledW * DPIScale;
	rc.bottom = rc.top + iUnscaledH * DPIScale;
}

/// <summary>Adjust a rectangle to a scale of 96dpi.</summary>
/// <param name="rc">The coordinates of the rectangle under the current dpi.</param>
/// <param name="DPIScale">The ratio of the current dpi to 96dpi.</param>
/// <remarks>DPIScale is 1.0 when the current setting is 96dpi.</remarks>
static inline void unscale_RECT(RECT& rc, const float& DPIScale) {
	int iScaledW = rc.right - rc.left;
	int iScaledH = rc.bottom - rc.top;

	rc.left = int(0.5f + rc.left / DPIScale);
	rc.top = int(0.5f + rc.top / DPIScale);
	rc.right = rc.left + int(0.5f + iScaledW / DPIScale);
	rc.bottom = rc.top + int(0.5f + iScaledH / DPIScale);
}

/// <summary>Adjust a rectangle to a scale of 96dpi.</summary>
/// <param name="rc">The coordinates of the rectangle under the current dpi.</param>
/// <param name="DPIScale">The ratio of the current dpi to 96dpi.</param>
/// <remarks>DPIScale is 1.0 when the current setting is 96dpi.</remarks>
static inline void unscale_RECT(D2D1_RECT_F& rc, const float& DPIScale) {
	float iScaledW = rc.right - rc.left;
	float iScaledH = rc.bottom - rc.top;

	rc.left = rc.left / DPIScale;
	rc.top = rc.top / DPIScale;
	rc.right = rc.left + iScaledW / DPIScale;
	rc.bottom = rc.top + iScaledH / DPIScale;
}

static inline std::wstring convert_string(const std::string& string_) {
	int s_length = (int)string_.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, string_.c_str(), s_length, 0, 0);

	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, string_.c_str(), s_length, buf, len);

	std::wstring r(buf);
	delete[] buf;

	return r;
}

static inline std::string convert_string(const std::wstring& string_) {
	int s_length = (int)string_.length() + 1;
	int len = WideCharToMultiByte(CP_ACP, 0, string_.c_str(), s_length, 0, 0, 0, 0);

	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, string_.c_str(), s_length, buf, len, 0, 0);

	std::string r(buf);
	delete[] buf;

	return r;
}

static inline D2D1::ColorF convert_color(const liblec::lecui::color& color_) {
	return D2D1::ColorF(static_cast<float>(color_.red) / 255.0f,
		static_cast<float>(color_.green) / 255.0f,
		static_cast<float>(color_.blue) / 255.0f,
		static_cast<float>(color_.alpha) / 255.0f
	);
}

static inline float convert_fontsize_to_dip(const float& points) {
	return points * 96.0f / 72.0f;
}

/// <summary>position rect within rect_reference,
/// perc_h% horizontally and perc_v% vertically</summary>
static inline void pos_rect(const D2D1_RECT_F& rect_reference,
	D2D1_RECT_F& rect, const float& perc_h, const float& perc_v) {
	D2D1_RECT_F rcIn = rect;

	float iDeltaX = (rect_reference.right - rect_reference.left) - (rcIn.right - rcIn.left);
	rect.left = rect_reference.left + (perc_h * iDeltaX) / 100.f;

	float iDeltaY = (rect_reference.bottom - rect_reference.top) - (rcIn.bottom - rcIn.top);
	rect.top = rect_reference.top + (perc_v * iDeltaY) / 100.f;

	rect.right = rect.left + (rcIn.right - rcIn.left);
	rect.bottom = rect.top + (rcIn.bottom - rcIn.top);
}

/// <summary>position D within C to reflect how B is positioned in A.</summary>
static inline void position_h_scrollbar(const D2D1_RECT_F& rectA,
	const D2D1_RECT_F& rectB, const D2D1_RECT_F& rectC, D2D1_RECT_F& rectD) {
	const float A_width = rectA.right - rectA.left;
	const float B_width = rectB.right - rectB.left;
	const float C_width = rectC.right - rectC.left;

	// if both A and B have 0.f width set to 1.f
	// if A has width 0.f but not B set to 0.f
	const float width_ratio = A_width == 0.f ? (B_width == 0.f ? 1.f : 0.f) : B_width / A_width;

	const float width_d = C_width * width_ratio;
	rectD.left = 0.f;
	rectD.right = rectD.left + width_d;
	rectD.top = rectC.top;
	rectD.bottom = rectC.bottom;

	// compute how far B is along A
	auto perc_b_along_a = [&]() {
		const float start_pos = rectA.left + ((rectB.right - rectB.left) / 2.f);
		const float end_pos = rectA.right - ((rectB.right - rectB.left) / 2.f);
		const float center_B = rectB.left + ((rectB.right - rectB.left) / 2.f);

		if (end_pos == start_pos)
			return 100.f;
		else
			return 100.f * (center_B - start_pos) / (end_pos - start_pos);
	};

	// position C within D
	pos_rect(rectC, rectD, perc_b_along_a(), 0.f);
}

/// <summary>position D within C to reflect how B is positioned in A.</summary>
static inline void position_v_scrollbar(const D2D1_RECT_F& rectA,
	const D2D1_RECT_F& rectB, const D2D1_RECT_F& rectC, D2D1_RECT_F& rectD) {
	const float A_height = rectA.bottom - rectA.top;
	const float B_height = rectB.bottom - rectB.top;
	const float C_height = rectC.bottom - rectC.top;

	// if both A and B have 0.f height set to 1.f
	// if A has height 0.f but not B set to 0.f
	const float height_ratio = A_height == 0.f ? (B_height == 0.f ? 1.f : 0.f) : B_height / A_height;

	const float height_d = C_height * height_ratio;
	rectD.top = 0.f;
	rectD.bottom = rectD.top + height_d;
	rectD.left = rectC.left;
	rectD.right = rectC.right;

	// compute how far B is along A
	auto perc_b_along_a = [&]() {
		const float start_pos = rectA.top + ((rectB.bottom - rectB.top) / 2.f);
		const float end_pos = rectA.bottom - ((rectB.bottom - rectB.top) / 2.f);
		const float center_B = rectB.top + ((rectB.bottom - rectB.top) / 2.f);

		if (end_pos == start_pos)
			return 100.f;
		else
			return 100.f * (center_B - start_pos) / (end_pos - start_pos);
	};

	// position C within D
	pos_rect(rectC, rectD, 0.f, perc_b_along_a());
}

static inline D2D1_RECT_F convert_rect(const liblec::lecui::rect& rect) {
	D2D1_RECT_F rect_;
	rect_.left = static_cast<float>(rect.left);
	rect_.top = static_cast<float>(rect.top);
	rect_.right = static_cast<float>(rect.right);
	rect_.bottom = static_cast<float>(rect.bottom);
	return rect_;
}

static inline liblec::lecui::rect convert_rect(const D2D1_RECT_F& rect) {
	liblec::lecui::rect rect_;
	rect_.left = static_cast<long>(rect.left);
	rect_.top = static_cast<long>(rect.top);
	rect_.right = static_cast<long>(rect.right);
	rect_.bottom = static_cast<long>(rect.bottom);
	return rect_;
}

/// <summary>Rounding off class.</summary>
class roundoff {
public:
	/// <summary>Round-off a double to a string.</summary>
	/// <param name="d">The double to round-off.</param>
	/// <param name="precision">The number of decimal places to round it off to.</param>
	/// <returns>The rounded-off value, as a string.</returns>
	template <typename T>
	static std::basic_string<T> tostr(const double& d, int precision) {
		std::basic_stringstream<T> ss;
		ss << std::fixed;
		ss.precision(precision);
		ss << d;
		return ss.str();
	}

	/// <summary>Round-off a double to another double.</summary>
	/// <param name="d">The double to round-off.</param>
	/// <param name="precision">The number of decimal places to round it off to.</param>
	/// <returns>The rounded-off value.</returns>
	static double tod(const double& d, int precision) {
		int y = (int)d;
		double z = d - (double)y;
		double m = pow(10, precision);
		double q = z * m;
		double r = round(q);

		return static_cast<double>(y) + (1.0 / m) * r;
	}

	/// <summary>Round-off a float to another float.</summary>
	/// <param name="d">The float to round-off.</param>
	/// <param name="precision">The number of decimal places to round it off to.</param>
	/// <returns>The rounded-off value.</returns>
	static float tof(const float& f, int precision) {
		return static_cast<float>(tod(static_cast<double>(f), precision));
	}
};

static inline void make_single_line(IDWriteFactory* p_directwrite_factory,
	IDWriteTextFormat* p_text_format) {
	// to-do: verify this function's correctness
	p_text_format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	DWRITE_TRIMMING trimming;
	trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
	trimming.delimiter = '.';

	IDWriteInlineObject* trimmingSign = nullptr;
	p_directwrite_factory->CreateEllipsisTrimmingSign(p_text_format, &trimmingSign);
	p_text_format->SetTrimming(&trimming, trimmingSign);
}

template <typename T>
static inline T smallest(T a, T b) {
	return (((a) < (b)) ? (a) : (b));
}

template <typename T>
static inline T largest(T a, T b) {
	return (((a) > (b)) ? (a) : (b));
}

static inline void show_selected(ID2D1HwndRenderTarget* p_render_target,
	ID2D1SolidColorBrush* p_brush_selected,
	const D2D1_RECT_F& rect,
	const bool& pressed) {
	const float margin = pressed ? -1.f : -.5f;
	D2D1_RECT_F rect_selection_ = rect;
	rect_selection_.left -= margin;
	rect_selection_.top -= margin;
	rect_selection_.right += margin;
	rect_selection_.bottom += margin;
	p_render_target->DrawRectangle(rect_selection_, p_brush_selected, 0.f - margin);
}

static inline liblec::lecui::color lighten_color(const liblec::lecui::color& clr_in,
	unsigned short percentage) {
	liblec::lecui::color color = clr_in;
	color.red = clr_in.red + (((255 - clr_in.red) * percentage) / 100);
	color.green = clr_in.green + (((255 - clr_in.green) * percentage) / 100);
	color.blue = clr_in.blue + (((255 - clr_in.blue) * percentage) / 100);
	return color;
}

static inline liblec::lecui::color darken_color(const liblec::lecui::color& clr_in,
	unsigned short percentage) {
	liblec::lecui::color color = clr_in;
	color.red = ((clr_in.red - 0) * (100 - percentage)) / 100;
	color.green = ((clr_in.green - 0) * (100 - percentage)) / 100;
	color.blue = ((clr_in.blue - 0) * (100 - percentage)) / 100;
	return color;
}

class auto_clip {
public:
	auto_clip(bool render,
		ID2D1HwndRenderTarget* p_render_target,
		const D2D1_RECT_F& rect,
		float tolerance) :
		render_(render),
		p_render_target_(p_render_target),
		rect_({ rect.left - tolerance, rect.top - tolerance, rect.right + tolerance, rect.bottom + tolerance }) {
		if (render)
			p_render_target_->PushAxisAlignedClip(rect_,
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	~auto_clip() {
		if (render_)
			p_render_target_->PopAxisAlignedClip();
	}

private:
	bool render_;
	ID2D1HwndRenderTarget* p_render_target_;
	D2D1_RECT_F rect_;
};
