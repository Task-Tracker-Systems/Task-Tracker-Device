#include "HMI_Menu_Items.hpp"
#include "lvgl.h"
#include <stack>

static std::stack<const HMI::IScreen *> screenHistory;

HMI::MenuItemButton::MenuItemButton(std::string text, const IScreen *screenOnClick)
    : _text{text}, _screenOnClick{screenOnClick}
{
}

std::string HMI::MenuItemButton::getText() const
{
    return this->_text;
}

HMI::MenuItemSwitch::MenuItemSwitch(std::string text, bool *varialeAddress)
    : _text{text}, _varialeAddress{varialeAddress}
{
}

std::string HMI::MenuItemSwitch::getText() const
{
    return this->_text;
}

void HMI::ScreenMenu::draw() const
{
    lv_obj_t *btn;
    lv_obj_t *cont;
    lv_obj_t *swth;
    lv_obj_t *lab;

    static lv_style_t style_small_padding;
    lv_style_init(&style_small_padding);
    lv_style_set_pad_left(&style_small_padding, 1);
    lv_style_set_pad_top(&style_small_padding, 1);
    lv_style_set_pad_bottom(&style_small_padding, 1);
    lv_style_set_pad_right(&style_small_padding, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_small_padding, 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(screen, 2, 0);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    for (auto const &item : this->_List)
    {
        switch (item->getType())
        {
        case HMI::MenuItemType::BUTTON: {
            /* draw button */
            btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            // lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, (void *)drawSubMenu);
            lv_obj_add_style(btn, &style_small_padding, 0);
            lab = lv_label_create(btn);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(100));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            // lab = lv_label_create(btn);
            // lv_label_set_text_fmt(lab, "%d min", 120);
            // lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            // lv_obj_set_width(lab, lv_pct(25));
            // lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);
            break;
        }
        case HMI::MenuItemType::SWITCH: {
            /* draw switch */
            cont = lv_obj_create(screen);
            lv_obj_set_size(cont, lv_pct(100), 12);
            lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
            lv_obj_add_flag(cont, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
            lv_obj_add_style(cont, &style_small_padding, 0);
            lab = lv_label_create(cont);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(70));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            swth = lv_switch_create(cont);
            lv_obj_set_size(swth, 18, 12);
            // lv_obj_add_event_cb(swth, swth_event_handler, LV_EVENT_ALL, NULL);
            lv_obj_set_align(swth, LV_ALIGN_RIGHT_MID);
            break;
        }
        }
    }

    lv_scr_load(screen);
    screenHistory.push(this);
}

void HMI::ScreenMenu::addItem(const IMenuItem *newItem)
{
    _List.push_back(newItem);
}

void HMI::IScreen::exit()
{
    if (screenHistory.size() <= 1)
        return;

    lv_obj_clean(lv_scr_act());
    screenHistory.pop();
    screenHistory.top()->draw();
}
