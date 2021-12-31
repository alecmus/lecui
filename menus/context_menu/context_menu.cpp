//
// context_menu.cpp - context menu implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../context_menu.h"
#include "../../appearance.h"
#include "../../controls.h"
#include "../../containers/page.h"
#include "../../widgets/label.h"
#include "../../widgets/rectangle.h"
#include "../../widgets/line.h"

#ifdef get_line
// prevent conflict with msxml.h
#undef get_line
#endif

#include "../../widgets/image_view.h"
#include "../../form_common.h"

namespace liblec {
	namespace lecui {
        class context_menu::impl : public lecui::form {
            const float _margin = 5.f;
            const lecui::size _min_size = { 135.f, 20.f };
            const float _min_font_size = 6.f;
            const float _max_font_size = 72.f;
            lecui::size _max_size = { 4200.f, 10000.f };
            const std::string _al_page_home = "home_page";
            lecui::page_manager _page_man{ *this };
            const context_menu::specs& _menu_specs;
            std::vector<lecui::rect> _rects;
            bool _images = false;
            float _image_size = 0.f;
            float _next_arrow_width = 0.f;

        public:
            std::string _acted_on;

            impl(lecui::form& parent, const specs& menu_specs) :
                form(menu_form_caption(), parent),
                _menu_specs(menu_specs) {
                // determine whether any item has an image
                for (const auto& item : _menu_specs.items)
                    if (item.image_png_resource || !item.image_file.empty()) {
                        _images = true;
                        break;
                    }

                // make top level form
                lecui::controls ctrls(*this);
                ctrls.make_top_most(true);

                // measure items
                lecui::dimensions dim(*this);

                // compute maximum based on working area size
                auto rect_wa = dim.working_area();

                // add padding to working area
                rect_wa.left() += _margin;
                rect_wa.right() -= _margin;
                rect_wa.top() += _margin;
                rect_wa.bottom() -= _margin;

                // set maximum size to working area
                _max_size.width(smallest(_max_size.get_width(), rect_wa.width()));
                _max_size.height(smallest(_max_size.get_height(), rect_wa.height()));

                const auto cursor_rect = dim.cursor_rect();

                // impose maximums
                const lecui::rect max_rect = rect()
                    .left(_margin)
                    .right(_max_size.get_width())
                    .top(_margin)
                    .bottom(_max_size.get_height());
                float _bottom = 0.f;
                float _right = 0.f;

                // compute rects
                bool children = false;
                for (auto& item : _menu_specs.items) {
                    auto rect = max_rect;

                    if (!_rects.empty())
                        rect.move(rect.left(), _bottom + _margin);

                    // enforce limits on font size
                    auto font_size = item.font_size;
                    font_size = largest(font_size, _min_font_size);
                    font_size = smallest(font_size, _max_font_size);

                    if (!item.label.empty()) {
                        rect = dim.measure_label(item.label, item.font, font_size, text_alignment::left, paragraph_alignment::top, rect);
                        rect.bottom() += (2 * _margin);   // padding
                    }
                    else {
                        // separator
                        rect.height(1.f);
                        rect.width(0.f);
                    }

                    if (!item.label.empty() && _images)
                        rect.right() += (rect.height() + (_margin / 2.f)); // image

                    _bottom = rect.bottom();
                    _right = largest(_right, rect.right());

                    // separators cannot have children
                    if (!item.label.empty()) {
                        // check if there are any children
                        if (!item.children.empty())
                            children = true;
                    }

                    _rects.push_back(rect);
                }

                // equate widths
                for (auto& rect : _rects)
                    rect.right() = _right;

                auto width = _right + _margin;

                auto height = smallest(_max_size.get_height(), _bottom + _margin);

                if (children) {
                    _next_arrow_width = height / _menu_specs.items.size();
                    width += _next_arrow_width;
                }

                if ((_menu_specs.pin.get_right() - _menu_specs.pin.get_left()) > 0.f && _menu_specs.type == pin_type::bottom)
                    width = largest(_min_size.get_width(), _menu_specs.pin.get_right() - _menu_specs.pin.get_left());

                // impose minimums
                width = largest(width, _min_size.get_width());
                height = largest(height, _min_size.get_height());

                auto set_size = [&]() {
                    dim.set_size({ width, height });
                    dim.set_minimum({ width, height });
                };

                // default to top left corner of cursor rect
                lecui::point top_left = { cursor_rect.get_left(), cursor_rect.get_top() };

                if ((_menu_specs.pin.get_right() - _menu_specs.pin.get_left()) > 0.f) {
                    if (_menu_specs.type == pin_type::bottom) {
                        // pin to the bottom if there is enough space, or if the area beneath is
                        // larger than that above, else pin above

                        // determine height using pin
                        const auto space_above = _menu_specs.pin.get_top() - rect_wa.top();
                        const auto space_below = rect_wa.bottom() - _menu_specs.pin.get_bottom();

                        // check if menu exceeds working area downwards when pinned
                        bool pin_below = true;

                        if (space_below < height) {
                            // choose larger side
                            if (space_below > space_above) {
                                // pin below
                                pin_below = true;
                                height = smallest(space_below, height);
                            }
                            else {
                                // pin above
                                pin_below = false;
                                height = smallest(space_above, height);
                            }
                        }
                        else
                            pin_below = true;

                        set_size();

                        if (pin_below)
                            top_left = { _menu_specs.pin.get_left(), _menu_specs.pin.get_bottom() };
                        else
                            top_left = { _menu_specs.pin.get_left(), _menu_specs.pin.get_top() - height };
                    }
                    else {
                        const auto space_right = rect_wa.right() - _menu_specs.pin.get_right();
                        const auto space_left = _menu_specs.pin.get_left() - rect_wa.left();

                        if (space_right > width) {
                            // pin to the right
                            top_left.x(_menu_specs.pin.get_right());
                        }
                        else {
                            if (space_right > space_left) {
                                width = smallest(space_right, width);
                                top_left.x(_menu_specs.pin.get_right());
                            }
                            else {
                                width = smallest(space_left, width);
                                top_left.x(_menu_specs.pin.get_left() - width);
                            }
                        }

                        top_left.y(_menu_specs.pin.get_top());

                        set_size();

                        // prevent from getting hidden at the bottom
                        if ((top_left.get_y() + height) > rect_wa.bottom())
                            top_left.y() -= (top_left.get_y() + height - rect_wa.bottom());
                    }
                }
                else {
                    set_size();

                    // prevent from getting hidden at the bottom
                    if ((top_left.get_y() + height) > rect_wa.bottom())
                        top_left.y() -= (top_left.get_y() + height - rect_wa.bottom());

                    // prevent from getting hidden on the right
                    if ((top_left.get_x() + width) > rect_wa.right())
                        top_left.x() -= (top_left.get_x() + width - rect_wa.right());
                }

                // move this form to the cursor position
                move(top_left);
            }

