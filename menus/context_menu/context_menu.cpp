//
// context_menu.cpp - context menu implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../context_menu.h"
#include "../../appearance.h"
#include "../../controls.h"
#include "../../containers/page.h"
#include "../../widgets/label.h"
#include "../../widgets/rectangle.h"
#include "../../widgets/line.h"
#include "../../widgets/image_view.h"
#include "../../form_common.h"

namespace liblec {
	namespace lecui {
        class context_menu::impl : public lecui::form {
            const float margin_ = 5.f;
            const lecui::size min_size_ = { 135.f, 20.f };
            const float min_font_size_ = 6.f;
            const float max_font_size_ = 72.f;
            lecui::size max_size_ = { 4200.f, 10000.f };
            const std::string al_page_home_ = "home_page";
            lecui::page_management page_man_{ *this };
            const context_menu::specs& menu_specs_;
            std::vector<lecui::rect> rects_;
            bool images_ = false;
            float image_size_ = 0.f;
            float next_arrow_width_ = 0.f;

        public:
            std::string acted_on_;

            impl(lecui::form& parent, const specs& menu_specs) :
                form(menu_form_caption(), parent),
                menu_specs_(menu_specs) {
                // determine whether any item has an image
                for (const auto& item : menu_specs_.items)
                    if (!item.image_file.empty()) {
                        images_ = true;
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
                rect_wa.left += margin_;
                rect_wa.right -= margin_;
                rect_wa.top += margin_;
                rect_wa.bottom -= margin_;

                // set maximum size to working area
                max_size_.width = smallest(max_size_.width, rect_wa.width());
                max_size_.height = smallest(max_size_.height, rect_wa.height());

                const auto cursor_rect = dim.cursor_rect();

                // impose maximums
                const lecui::rect max_rect = { margin_, max_size_.width, margin_, max_size_.height };
                float bottom_ = 0.f;
                float right_ = 0.f;

                // compute rects
                bool children = false;
                for (auto& item : menu_specs_.items) {
                    auto rect = max_rect;

                    if (!rects_.empty())
                        rect.move(rect.left, bottom_ + margin_);

                    // enforce limits on font size
                    auto font_size = item.font_size;
                    font_size = largest(font_size, min_font_size_);
                    font_size = smallest(font_size, max_font_size_);

                    if (!item.label.empty()) {
                        rect = dim.measure_label(item.label, item.font, font_size, false, false, rect);
                        rect.bottom += (2 * margin_);   // padding
                    }
                    else {
                        // separator
                        rect.height(1.f);
                        rect.width(0.f);
                    }

                    if (!item.label.empty() && images_)
                        rect.right += (rect.height() + (margin_ / 2.f)); // image

                    bottom_ = rect.bottom;
                    right_ = largest(right_, rect.right);

                    // separators cannot have children
                    if (!item.label.empty()) {
                        // check if there are any children
                        if (!item.children.empty())
                            children = true;
                    }

                    rects_.push_back(rect);
                }

                // equate widths
                for (auto& rect : rects_)
                    rect.right = right_;

                auto width = right_ + margin_;

                auto height = smallest(max_size_.height, bottom_ + margin_);

                if (children) {
                    next_arrow_width_ = height / menu_specs_.items.size();
                    width += next_arrow_width_;
                }

                if ((menu_specs_.pin.right - menu_specs_.pin.left) > 0.f && menu_specs_.type == pin_type::bottom)
                    width = largest(min_size_.width, menu_specs_.pin.right - menu_specs_.pin.left);

                // impose minimums
                width = largest(width, min_size_.width);
                height = largest(height, min_size_.height);

                auto set_size = [&]() {
                    dim.set_size({ width, height });
                    dim.set_minimum({ width, height });
                };

                // default to top left corner of cursor rect
                lecui::point top_left = { cursor_rect.left, cursor_rect.top };

                if ((menu_specs_.pin.right - menu_specs_.pin.left) > 0.f) {
                    if (menu_specs_.type == pin_type::bottom) {
                        // pin to the bottom if there is enough space, or if the area beneath is
                        // larger than that above, else pin above

                        // determine height using pin
                        const auto space_above = menu_specs_.pin.top - rect_wa.top;
                        const auto space_below = rect_wa.bottom - menu_specs_.pin.bottom;

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
                            top_left = { menu_specs_.pin.left, menu_specs_.pin.bottom };
                        else
                            top_left = { menu_specs_.pin.left, menu_specs_.pin.top - height };
                    }
                    else {
                        const auto space_right = rect_wa.right - menu_specs_.pin.right;
                        const auto space_left = menu_specs_.pin.left - rect_wa.left;

                        if (space_right > width) {
                            // pin to the right
                            top_left.x = menu_specs_.pin.right;
                        }
                        else {
                            if (space_right > space_left) {
                                width = smallest(space_right, width);
                                top_left.x = menu_specs_.pin.right;
                            }
                            else {
                                width = smallest(space_left, width);
                                top_left.x = menu_specs_.pin.left - width;
                            }
                        }

                        top_left.y = menu_specs_.pin.top;

                        set_size();

                        // prevent from getting hidden at the bottom
                        if ((top_left.y + height) > rect_wa.bottom)
                            top_left.y -= (top_left.y + height - rect_wa.bottom);
                    }
                }
                else {
                    set_size();

                    // prevent from getting hidden at the bottom
                    if ((top_left.y + height) > rect_wa.bottom)
                        top_left.y -= (top_left.y + height - rect_wa.bottom);

                    // prevent from getting hidden on the right
                    if ((top_left.x + width) > rect_wa.right)
                        top_left.x -= (top_left.x + width - rect_wa.right);
                }

                // move this form to the cursor position
                move(top_left);
            }

            bool on_layout(std::string& error) override {
                // add home page
                auto& home_page = page_man_.add(al_page_home_);

                // add labels
                int index = 0;
                for (const auto& item : menu_specs_.items) {
                    float left_most = rects_[index].left;

                    if (!item.label.empty()) {
                        // background
                        lecui::widgets::rectangle rect(home_page, "");
                        rect().rect(rects_[index]);
                        rect().rect().left = margin_ / 3.f;
                        rect().rect().right = home_page.size().width - margin_ / 3.f;
                        rect().color_fill().alpha = 0;
                        rect().color_border().alpha = 0;
                        rect().color_border_hot().alpha = 0;
                        rect().color_hot().alpha = 50;
                        rect().events().action = [&]() {
                            acted_on_ = item.label;
                            close();
                        };

                        if (images_) {
                            // image
                            lecui::widgets::image_view image(home_page, "");
                            image().rect(rects_[index]);
                            image().rect().width(image().rect().height());    // make into a square
                            image().file(item.image_file).quality(menu_specs_.quality);
                            left_most = image().rect().right + (margin_ / 2.f);

                            // padding
                            image().rect().left += (margin_ / 1.5f);
                            image().rect().top += (margin_ / 1.5f);
                            image().rect().right -= (margin_ / 1.5f);
                            image().rect().bottom -= (margin_ / 1.5f);
                        }

                        // label
                        lecui::widgets::label label(home_page, "");
                        label().text(item.label).font(item.font).font_size(item.font_size);

                        // enforce font size limits
                        label().font_size(largest(label().font_size(), min_font_size_));
                        label().font_size(smallest(label().font_size(), max_font_size_));

                        label().rect(rects_[index]);
                        label().rect().left = left_most;
                        label().center_v(true);

                        if (!item.children.empty()) {
                            // draw expansion arrow
                            lecui::widgets::image_view image(home_page, "");
                            image().rect(rects_[index]);
                            image().rect().right = home_page.size().width;
                            image().rect().left = image().rect().right - (next_arrow_width_);
                            image().file("images\\menu_item_next.png");

                            // padding
                            image().rect().left += (margin_ / 1.5f);
                            image().rect().top += (margin_ / 1.5f);
                            image().rect().right -= (margin_ / 1.5f);
                            image().rect().bottom -= (margin_ / 1.5f);

                            rect().events().mouse_enter = [&]() {
                                make_child();
                            };
                            rect().events().mouse_leave = [&]() {
                                destroy_child();
                            };
                        }
                    }
                    else {
                        // line
                        lecui::widgets::line line(home_page, "");
                        line().color_fill().alpha /= 2;
                        line().thickness() /= 2.f;
                        line().rect(rects_[index]);
                        line().rect().left = margin_ / 3.f;
                        line().rect().right = home_page.size().width - margin_ / 3.f;
                        line().points({ { 0.f, line().rect().height() / 2.f },
                            { line().rect().width(), line().rect().height() / 2.f } });
                    }

                    index++;
                }

                page_man_.show(al_page_home_);
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
            if (!m.show(error))
                log(error);
            return m.acted_on_;
		}
	}
}
