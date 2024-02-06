#include "HMI_Menu_Items.hpp"
#include <cmath>
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

const HMI::IScreen *HMI::MenuItemButton::getScreenOnClick() const
{
    return this->_screenOnClick;
}

HMI::MenuItemSwitch::MenuItemSwitch(std::string text, bool *ptrBool)
    : _text{text}, _ptrBool{ptrBool}
{
}

std::string HMI::MenuItemSwitch::getText() const
{
    return this->_text;
}

bool *HMI::MenuItemSwitch::getPtrBool() const
{
    return this->_ptrBool;
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
            auto btnItem = reinterpret_cast<const HMI::MenuItemButton *const>(item);
            btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_event_cb(btn, HMI::ScreenMenu::_button_cb, LV_EVENT_ALL, (void *)btnItem->getScreenOnClick());
            lv_obj_add_style(btn, &style_small_padding, 0);
            lab = lv_label_create(btn);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(100));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            break;
        }
        case HMI::MenuItemType::SWITCH: {
            /* draw switch */
            auto swtItem = reinterpret_cast<const HMI::MenuItemSwitch *const>(item);
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
            lv_obj_add_state(swth, (*swtItem->getPtrBool()) ? LV_STATE_CHECKED : 0);
            lv_obj_set_size(swth, 18, 12);
            lv_obj_add_event_cb(swth, HMI::ScreenMenu::_switch_cb, LV_EVENT_ALL, (void *)swtItem->getPtrBool());
            lv_obj_set_align(swth, LV_ALIGN_RIGHT_MID);
            break;
        }
        case HMI::MenuItemType::VALUE: {
            /* draw value */
            auto valItem = reinterpret_cast<const HMI::MenuItemValue *const>(item);
            btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_event_cb(btn, HMI::ScreenMenu::_value_cb, LV_EVENT_ALL, (void *)valItem);
            lv_obj_add_style(btn, &style_small_padding, 0);
            lab = lv_label_create(btn);
            lv_label_set_text(lab, item->getText().c_str());
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_width(lab, lv_pct(70));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            lab = lv_label_create(btn);
            lv_label_set_text_fmt(lab, "%.*f", valItem->Decimals, *valItem->PtrDouble);
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_style_text_align(lab, LV_TEXT_ALIGN_RIGHT, 0);
            lv_obj_set_width(lab, lv_pct(25));
            lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);
            break;
        }
        }
    }

    lv_scr_load(screen);

    if (screenHistory.top() != this)
        screenHistory.push(this);
}

void HMI::ScreenMenu::addItem(const IMenuItem *newItem)
{
    _List.push_back(newItem);
}

void HMI::ScreenMenu::_button_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto screen = reinterpret_cast<const HMI::IScreen *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (screen != NULL))
    {
        lv_obj_clean(lv_scr_act());
        screen->draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            HMI::IScreen::exit();
        }
    }
}

void HMI::ScreenMenu::_switch_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto var = reinterpret_cast<bool *>(lv_event_get_user_data(e));

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        *var = lv_obj_has_state(obj, LV_STATE_CHECKED) ? 1 : 0;
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            HMI::IScreen::exit();
        }
    }
}

void HMI::ScreenMenu::_value_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto valItem = reinterpret_cast<const HMI::MenuItemValue *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (valItem != NULL))
    {
        static HMI::ScreenValueModifier ValModifyScreen;
        ValModifyScreen.set(valItem->PtrDouble, valItem->Decimals, valItem->Min, valItem->Max);
        lv_obj_clean(lv_scr_act());
        ValModifyScreen.draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            HMI::IScreen::exit();
        }
    }
}

void HMI::IScreen::exit()
{
    if (screenHistory.size() <= 1)
        return;

    lv_obj_clean(lv_scr_act());
    screenHistory.pop();
    screenHistory.top()->draw();
}

void HMI::ScreenValueModifier::draw() const
{
    lv_obj_t *btn;
    lv_obj_t *lab;
    lv_obj_t *spinbox;

    static lv_style_t style_scr;
    lv_style_init(&style_scr);
    lv_style_set_pad_left(&style_scr, 1);
    lv_style_set_pad_top(&style_scr, 1);
    lv_style_set_pad_bottom(&style_scr, 1);
    lv_style_set_pad_right(&style_scr, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_scr, 0);

    spinbox = lv_spinbox_create(screen);
    int32_t min = ((_min)*std::pow(10, _decimals));
    int32_t max = ((_max)*std::pow(10, _decimals));
    lv_spinbox_set_range(spinbox, min, max);
    int32_t val = ((*_ptrDouble) * std::pow(10, _decimals));
    lv_spinbox_set_value(spinbox, val);
    lv_spinbox_set_digit_format(spinbox, 7, (7 - _decimals));
    lv_obj_set_width(spinbox, lv_pct(55));
    lv_obj_add_style(spinbox, &style_scr, 0);
    lv_obj_center(spinbox);
    lv_group_remove_obj(spinbox);

    lv_coord_t h = lv_obj_get_height(spinbox);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_add_event_cb(btn, _button_inc_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "+");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_add_event_cb(btn, _button_dec_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "-");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, 10, 10);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_add_event_cb(btn, _button_step_cb, LV_EVENT_ALL, spinbox);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "*");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    lv_scr_load(screen);

    if (screenHistory.top() != this)
        screenHistory.push(this);
}

void HMI::ScreenValueModifier::set(double *ptrDouble, uint8_t decimals, double min, double max)
{
    _ptrDouble = ptrDouble;
    _decimals = decimals;
    _min = min;
    _max = max;
}

void HMI::ScreenValueModifier::_button_inc_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            HMI::IScreen::exit();
        }
    }
}

void HMI::ScreenValueModifier::_button_dec_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_decrement(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            HMI::IScreen::exit();
        }
    }
}

void HMI::ScreenValueModifier::_button_step_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
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
            HMI::IScreen::exit();
        }
    }
}

HMI::MenuItemValue::MenuItemValue(std::string text, double *ptrDouble, uint8_t decimals, double min, double max)
    : _text{text}, PtrDouble{ptrDouble}, Decimals{decimals}, Min{min}, Max{max}
{
}

std::string HMI::MenuItemValue::getText() const
{
    return _text;
}

double *HMI::MenuItemValue::getPtrDouble() const
{
    return PtrDouble;
}
