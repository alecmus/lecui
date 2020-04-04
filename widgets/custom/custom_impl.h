/*
** custom_impl.h - custom widget interface
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
#include "../custom.h"

namespace liblec {
	namespace lecui {
		namespace widgets_implementation {
			class custom : public widget {
			public:
				custom(const std::string& page,
					const std::string& name,
					IDWriteFactory* p_directwrite_factory,
					IWICImagingFactory* p_iwic_factory);
				~custom();

				// virtual function override

				std::string page();
				std::string name();
				virtual liblec::lecui::widgets_implementation::widget_type type();
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target);
				void discard_resources();
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const float& change_in_width, const float& change_in_height, float x_off_set,
					float y_off_set, const bool& render);
				void on_click();

				// widget specific

				liblec::lecui::widgets::specs::custom& specs();

			private:
				custom() :
					custom(std::string(), std::string(), nullptr, nullptr) {}

				custom(const custom&);
				custom& operator=(const custom&);

				liblec::lecui::widgets::specs::custom specs_, specs_old_;

				IDWriteFactory* p_directwrite_factory_;
				IWICImagingFactory* p_iwic_factory_;
			};
		}
	}
}
