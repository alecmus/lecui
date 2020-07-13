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

#include "form.h"

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace liblec {
	namespace lecui {
		static inline void log(const std::string& string) {
#if defined(_DEBUG)
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

			rc.left = int(.5f + rc.left * DPIScale);
			rc.top = int(.5f + rc.top * DPIScale);
			rc.right = rc.left + int(.5f + iUnscaledW * DPIScale);
			rc.bottom = rc.top + int(.5f + iUnscaledH * DPIScale);
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

			rc.left = int(.5f + rc.left / DPIScale);
			rc.top = int(.5f + rc.top / DPIScale);
			rc.right = rc.left + int(.5f + iScaledW / DPIScale);
			rc.bottom = rc.top + int(.5f + iScaledH / DPIScale);
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

		static inline D2D1::ColorF convert_color(const color& color_) {
			return D2D1::ColorF(static_cast<float>(color_.red) / 255.f,
				static_cast<float>(color_.green) / 255.f,
				static_cast<float>(color_.blue) / 255.f,
				static_cast<float>(color_.alpha) / 255.f
				);
		}

		static inline float convert_fontsize_to_dip(const float& points) {
			return points * 96.f / 72.f;
		}

		/// <summary>position rect_subject within rect_reference,
		/// perc_h% horizontally and perc_v% vertically</summary>
		template <class t>
		static inline void pos_rect(const t& rect_reference,
			t& rect_subject, const float& perc_h, const float& perc_v) {
			t rect_original = rect_subject;

			const auto delta_x = (rect_reference.right - rect_reference.left) - (rect_original.right - rect_original.left);
			rect_subject.left = rect_reference.left + (perc_h * delta_x) / 100.f;

			const auto delta_y = (rect_reference.bottom - rect_reference.top) - (rect_original.bottom - rect_original.top);
			rect_subject.top = rect_reference.top + (perc_v * delta_y) / 100.f;

			rect_subject.right = rect_subject.left + (rect_original.right - rect_original.left);
			rect_subject.bottom = rect_subject.top + (rect_original.bottom - rect_original.top);
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

		static inline D2D1_RECT_F convert_rect(const rect& rect) {
			D2D1_RECT_F rect_;
			rect_.left = rect.left;
			rect_.top = rect.top;
			rect_.right = rect.right;
			rect_.bottom = rect.bottom;
			return rect_;
		}

		static inline rect convert_rect(const D2D1_RECT_F& rect) {
			lecui::rect rect_;
			rect_.left = rect.left;
			rect_.top = rect.top;
			rect_.right = rect.right;
			rect_.bottom = rect.bottom;
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
			DWRITE_TRIMMING trimming;
			trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
			trimming.delimiter = 0;
			trimming.delimiterCount = 0;

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

		template <class t>
		static inline void swap(t& left, t& right) {
			t temp = left;
			left = right;
			right = temp;
		}

		static inline color lighten_color(const color& clr_in,
			unsigned short percentage) {
			color color = clr_in;
			color.red = clr_in.red + (((255 - clr_in.red) * percentage) / 100);
			color.green = clr_in.green + (((255 - clr_in.green) * percentage) / 100);
			color.blue = clr_in.blue + (((255 - clr_in.blue) * percentage) / 100);
			return color;
		}

		static inline color darken_color(const color& clr_in,
			unsigned short percentage) {
			color color = clr_in;
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
				rect_({ rect.left - tolerance, rect.top - tolerance,
					rect.right + tolerance, rect.bottom + tolerance }) {
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

		static inline HRESULT load_bitmap_resource(ID2D1RenderTarget* pRenderTarget,
			IWICImagingFactory* pIWICFactory, HINSTANCE hInst, int id_image,
			std::string resource_type, ID2D1Bitmap** ppBitmap) {
			IWICBitmapDecoder* pDecoder = NULL;
			IWICBitmapFrameDecode* pSource = NULL;
			IWICStream* pStream = NULL;
			IWICFormatConverter* pConverter = NULL;

			HRSRC imageResHandle = NULL;
			HGLOBAL imageResDataHandle = NULL;
			void* pImageFile = NULL;
			DWORD imageFileSize = 0;

			// Locate the resource.
			imageResHandle = FindResourceA(hInst, MAKEINTRESOURCEA(id_image), resource_type.c_str());
			HRESULT hr = imageResHandle ? S_OK : E_FAIL;
			if (SUCCEEDED(hr)) {
				// Load the resource.
				imageResDataHandle = LoadResource(hInst, imageResHandle);
				hr = imageResDataHandle ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr)) {
				// Lock it to get a system memory pointer.
				pImageFile = LockResource(imageResDataHandle);
				hr = pImageFile ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr)) {
				// Calculate the size.
				imageFileSize = SizeofResource(hInst, imageResHandle);
				hr = imageFileSize ? S_OK : E_FAIL;
			}
			if (SUCCEEDED(hr)) {
				// Create a WIC stream to map onto the memory.
				hr = pIWICFactory->CreateStream(&pStream);
			}
			if (SUCCEEDED(hr)) {
				// Initialize the stream with the memory pointer and size.
				hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);
			}
			if (SUCCEEDED(hr)) {
				// Create a decoder for the stream.
				hr = pIWICFactory->CreateDecoderFromStream(pStream, NULL,
					WICDecodeMetadataCacheOnLoad, &pDecoder);
			}
			if (SUCCEEDED(hr)) {
				// Create the initial frame.
				hr = pDecoder->GetFrame(0, &pSource);
			}
			if (SUCCEEDED(hr)) {
				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pIWICFactory->CreateFormatConverter(&pConverter);
			}
			if (SUCCEEDED(hr)) {
				hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
			}
			if (SUCCEEDED(hr)) {
				//create a Direct2D bitmap from the WIC bitmap.
				hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
			}

			safe_release(&pDecoder);
			safe_release(&pSource);
			safe_release(&pStream);
			safe_release(&pConverter);
			return hr;
		}

		static inline HRESULT load_bitmap_file(ID2D1RenderTarget* pRenderTarget,
			IWICImagingFactory* pIWICFactory, PCWSTR uri, ID2D1Bitmap** ppBitmap) {
			IWICBitmapDecoder* pDecoder = NULL;
			IWICBitmapFrameDecode* pSource = NULL;
			IWICFormatConverter* pConverter = NULL;

			HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ,
				WICDecodeMetadataCacheOnLoad, &pDecoder);

			if (SUCCEEDED(hr)) {
				// Create the initial frame.
				hr = pDecoder->GetFrame(0, &pSource);
			}

			if (SUCCEEDED(hr)) {
				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pIWICFactory->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr)) {
				hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
			}

			if (SUCCEEDED(hr)) {
				// Create a Direct2D bitmap from the WIC bitmap.
				hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
			}

			safe_release(&pDecoder);
			safe_release(&pSource);
			safe_release(&pConverter);
			return hr;
		}

		/// <summary>
		/// Fit a rectangle within another.
		/// </summary>
		/// <param name="rect_container">The container rectangle.</param>
		/// <param name="stretch">Whether to stretch the bitmap to the supplied dimensions.</param>
		/// <param name="enlarge_if_smaller">Enlarge if it's smaller than the container.</param>
		/// <param name="center">Whether to center the rectangle in the container.</param>
		/// <param name="rect"></param>
		static inline void fit_rect(const D2D1_RECT_F rect_container, D2D1_RECT_F& rect,
			bool stretch, bool enlarge_if_smaller, bool center) {
			auto width = rect_container.right - rect_container.left;
			auto height = rect_container.bottom - rect_container.top;

			// deduce old height and ratio
			const auto old_height = rect.bottom - rect.top;
			const auto old_width = rect.right - rect.left;
			const auto ratio = old_width / old_height;

			if (enlarge_if_smaller == false) {
				if (old_width < width && old_height < height) {
					// both sides of the rectangle are smaller than the container, preserve size
					width = old_width;
					height = old_height;
				}
			}

			// save target dimensions
			const auto control_w = width;
			const auto control_h = height;

			if (ratio == 1) {
				if (!stretch) {
					if (width > height)
						width = height;	// landscape
					else
						height = width;	// portrait
				}
			}
			else {
				if (!stretch) {
					// adjust either new width or height to keep aspect ratio
					if (old_width > old_height) {
						// old width is greater than old height
						// adjust new height using new width to keep aspect ratio
						height = width / ratio;

						if (height > control_h) {
							// new width is greater than target width, adjust it accordingly
							height = control_h;
							width = height * ratio;
						}
					}
					else {
						// old height is greater than old width
						// adjust new width using new height to keep aspect ratio
						width = height * ratio;

						if (width > control_w) {
							// new width is greater than target width, adjust it accordingly
							width = control_w;
							height = width / ratio;
						}
					}
				}
			}

			rect.left = 0.f;
			rect.top = 0.f;

			if (center) {
				rect.left = ((rect_container.right - rect_container.left) - width) / 2.f;
				rect.top = ((rect_container.bottom - rect_container.top) - height) / 2.f;
			}

			rect.left += rect_container.left;
			rect.top += rect_container.top;
			rect.right = rect.left + width;
			rect.bottom = rect.top + height;
		}

		static inline float get_process_dpi() {
			// make the process DPI aware, if it isn't already
			if (!IsProcessDPIAware()) {
				if (!SetProcessDPIAware())
					log("This program is not DPI aware. As a result, UI elements may not be clear.");
			}

			// capture current DPI scale
			HDC hdc_screen = GetDC(NULL);
			auto dpi_scale_ = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
			ReleaseDC(NULL, hdc_screen);

			return dpi_scale_;
		}
	}
}