            bool on_layout(std::string& error) override {
                // add home page
                auto& home_page = _page_man.add(_al_page_home);

                // add labels
                int index = 0;
                for (const auto& item : _menu_specs.items) {
                    float left_most = _rects[index].left();

                    if (!item.label.empty()) {
                        // background
                        auto& rect = lecui::widgets::rectangle::add(home_page);
                        rect.rect(_rects[index]);
                        rect.rect()
                            .left(_margin / 3.f)
                            .right(home_page.size().get_width() - _margin / 3.f);
                        rect.color_fill().alpha(0);
                        rect.color_border().alpha(0);
                        rect.color_border_hot().alpha(0);
                        rect.color_hot().alpha(50);
                        rect.events().action = [&]() {
                            _acted_on = item.label;
                            close();
                        };

                        if (_images) {
                            // image
                            auto& image = lecui::widgets::image_view::add(home_page);
                            image
                                .rect(_rects[index])
                                .rect().width(image.rect().height());    // make into a square
                            image
                                .png_resource(item.image_png_resource)
                                .file(item.image_file)
                                .quality(_menu_specs.quality);

                            left_most = image.rect().right() + (_margin / 2.f);

                            // padding
                            image.rect().left() += (_margin / 1.5f);
                            image.rect().top() += (_margin / 1.5f);
                            image.rect().right() -= (_margin / 1.5f);
                            image.rect().bottom() -= (_margin / 1.5f);
                        }

                        // label
                        auto& label = lecui::widgets::label::add(home_page);
                        label.text(item.label).font(item.font).font_size(item.font_size);

                        // enforce font size limits
                        label
                            .font_size(largest(label.font_size(), _min_font_size))
                            .font_size(smallest(label.font_size(), _max_font_size))
                            .paragraph_alignment(paragraph_alignment::middle)
                            .rect(_rects[index])
                            .rect().left(left_most);

                        if (!item.children.empty()) {
                            // draw expansion arrow
                            auto& image = lecui::widgets::image_view::add(home_page);
                            image
                                .file("images\\menu_item_next.png")
                                .rect(_rects[index])
                                .rect().right(home_page.size().get_width()).left(image.rect().right() - (_next_arrow_width));

                            // padding
                            image.rect().left() += (_margin / 1.5f);
                            image.rect().top() += (_margin / 1.5f);
                            image.rect().right() -= (_margin / 1.5f);
                            image.rect().bottom() -= (_margin / 1.5f);

                            rect.events().mouse_enter = [&]() {
                                make_child();
                            };
                            rect.events().mouse_leave = [&]() {
                                destroy_child();
                            };
                        }
                    }
                    else {
                        // line
                        auto& line = lecui::widgets::line::add(home_page);
                        line.color_fill().alpha() /= 2;
                        line.thickness() /= 2.f;
                        line.rect(_rects[index]);
                        line.rect().left(_margin / 3.f).right(home_page.size().get_width() - _margin / 3.f);
                        line.points({ { 0.f, line.rect().height() / 2.f },
                            { line.rect().width(), line.rect().height() / 2.f } });
                    }

                    index++;
                }

                _page_man.show(_al_page_home);
                return true;
            }

            void make_child() {
                // to-do: implement
                log("make_child");
            }

            void destroy_child() {
                // to-do: implement
                log("destroy_child");
            }
        };

        context_menu::context_menu() {}

        std::string context_menu::operator()(form& fm, const specs& menu_specs) {
            impl m(fm, menu_specs);
            std::string error;
            if (!m.create(error))
                log(error);
            return m._acted_on;
		}
	}
}
