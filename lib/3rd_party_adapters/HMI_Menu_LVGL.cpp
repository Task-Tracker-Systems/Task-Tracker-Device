#ifdef LV_CONF_INCLUDE_SIMPLE //use this buildflag to check if we actually include LVGL or not

#include "HMI_Menu.hpp"
#include "HMI_Menu_Items.hpp"
#include <lvgl.h>

extern lv_indev_t *my_indev;

static lv_obj_t *spinbox;

static void btn_spinbox_inc_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        // lv_spinbox_increment(spinbox);
    }

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            // lv_obj_clean(lv_scr_act());
            // drawMainMenu();
        }
    }
}

static void btn_spinbox_dec_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        // lv_spinbox_decrement(spinbox);
    }

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            // lv_obj_clean(lv_scr_act());
            // drawMainMenu();
        }
    }
}

static void btn_spinbox_step_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_step_prev(spinbox);
    }

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            // lv_obj_clean(lv_scr_act());
            // drawMainMenu();
        }
    }
}

void my_focus_cb(lv_group_t *group)
{
    lv_obj_t *focusedObject = lv_group_get_focused(group);
}

bool TestBool1;
bool TestBool2 = true;
double TestDouble = 1.23;

/// @brief build basic menu structure
void menu::initialize()
{
    /* create default group for button navigation and assign input device to it */
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(my_indev, group);
    lv_group_set_focus_cb(group, my_focus_cb);

    // drawMainMenu();
    static auto screen = HMI::ScreenMenu{};
    static auto screenBtn1 = HMI::ScreenMenu{};
    static auto screenBtn2 = HMI::ScreenMenu{};
    static auto screenBtn3 = HMI::ScreenMenu{};

    static auto ListButton1 = HMI::MenuItemButton{"ListButton1 Text", &screenBtn1};
    static auto ListValue = HMI::MenuItemValue("ListValue", &TestDouble, 2, 1.4, 0.9);
    static auto ListButton2 = HMI::MenuItemButton{"ListButton2 Text", &screenBtn2};
    static auto ListButton3 = HMI::MenuItemButton{"ListButton3 Text", &screenBtn3};
    static auto ListSwitch1 = HMI::MenuItemSwitch{"ListSwitch1 Text", &TestBool1};
    static auto ListSwitch2 = HMI::MenuItemSwitch{"ListSwitch2 Text", &TestBool2};
    screen.addItem(&ListButton1);
    screen.addItem(&ListValue);
    screen.addItem(&ListButton2);
    screen.addItem(&ListSwitch1);
    screen.addItem(&ListSwitch2);
    screen.addItem(&ListButton3);

    static auto Sub1Button1 = HMI::MenuItemButton{"Sub1 Button1", &screenBtn1};
    static auto Sub1Button2 = HMI::MenuItemButton{"Sub1 Button2", &screenBtn2};
    screenBtn1.addItem(&Sub1Button1);
    screenBtn1.addItem(&Sub1Button2);

    static auto Sub2Button1 = HMI::MenuItemButton{"Sub2 Button1", &screenBtn1};
    static auto Sub2Button2 = HMI::MenuItemButton{"Sub2 Button2", &screenBtn2};
    screenBtn2.addItem(&Sub2Button1);
    screenBtn2.addItem(&Sub2Button2);

    static auto Sub3Button1 = HMI::MenuItemButton{"Sub3 Button1", &screenBtn1};
    static auto Sub3Button2 = HMI::MenuItemButton{"Sub3 Button2", &screenBtn2};
    screenBtn3.addItem(&Sub3Button1);
    screenBtn3.addItem(&Sub3Button2);

    screen.draw();
}

/// @brief called cyclical for debug reasons
void menu::cyclic()
{
    static uint32_t count = 0;
    count += 1;

    switch (count)
    {
    case 20:
        break;
    case 30:
        break;
    case 40:
        break;
    case 50:
        break;
    case 60:
        break;
    case 80:
        break;
    case 120:
        break;
    default:
        break;
    }

    LV_LOG_USER("Bools: %u %u", TestBool1, TestBool2);
}

#endif