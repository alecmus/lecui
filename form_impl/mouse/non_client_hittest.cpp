//
// non_client_hittest.cpp - non client hit test implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		LRESULT form::impl::non_client_hittest(const POINT& cursor) {
			// identify borders and corners to allow resizing the window.
			// Note: On Windows 10, windows behave differently and
			// allow resizing outside the visible window frame.
			// This implementation does not replicate that behavior.
			// to-do: check if this behavior can be replicated for a borderless window.
			POINT border{
				::GetSystemMetrics(SM_CXFRAME),
				::GetSystemMetrics(SM_CYFRAME)
			};

			border.x = static_cast<long>(.5f + _dpi_scale * border.x);
			border.y = static_cast<long>(.5f + _dpi_scale * border.y);

			RECT window;
			if (!::GetWindowRect(_hWnd, &window))
				return HTNOWHERE;

			enum region_mask {
				client = 0b00000,
				left = 0b00001,
				right = 0b00010,
				top = 0b00100,
				bottom = 0b01000,
				caption = 0b10000,
			};

			auto result =
				left * (cursor.x < (window.left + border.x)) |
				right * (cursor.x >= (window.right - border.x)) |
				top * (cursor.y < (window.top + border.y)) |
				bottom * (cursor.y >= (window.bottom - border.y));

			if (cursor.x >= (window.left + border.x) &&
				cursor.x < (window.right - border.x) &&
				cursor.y >= (window.top + border.y) &&
				cursor.y < (window.top + static_cast<long>(.5f + _dpi_scale * _caption_bar_height))) {
				POINT m_pt = { 0, 0 };
				ScreenToClient(_hWnd, &m_pt);

				D2D1_POINT_2F point{ static_cast<float>(cursor.x + m_pt.x),
					static_cast<float>(cursor.y + m_pt.y) };

				// exclude page widgets
				bool exclude = false;
				for (auto& widget : _widgets) {
					if (widget.second.contains(point)) {
						exclude = true;
						break;
					}
				}

				// exclude side pane
				if (_side_pane_present) {
					try {
						auto& side_pane = _p_status_panes.at("status::left");
						if (side_pane._d_page.contains(point))
							exclude = true;
					}
					catch (const std::exception&) {}
				}

				if (exclude)
					result = client;
				else
					result = caption;
			}

			switch (result) {
			case left: return _allow_resizing ? HTLEFT : HTCLIENT;
			case right: return _allow_resizing ? HTRIGHT : HTCLIENT;
			case top: return _allow_resizing ? HTTOP : HTCLIENT;
			case bottom: return _allow_resizing ? HTBOTTOM : HTCLIENT;
			case top | left: return _allow_resizing ? HTTOPLEFT : HTCLIENT;
			case top | right: return _allow_resizing ? HTTOPRIGHT : HTCLIENT;
			case bottom | left: return _allow_resizing ? HTBOTTOMLEFT : HTCLIENT;
			case bottom | right: return _allow_resizing ? HTBOTTOMRIGHT : HTCLIENT;
			case client: return HTCLIENT;
			case caption: return HTCAPTION;
			default: return HTNOWHERE;
			}
		}
	}
}
