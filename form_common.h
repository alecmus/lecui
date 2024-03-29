//
// form_common.h - common form functions
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "form.h"

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#include <iostream>
#include <sstream>

namespace liblec {
	namespace lecui {
		static inline void log(const std::string& string) {
#if defined(_DEBUG)
			std::string _string = "-->" + string + "\n";
			printf(_string.c_str());
			OutputDebugStringA(_string.c_str());
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

		static inline std::wstring convert_string(const std::string& input) {
			int s_length = (int)input.length() + 1;
			int len = MultiByteToWideChar(CP_ACP, 0, input.c_str(), s_length, 0, 0);

			wchar_t* buffer = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, input.c_str(), s_length, buffer, len);

			std::wstring output(buffer);
			delete[] buffer;

			return output;
		}

		static inline std::string convert_string(const std::wstring& input) {
			int s_length = (int)input.length() + 1;
			int len = WideCharToMultiByte(CP_ACP, 0, input.c_str(), s_length, 0, 0, 0, 0);

			char* buffer = new char[len];
			WideCharToMultiByte(CP_ACP, 0, input.c_str(), s_length, buffer, len, 0, 0);

			std::string output(buffer);
			delete[] buffer;

			return output;
		}

		static inline D2D1::ColorF convert_color(const color& _color) {
			return D2D1::ColorF(static_cast<float>(_color.get_red()) / 255.f,
				static_cast<float>(_color.get_green()) / 255.f,
				static_cast<float>(_color.get_blue()) / 255.f,
				static_cast<float>(_color.get_alpha()) / 255.f
				);
		}

		static inline float convert_fontsize_to_dip(const float& points) {
			return points * 96.f / 72.f;
		}

		/// <summary>position rect_subject within rect_reference,
		/// perc_h% horizontally and perc_v% vertically</summary>
		static inline void pos_rect(const D2D1_RECT_F& rect_reference,
			D2D1_RECT_F& rect_subject, const float& perc_h, const float& perc_v) {
			D2D1_RECT_F rect_original = rect_subject;

			const auto delta_x = (rect_reference.right - rect_reference.left) - (rect_original.right - rect_original.left);
			rect_subject.left = rect_reference.left + (perc_h * delta_x) / 100.f;

			const auto delta_y = (rect_reference.bottom - rect_reference.top) - (rect_original.bottom - rect_original.top);
			rect_subject.top = rect_reference.top + (perc_v * delta_y) / 100.f;

			rect_subject.right = rect_subject.left + (rect_original.right - rect_original.left);
			rect_subject.bottom = rect_subject.top + (rect_original.bottom - rect_original.top);
		}

		/// <summary>position rect_subject within rect_reference,
		/// perc_h% horizontally and perc_v% vertically</summary>
		static inline void pos_rect(const lecui::rect& rect_reference,
			lecui::rect& rect_subject, const float& perc_h, const float& perc_v) {
			lecui::rect rect_original = rect_subject;

			const auto delta_x = (rect_reference.get_right() - rect_reference.get_left()) - (rect_original.right() - rect_original.left());
			rect_subject.left() = rect_reference.get_left() + (perc_h * delta_x) / 100.f;

			const auto delta_y = (rect_reference.get_bottom() - rect_reference.get_top()) - (rect_original.bottom() - rect_original.top());
			rect_subject.top(rect_reference.get_top() + (perc_v * delta_y) / 100.f);

			rect_subject.right(rect_subject.left() + (rect_original.right() - rect_original.left()));
			rect_subject.bottom(rect_subject.top() + (rect_original.bottom() - rect_original.top()));
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
			D2D1_RECT_F _rect;
			_rect.left = rect.get_left();
			_rect.top = rect.get_top();
			_rect.right = rect.get_right();
			_rect.bottom = rect.get_bottom();
			return _rect;
		}

		static inline rect convert_rect(const D2D1_RECT_F& rect) {
			lecui::rect _rect;
			_rect.left(rect.left);
			_rect.top(rect.top);
			_rect.right(rect.right);
			_rect.bottom(rect.bottom);
			return _rect;
		}

		/// <summary>Rounding off class.</summary>
		class round_off {
		public:
			/// <summary>Round-off a double to a string.</summary>
			/// <param name="d">The double to round-off.</param>
			/// <param name="precision">The number of decimal places to round it off to.</param>
			/// <returns>The rounded-off value, as a string.</returns>
			static std::string to_string(const double& d, int precision) {
				std::stringstream ss;
				ss << std::fixed;
				ss.precision(precision);
				ss << d;
				return ss.str();
			}

			/// <summary>Round-off a double to another double.</summary>
			/// <param name="d">The double to round-off.</param>
			/// <param name="precision">The number of decimal places to round it off to.</param>
			/// <returns>The rounded-off value.</returns>
			static double to_double(const double& d, int precision) {
				int y = (int)d;
				double z = d - (double)y;
				double m = pow(10.0, (double)precision);
				double q = z * m;
				double r = round(q);

				return static_cast<double>(y) + (1.0 / m) * r;
			}

