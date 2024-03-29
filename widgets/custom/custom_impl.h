//
// custom_impl.h - custom_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../widget_impl.h"
#include "../custom.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class custom_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				custom_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory,
					IWICImagingFactory* p_iwic_factory);
				~custom_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::custom& specs();
				widgets::custom& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				custom_impl() = delete;
				custom_impl(const custom_impl&) = delete;
				custom_impl& operator=(const custom_impl&) = delete;

				/// Private variables
				widgets::custom _specs, _specs_old;

				IDWriteFactory* _p_directwrite_factory;
				IWICImagingFactory* _p_iwic_factory;
			};
		}
	}
}
