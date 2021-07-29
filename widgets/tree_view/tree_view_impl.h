//
// tree_view_impl.h - tree_view_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include <optional>
#include "../widget_impl.h"
#include "../tree_view.h"
#include "../../containers/pane/pane_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class tree_view_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				tree_view_impl(containers::page& page,
					const std::string& alias,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~tree_view_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool hit(const bool& hit) override;
				void on_right_click() override;

				/// widget specific methods
				widgets::tree_view& specs();
				widgets::tree_view& operator()();
				void set_tree_pane_specs(containers::pane_specs& specs);

			private:
				// Default constructor and copying an object of this class are not allowed
				tree_view_impl() = delete;
				tree_view_impl(const tree_view_impl&) = delete;
				tree_view_impl& operator=(const tree_view_impl&) = delete;

				/// Private variables
				widgets::tree_view _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				ID2D1Factory* _p_direct2d_factory;
				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;

				const float _margin;

				std::optional<std::reference_wrapper<containers::pane_specs>> _tree_pane_specs;

				/// Private methods.
				void on_selection();
			};
		}
	}
}