			/// <summary>Round-off a float to another float.</summary>
			/// <param name="d">The float to round-off.</param>
			/// <param name="precision">The number of decimal places to round it off to.</param>
			/// <returns>The rounded-off value.</returns>
			static float to_float(const float& f, int precision) {
				int y = (int)f;
				float z = f - (float)y;
				float m = pow(10.f, (float)precision);
				float q = z * m;
				float r = round(q);

				return static_cast<float>(y) + (1.f / m) * r;
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
			color.red(clr_in.get_red() + (((255 - clr_in.get_red()) * percentage) / 100));
			color.green(clr_in.get_green() + (((255 - clr_in.get_green()) * percentage) / 100));
			color.blue(clr_in.get_blue() + (((255 - clr_in.get_blue()) * percentage) / 100));
			return color;
		}

		static inline color darken_color(const color& clr_in,
			unsigned short percentage) {
			color color = clr_in;
			color.red(((clr_in.get_red() - 0) * (100 - percentage)) / 100);
			color.green(((clr_in.get_green() - 0) * (100 - percentage)) / 100);
			color.blue(((clr_in.get_blue() - 0) * (100 - percentage)) / 100);
			return color;
		}

		class auto_clip {
		public:
			auto_clip(bool render,
				ID2D1HwndRenderTarget* p_render_target,
				const D2D1_RECT_F& rect,
				float content_margin) :
				_render(render),
				_p_render_target(p_render_target),
				_rect({ rect.left - content_margin, rect.top - content_margin,
					rect.right + content_margin, rect.bottom + content_margin }) {
				if (render)
					_p_render_target->PushAxisAlignedClip(_rect,
						D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			}

			~auto_clip() {
				if (_render)
					_p_render_target->PopAxisAlignedClip();
			}

		private:
			bool _render;
			ID2D1HwndRenderTarget* _p_render_target;
			D2D1_RECT_F _rect;
		};

		/// <summary>Fit a rectangle within another.</summary>
		/// <param name="rect_container">The container rectangle.</param>
		/// <param name="enlarge_if_smaller">Enlarge if it's smaller than the container.</param>
		/// <param name="keep_aspect_ratio">Whether to keep the source image's aspect ratio.</param>
		/// <param name="center">Whether to center the rectangle in the container.</param>
		/// <param name="rect"></param>
		static inline void fit_rect(const D2D1_RECT_F rect_container, D2D1_RECT_F& rect,
			bool enlarge_if_smaller, bool keep_aspect_ratio, bool center) {
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
				if (keep_aspect_ratio) {
					if (width > height)
						width = height;	// landscape
					else
						height = width;	// portrait
				}
			}
			else {
				if (keep_aspect_ratio) {
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

		static inline void create_bitmap(ID2D1RenderTarget* p_render_target,
			IWICImagingFactory* p_IWICFactory, IWICBitmapDecoder* p_decoder,
			ID2D1Bitmap** pp_bitmap, size target_size,
			bool enlarge_if_smaller, bool keep_aspect_ratio, image_quality quality) {
			IWICBitmapFrameDecode* pSource = nullptr;
			IWICFormatConverter* pConverter = nullptr;
			IWICBitmapScaler* pScaler = nullptr;

			// Create the initial frame.
			HRESULT hr = p_decoder->GetFrame(0, &pSource);

			if (SUCCEEDED(hr)) {
				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = p_IWICFactory->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr)) {
				UINT32 original_width = 0;
				UINT32 original_height = 0;
				hr = pSource->GetSize(&original_width, &original_height);
				if (SUCCEEDED(hr)) {
					D2D1_RECT_F rect_container = { 0, 0, 0, 0 };
					rect_container.right = rect_container.left + target_size.width();
					rect_container.bottom = rect_container.top + target_size.height();
					D2D1_RECT_F rect = { 0, 0, 0, 0 };
					rect.right = rect.left + static_cast<float>(original_width);
					rect.bottom = rect.top + static_cast<float>(original_height);
					fit_rect(rect_container, rect, enlarge_if_smaller, keep_aspect_ratio, false);
					auto new_width = static_cast<UINT>(rect.right - rect.left);
					auto new_height = static_cast<UINT>(rect.bottom - rect.top);

					if (new_width != original_width || new_height != original_height) {
						// scale image
						hr = p_IWICFactory->CreateBitmapScaler(&pScaler);

						if (SUCCEEDED(hr)) {
							WICBitmapInterpolationMode mode;
							switch (quality) {
							case image_quality::low:
								mode = WICBitmapInterpolationModeNearestNeighbor;
								break;
							case image_quality::high:
								mode = WICBitmapInterpolationModeHighQualityCubic;
								break;
							case image_quality::medium:
							default:
								mode = WICBitmapInterpolationModeLinear;
								break;
							}
							hr = pScaler->Initialize(pSource, new_width, new_height, mode);
						}
						if (SUCCEEDED(hr)) {
							hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA,
								WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
						}
					}
					else {
						// don't scale image
						hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
							WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
					}
				}
			}

			if (SUCCEEDED(hr)) {
				// Create a Direct2D bitmap from the WIC bitmap.
				hr = p_render_target->CreateBitmapFromWicBitmap(pConverter, nullptr, pp_bitmap);
			}

			safe_release(&pScaler);
			safe_release(&pConverter);
			safe_release(&pSource);
		}

		static inline HRESULT load_bitmap_resource(ID2D1RenderTarget* p_render_target,
			IWICImagingFactory* p_IWICFactory, HINSTANCE h_inst, int id_image,
			std::string resource_type, ID2D1Bitmap** pp_bitmap,
			size target_size, bool enlarge_if_smaller, bool keep_aspect_ratio, image_quality quality) {
			IWICBitmapDecoder* p_decoder = nullptr;
			IWICStream* p_stream = nullptr;

			HRSRC image_resource_handle = NULL;
			HGLOBAL image_resource_data_handle = NULL;
			void* p_image_file = nullptr;
			DWORD image_file_size = 0;

			// Locate the resource.
			image_resource_handle = FindResourceA(h_inst, MAKEINTRESOURCEA(id_image), resource_type.c_str());
			HRESULT hr = image_resource_handle ? S_OK : E_FAIL;
			if (SUCCEEDED(hr)) {
				// Load the resource.
				image_resource_data_handle = LoadResource(h_inst, image_resource_handle);
				hr = image_resource_data_handle ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr)) {
				// Lock it to get a system memory pointer.
				p_image_file = LockResource(image_resource_data_handle);
				hr = p_image_file ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr)) {
				// Calculate the size.
				image_file_size = SizeofResource(h_inst, image_resource_handle);
				hr = image_file_size ? S_OK : E_FAIL;
			}
			if (SUCCEEDED(hr)) {
				// Create a WIC stream to map onto the memory.
				hr = p_IWICFactory->CreateStream(&p_stream);
			}
			if (SUCCEEDED(hr)) {
				// Initialize the stream with the memory pointer and size.
				hr = p_stream->InitializeFromMemory(reinterpret_cast<BYTE*>(p_image_file), image_file_size);
			}
			if (SUCCEEDED(hr)) {
				// Create a decoder for the stream.
				hr = p_IWICFactory->CreateDecoderFromStream(p_stream, nullptr,
					WICDecodeMetadataCacheOnLoad, &p_decoder);
			}

