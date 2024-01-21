#ifdef LV_CONF_INCLUDE_SIMPLE //use this buildflag to check if we actually include LVGL or not

#include "HMI_Menu.hpp"
#include <lvgl.h>

extern lv_indev_t *my_indev;

static void btn_event_handler(lv_event_t *e);
static void swth_event_handler(lv_event_t *e);
static void btn_spinbox_inc_event_cb(lv_event_t *e);
static void btn_spinbox_dec_event_cb(lv_event_t *e);
static void btn_spinbox_step_event_cb(lv_event_t *e);

static void drawMainMenu();
static void drawValueModifier();
static void drawSubMenu();

static void drawMainMenu()
{
    lv_obj_t *btn;
    lv_obj_t *lab;

    lv_obj_t *cont;
    lv_obj_t *swth;

    static lv_style_t style_scr;
    lv_style_init(&style_scr);
    lv_style_set_pad_left(&style_scr, 1);
    lv_style_set_pad_top(&style_scr, 1);
    lv_style_set_pad_bottom(&style_scr, 1);
    lv_style_set_pad_right(&style_scr, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_scr, 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(screen, 2, 0);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_pad_left(&style_btn, 0);

    /* draw first button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, (void *)drawSubMenu);
    lv_obj_add_style(btn, &style_btn, 0);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "Button 1");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(70));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
    lab = lv_label_create(btn);
    lv_label_set_text_fmt(lab, "%d min", 120);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(25));
    lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);

    /* draw second button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, (void *)drawValueModifier);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "Button 2");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(70));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "was anderes");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(25));
    lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);

    /* draw switch */
    cont = lv_obj_create(screen);
    lv_obj_set_size(cont, lv_pct(100), 12);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lab = lv_label_create(cont);
    lv_label_set_text(lab, "Switch");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(70));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
    swth = lv_switch_create(cont);
    lv_obj_set_size(swth, 18, 12);
    lv_obj_add_event_cb(swth, swth_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_align(swth, LV_ALIGN_RIGHT_MID);

    /* draw third button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "Button 3");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(100));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);

    /* draw fourth button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "Button 4");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(100));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);

    /* draw fifth button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "Button 5");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(100));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);

    lv_scr_load(screen);
}

static void drawSubMenu()
{
    lv_obj_t *btn;
    lv_obj_t *lab;

    lv_obj_t *cont;
    lv_obj_t *swth;

    static lv_style_t style_scr;
    lv_style_init(&style_scr);
    lv_style_set_pad_left(&style_scr, 1);
    lv_style_set_pad_top(&style_scr, 1);
    lv_style_set_pad_bottom(&style_scr, 1);
    lv_style_set_pad_right(&style_scr, 1);

    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_scr, 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(screen, 2, 0);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    /* draw sub button 1 */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, (void *)drawValueModifier);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "SubButton 1");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(100));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);

    /* draw sub button 2 */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, lv_pct(100), 12);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);
    lab = lv_label_create(btn);
    lv_label_set_text(lab, "SubButton 2");
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(lab, lv_pct(100));
    lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);

    lv_scr_load(screen);
}

static lv_obj_t *spinbox;

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

static void btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto drawFct = reinterpret_cast<void (*)(void)>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (drawFct != NULL))
    {
        lv_obj_clean(lv_scr_act());
        drawFct();
    }

    if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            LV_LOG_USER("Button received ESC");
            lv_obj_clean(lv_scr_act());
            drawMainMenu();
        }
    }
}

static void swth_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);

        if (key == LV_KEY_ESC)
        {
            LV_LOG_USER("Switch received ESC");
            lv_obj_clean(lv_scr_act());
            drawMainMenu();
        }
    }
}

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
            lv_obj_clean(lv_scr_act());
            drawMainMenu();
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
            lv_obj_clean(lv_scr_act());
            drawMainMenu();
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
            lv_obj_clean(lv_scr_act());
            drawMainMenu();
        }
    }
}

void my_focus_cb(lv_group_t *group)
{
    lv_obj_t *focusedObject = lv_group_get_focused(group);
}

/// @brief build basic menu structure
void menu::initialize()
{
    /* create default group for button navigation and assign input device to it */
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(my_indev, group);
    lv_group_set_focus_cb(group, my_focus_cb);

    drawMainMenu();
    // drawValueModifier();
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
}

#endif