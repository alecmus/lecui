/*
** non_client_hittest.cpp - non client hit test implementation
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

			border.x = static_cast<long>(.5f + dpi_scale_ * border.x);
			border.y = static_cast<long>(.5f + dpi_scale_ * border.y);

			RECT window;
			if (!::GetWindowRect(hWnd_, &window))
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
				cursor.y < (window.top + static_cast<long>(.5f + dpi_scale_ * caption_bar_height_))) {
				POINT m_pt = { 0, 0 };
				ScreenToClient(hWnd_, &m_pt);

				D2D1_POINT_2F point{ static_cast<float>(cursor.x + m_pt.x),
					static_cast<float>(cursor.y + m_pt.y) };

				// exclude page widgets
				bool exclude = false;
				for (auto& widget : widgets_) {
					if (widget.second.contains(point)) {
						exclude = true;
						break;
					}
				}

				if (exclude)
					result = client;
				else
					result = caption;
			}

			switch (result) {
			case left: return allow_resizing_ ? HTLEFT : HTCLIENT;
			case right: return allow_resizing_ ? HTRIGHT : HTCLIENT;
			case top: return allow_resizing_ ? HTTOP : HTCLIENT;
			case bottom: return allow_resizing_ ? HTBOTTOM : HTCLIENT;
			case top | left: return allow_resizing_ ? HTTOPLEFT : HTCLIENT;
			case top | right: return allow_resizing_ ? HTTOPRIGHT : HTCLIENT;
			case bottom | left: return allow_resizing_ ? HTBOTTOMLEFT : HTCLIENT;
			case bottom | right: return allow_resizing_ ? HTBOTTOMRIGHT : HTCLIENT;
			case client: return HTCLIENT;
			case caption: return HTCAPTION;
			default: return HTNOWHERE;
			}
		}
	}
}
