//
// splash.cpp - splash screen implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../splash.h"
#include "../../image_converter/image_converter.h"
#include "../../form_impl/form_impl.h"
#include <Windows.h>

namespace liblec {
	namespace lecui {
		class splash::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;

			// Window Class name
			const TCHAR* _splash_class_name = L"liblec::lecui::splash";

			HWND _hWnd_splash;
			HBITMAP _hbmp_splash;

			/// Registers a window class for the splash and splash owner windows.
			void register_window_class(int IDI_SPLASHICON) {
				HINSTANCE h_instance = GetModuleHandle(nullptr);	// mod
				WNDCLASS wc = { 0 };
				wc.lpfnWndProc = DefWindowProc;
				wc.hInstance = h_instance;
				wc.hIcon = LoadIcon(h_instance, MAKEINTRESOURCE(IDI_SPLASHICON));
				wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
				wc.lpszClassName = _splash_class_name;

				if (RegisterClass(&wc) == NULL) {
					// splash window registration failed
				}
			}

			/// Create the splash owner window and the splash window
			/// returns a handle to the splash window
			HWND CreateSplashWindow(int IDI_SPLASHICON) {
				register_window_class(IDI_SPLASHICON);

				HINSTANCE h_instance = GetModuleHandle(NULL);
				HWND hWnd_owner = CreateWindow(_splash_class_name, nullptr, WS_POPUP,
					0, 0, 0, 0, nullptr, nullptr, h_instance, nullptr);

				if (hWnd_owner == nullptr)
					return nullptr;

				/// create the window
				/// WS_EX_LAYERED flag for extended styles is crucial here
				return CreateWindowEx(WS_EX_LAYERED, _splash_class_name, nullptr, WS_POPUP | WS_VISIBLE,
					0, 0, 0, 0, hWnd_owner, nullptr, h_instance, nullptr);
			}

			/// Calls UpdateLayeredWindow to set a bitmap (with an alpha channel) as the content of the splash window
			void SetSplashImage(HWND hWnd_splash, HBITMAP hbmp_splash) {
				// get the size of the bitmap
				BITMAP bmp;
				GetObject(hbmp_splash, sizeof(bmp), &bmp);
				SIZE sizeSplash = { bmp.bmWidth, bmp.bmHeight };

				// position window to determine which monitor to use
				SetWindowPos(hWnd_splash, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

				// get the monitor's info
				HMONITOR h_mon = MonitorFromWindow(hWnd_splash, MONITOR_DEFAULTTONEAREST);
				MONITORINFO monitor_info = { 0 };
				monitor_info.cbSize = sizeof(monitor_info);
				GetMonitorInfo(h_mon, &monitor_info);

				// center the splash screen in the middle of the primary work area
				const RECT& rect_work = monitor_info.rcWork;
				POINT pt_origin;
				pt_origin.x = rect_work.left + (rect_work.right - rect_work.left - sizeSplash.cx) / 2;
				pt_origin.y = rect_work.top + (rect_work.bottom - rect_work.top - sizeSplash.cy) / 2;

				// create a memory DC holding the splash bitmap
				HDC hdc_screen = GetDC(nullptr);
				HDC hdc_mem = CreateCompatibleDC(hdc_screen);
				HBITMAP hbmp_old = (HBITMAP)SelectObject(hdc_mem, hbmp_splash);

				// use the source image's alpha channel for blending
				BLENDFUNCTION blend = { 0 };
				blend.BlendOp = AC_SRC_OVER;
				blend.SourceConstantAlpha = 255;
				blend.AlphaFormat = AC_SRC_ALPHA;

				// paint the window (in the right location) with the alpha-blended bitmap
				POINT pt_zero = { 0 };
				UpdateLayeredWindow(hWnd_splash, hdc_screen, &pt_origin, &sizeSplash,
					hdc_mem, &pt_zero, RGB(0, 0, 0), &blend, ULW_ALPHA);

				// delete temporary objects
				SelectObject(hdc_mem, hbmp_old);
				DeleteDC(hdc_mem);
				ReleaseDC(nullptr, hdc_screen);
			}
		};

		splash::splash(form& fm) :
			_d(*(new impl(fm))) {}

		splash::~splash() {
			remove();
			delete& _d;
		}

		bool splash::display(int png_resource, bool dpi_aware, std::string& error) {
			if (!_d._hWnd_splash) {		// failsafe
				image_converter imgcv;

				_d._hWnd_splash = _d.CreateSplashWindow(_d._fm._d._idi_icon_small);
				_d._hbmp_splash = imgcv.png_to_argb(_d._fm._d._resource_module_handle,
					png_resource, dpi_aware ? _d._fm._d.get_dpi_scale() : 1.f, error);

				if (_d._hbmp_splash == nullptr)
					return false;	// error

				_d.SetSplashImage(_d._hWnd_splash, _d._hbmp_splash);
			}

			return true;
		}
		void splash::remove() {
			if (_d._hWnd_splash) {
				DestroyWindow(_d._hWnd_splash);
				_d._hWnd_splash = nullptr;
			}
		}
	}
}