			if (SUCCEEDED(hr))
				create_bitmap(p_render_target, p_IWICFactory, p_decoder,
					pp_bitmap, target_size, enlarge_if_smaller, keep_aspect_ratio, quality);

			safe_release(&p_decoder);
			safe_release(&p_stream);
			return hr;
		}

		static inline HRESULT load_bitmap_file(ID2D1RenderTarget* p_render_target,
			IWICImagingFactory* p_IWICFactory, PCWSTR uri, ID2D1Bitmap** pp_bitmap,
			size target_size, bool enlarge_if_smaller, bool keep_aspect_ratio, image_quality quality) {
			IWICBitmapDecoder* p_decoder = nullptr;

			HRESULT hr = p_IWICFactory->CreateDecoderFromFilename(uri, nullptr, GENERIC_READ,
				WICDecodeMetadataCacheOnLoad, &p_decoder);

			if (SUCCEEDED(hr))
				create_bitmap(p_render_target, p_IWICFactory, p_decoder,
					pp_bitmap, target_size, enlarge_if_smaller, keep_aspect_ratio, quality);

			safe_release(&p_decoder);
			return hr;
		}

		static inline float get_process_dpi() {
			// make the process DPI aware, if it isn't already
			if (!IsProcessDPIAware()) {
				if (!SetProcessDPIAware())
					log("This program is not DPI aware. As a result, UI elements may not be clear.");
			}

			// capture current DPI scale
			HDC hdc_screen = GetDC(NULL);
			auto _dpi_scale = (float)GetDeviceCaps(hdc_screen, LOGPIXELSY) / 96.0f;
			ReleaseDC(NULL, hdc_screen);

			return _dpi_scale;
		}

		static inline void trim(std::string& s) {
			auto trim_left = [](std::string& s) {
				s.erase(s.begin(), std::find_if(s.begin(), s.end(),
					[](unsigned char ch) { return !std::isspace(ch); }));
			};

			auto trim_right = [](std::string& s) {
				s.erase(std::find_if(s.rbegin(), s.rend(),
					[](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
			};

			trim_left(s);
			trim_right(s);
		}

		/// <summary>Check if two vectors are equal.</summary>
		/// <typeparam name="T">The typename.</typeparam>
		/// <param name="v1">The first vector.</param>
		/// <param name="v2">The second vector.</param>
		/// <returns>Returns true if the vectors are the same, else false.</returns>
		template<typename T>
		static inline bool is_equal(std::vector<T>& v1, std::vector<T>& v2) {
			return (v1.size() == v2.size() &&
				std::equal(v1.begin(), v1.end(), v2.begin()));
		}
	}
}
