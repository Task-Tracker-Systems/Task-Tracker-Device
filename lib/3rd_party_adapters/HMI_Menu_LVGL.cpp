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

static void drawValueModifier()
{
    lv_obj_t *btn;
    lv_obj_t *lab;

    static lv_style_t style_scr;
    lv_style_init(&style_scr);
    lv_style_set_pad_left(&style_scr, 1);
    lv_style_set_pad_top(&style_scr, 1);
    lv_style_set_pad_bottom(&style_scr, 1);
    lv_style_set_pad_right(&style_scr, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_scr, 0);

    spinbox = lv_spinbox_create(screen);
    lv_spinbox_set_range(spinbox, -1000, 25000);
    lv_spinbox_set_digit_format(spinbox, 5, 0);
    lv_spinbox_step_prev(spinbox);
    lv_obj_set_width(spinbox, lv_pct(55));
    lv_obj_add_style(spinbox, &style_scr, 0);
    lv_obj_center(spinbox);
    lv_group_remove_obj(spinbox);

    lv_coord_t h = lv_obj_get_height(spinbox);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_add_event_cb(btn, btn_spinbox_inc_event_cb, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "+");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_add_event_cb(btn, btn_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "-");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    btn = lv_btn_create(screen);
    lv_obj_add_style(btn, &style_scr, 0);
    lv_obj_set_size(btn, 10, 10);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_add_event_cb(btn, btn_spinbox_step_event_cb, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "*");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    lv_scr_load(screen);
}

void my_focus_cb(lv_group_t *group)
{
    lv_obj_t *focusedObject = lv_group_get_focused(group);
}

bool TestBool1, TestBool2;

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
    static auto ListButton2 = HMI::MenuItemButton{"ListButton2 Text", &screenBtn2};
    static auto ListButton3 = HMI::MenuItemButton{"ListButton2 Text", &screenBtn3};
    static auto ListSwitch1 = HMI::MenuItemSwitch{"ListSwitch1 Text", &TestBool1};
    static auto ListSwitch2 = HMI::MenuItemSwitch{"ListSwitch2 Text", &TestBool2};

    screen.addItem(&ListButton1);
    screen.addItem(&ListButton2);
    screen.addItem(&ListSwitch1);
    screen.addItem(&ListSwitch2);
    screen.addItem(&ListButton3);

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