/*
** time_impl.h - time_impl interface
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

#include "../widget_impl.h"
#include "../time.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class time_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				static std::string alias_hour();
				static std::string alias_hour_label();
				static std::string alias_minute();
				static std::string alias_minute_label();
				static std::string alias_second();
				static std::string alias_second_label();

				/// Prevent the use of the default constructor.
				time_impl() = delete;

				/// constructor and destructor
				time_impl(containers::page& page,
					const std::string& alias);
				~time_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::time::time_specs& specs();
				widgets::time::time_specs& operator()();

			private:
				/// Prevent copying an object of this class.
				time_impl(const time_impl&);
				time_impl& operator=(const time_impl&);

				/// Private variables
				widgets::time::time_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
			};
		}
	}
}