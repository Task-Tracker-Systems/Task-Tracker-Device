#include "HMI_Menu_Items.hpp"
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
            // lab = lv_label_create(btn);
            // lv_label_set_text_fmt(lab, "%d min", 120);
            // lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            // lv_obj_set_width(lab, lv_pct(25));
            // lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);
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
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
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

void HMI::IScreen::exit()
{
    LV_LOG_USER("exit start history has %u", screenHistory.size());
    if (screenHistory.size() <= 1)
        return;

    lv_obj_clean(lv_scr_act());
    screenHistory.pop();
    screenHistory.top()->draw();
    LV_LOG_USER("exit end history has %u", screenHistory.size());
}
