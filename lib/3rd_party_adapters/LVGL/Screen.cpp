#include "Screen.hpp"
#include <lvgl.h>
#include <stack>
#include <math.h>

static std::stack<IScreen *> screenHistory;

static inline void IScreen_leave()
{
    lv_obj_clean(lv_scr_act());
    if (screenHistory.size() > 1)
    {
        //free up the memory of current screen
        delete screenHistory.top();
        //remove current screen
        screenHistory.pop();
        //draw previous screen
        screenHistory.top()->draw();
        //remove it once again as the draw function has added it again
        screenHistory.pop();
    }
}

static void ScreenMenu_submenu_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemSubmenu *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (item != nullptr))
    {
        lv_obj_clean(lv_scr_act());
        auto ptrSubMenuScreen = new ScreenMenu{*item->getSubMenuList()};
        ptrSubMenuScreen->draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

static void ScreenMenu_switch_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemSwitch *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_VALUE_CHANGED) && (item != nullptr))
    {
        bool *ptr = item->getPtrBool();
        *ptr = lv_obj_has_state(obj, LV_STATE_CHECKED) ? 1 : 0;
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

static void ScreenMenu_value_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemValue *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (item != nullptr))
    {
        auto ValModifyScreen = new ScreenValueModifier{item};
        ValModifyScreen->draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

ScreenMenu::ScreenMenu(MenuItemList itemList)
    : _List{itemList}
{
}

void ScreenMenu::draw()
{
    lv_obj_t *btn;
    lv_obj_t *cont;
    lv_obj_t *swth;
    lv_obj_t *lab;

    lv_obj_clean(lv_scr_act());

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
        case MenuItemType::SUBMENU: {
            /* draw submenu button */
            auto btnItem = reinterpret_cast<const MenuItemSubmenu *const>(item);
            btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_event_cb(btn, ScreenMenu_submenu_cb, LV_EVENT_ALL, (void *)item);
            lv_obj_add_style(btn, &style_small_padding, 0);
            lab = lv_label_create(btn);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(100));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            break;
        }
        case MenuItemType::SWITCH: {
            /* draw switch */
            auto swtItem = reinterpret_cast<const MenuItemSwitch *const>(item);
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
            bool *ptr = swtItem->getPtrBool();
            if (ptr != nullptr)
                lv_obj_add_state(swth, (*ptr) ? LV_STATE_CHECKED : 0);
            else
                lv_obj_add_state(swth, LV_STATE_DISABLED);
            lv_obj_set_size(swth, 18, 12);
            lv_obj_add_event_cb(swth, ScreenMenu_switch_cb, LV_EVENT_ALL, (void *)item);
            lv_obj_set_align(swth, LV_ALIGN_RIGHT_MID);
            break;
        }
        case MenuItemType::VALUE: {
            /* draw value */
            auto valItem = reinterpret_cast<const MenuItemValue *const>(item);
            btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_event_cb(btn, ScreenMenu_value_cb, LV_EVENT_ALL, (void *)item);
            lv_obj_add_style(btn, &style_small_padding, 0);
            lab = lv_label_create(btn);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(70));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            lab = lv_label_create(btn);
            lv_label_set_text_fmt(lab, "%.*f", valItem->getDecimals(), *valItem->getPtrDouble());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_style_text_align(lab, LV_TEXT_ALIGN_RIGHT, 0);
            lv_obj_set_width(lab, lv_pct(25));
            lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);

            double *ptr = valItem->getPtrDouble();
            if (ptr == nullptr)
            {
                lv_obj_add_state(btn, LV_STATE_DISABLED);
            }

            break;
        }
        }
    }

    lv_scr_load(screen);

    screenHistory.push(this);
}

static void ScreenValueModifier_inc_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) && (spinbox != nullptr))
    {
        lv_spinbox_increment(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

static void ScreenValueModifier_dec_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) && (spinbox != nullptr))
    {
        lv_spinbox_decrement(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

static void ScreenValueModifier_step_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)  && (spinbox != nullptr))
    {
        uint32_t step = lv_spinbox_get_step(spinbox);
        step = (step > 100000) ? 1 : (step * 10);
        lv_spinbox_set_step(spinbox, step);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

static void ScreenValueModifier_valueChange_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto item = reinterpret_cast<const MenuItemValue *const>(lv_event_get_user_data(e));

    lv_obj_t *spinbox = 0;

    if ((code == LV_EVENT_VALUE_CHANGED) && (item != nullptr))
    {
        auto ptr = item->getPtrDouble();
        *ptr = ((double)lv_spinbox_get_value(spinbox)) / std::pow(10, item->getDecimals());
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            IScreen_leave();
        }
    }
}

ScreenValueModifier::ScreenValueModifier(const MenuItemValue *const menuItem)
: _menuItem{menuItem}
{
}

void ScreenValueModifier::draw()
{
    lv_obj_t *btn;
    lv_obj_t *lab;
    lv_obj_t *spinbox;

    lv_obj_clean(lv_scr_act());

    static lv_style_t style_scr;
    lv_style_init(&style_scr);
    lv_style_set_pad_left(&style_scr, 1);
    lv_style_set_pad_top(&style_scr, 1);
    lv_style_set_pad_bottom(&style_scr, 1);
    lv_style_set_pad_right(&style_scr, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_scr, 0);

    spinbox = lv_spinbox_create(screen);
    int32_t min = (( _menuItem->getMin())*std::pow(10, _menuItem->getDecimals()));
    int32_t max = (( _menuItem->getMax())*std::pow(10, _menuItem->getDecimals()));
    lv_spinbox_set_range(spinbox, min, max);
    int32_t val = ((*_menuItem->getPtrDouble()) * std::pow(10, _menuItem->getDecimals()));
    lv_spinbox_set_value(spinbox, val);
    lv_spinbox_set_digit_format(spinbox, 7, (7 - _menuItem->getDecimals()));
    lv_obj_set_width(spinbox, lv_pct(55));
    lv_obj_add_style(spinbox, &style_scr, 0);
    lv_obj_center(spinbox);
    lv_group_remove_obj(spinbox);
    lv_obj_add_event_cb(spinbox, ScreenValueModifier_valueChange_cb, LV_EVENT_ALL, (void*)_menuItem);

    lv_coord_t h = lv_obj_get_height(spinbox);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_add_event_cb(btn, ScreenValueModifier_inc_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "+");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_add_event_cb(btn, ScreenValueModifier_dec_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "-");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, 10, 10);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_add_event_cb(btn, ScreenValueModifier_step_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "*");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    lv_scr_load(screen);

    screenHistory.push(this);
}
