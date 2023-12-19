#include "HMI_Menu.hpp"
#include <lvgl.h>

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked\r");
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        LV_LOG_USER("Toggled\r");
    }
}

static lv_obj_t *btn1;
static lv_obj_t *btn2;
static lv_obj_t *global_text;

void menu::initialize()
{

    static lv_style_t label_style;
    lv_style_init(&label_style);
    // lv_style_set_text_font(&style, &lv_font_montserrat_10);
    lv_style_set_text_font(&label_style, &lv_font_unscii_8);

    static lv_style_t button_idle;
    lv_style_init(&button_idle);
    lv_style_set_text_font(&button_idle, &lv_font_unscii_8);
    lv_style_set_bg_color(&button_idle, lv_color_black());
    lv_style_set_text_color(&button_idle, lv_color_white());
    lv_style_set_outline_color(&button_idle, lv_color_white());

    static lv_style_t button_pressed;
    lv_style_init(&button_pressed);
    lv_style_set_text_font(&button_pressed, &lv_font_unscii_8);
    lv_style_set_bg_color(&button_pressed, lv_color_white());
    lv_style_set_text_color(&button_pressed, lv_color_black());
    lv_style_set_outline_color(&button_pressed, lv_color_black());

    static lv_style_t button_focused;
    lv_style_init(&button_focused);
    lv_style_set_text_font(&button_focused, &lv_font_unscii_8);
    lv_style_set_bg_color(&button_idle, lv_color_black());
    lv_style_set_text_color(&button_idle, lv_color_white());
    lv_style_set_outline_color(&button_focused, lv_color_black());
    lv_style_set_outline_width(&button_focused, 3);

    lv_obj_t *label;

    label = lv_label_create(lv_scr_act());
    lv_label_set_text_static(label, "Title");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(label, &label_style, 0);

    btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_width(btn1, 100);
    lv_obj_set_height(btn1, 14);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -8);
    lv_obj_add_style(btn1, &button_idle, LV_STATE_DEFAULT);
    lv_obj_add_style(btn1, &button_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn1, &button_focused, LV_STATE_FOCUSED);

    label = lv_label_create(btn1);
    lv_label_set_text_static(label, "Button");
    lv_obj_center(label);

    btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_width(btn2, 100);
    lv_obj_set_height(btn2, 14);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 8);
    lv_obj_add_style(btn2, &button_idle, LV_STATE_DEFAULT);
    lv_obj_add_style(btn2, &button_pressed, LV_STATE_PRESSED);
    lv_obj_add_style(btn2, &button_focused, LV_STATE_FOCUSED);

    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    // lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text_static(label, "Toggle");
    lv_obj_center(label);

    global_text = lv_label_create(lv_scr_act());
    lv_label_set_text_static(global_text, "some text");
    lv_obj_align(global_text, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(global_text, &label_style, 0);
}

void menu::cyclic()
{
    static uint32_t count = 0;
    count += 1;

    switch (count)
    {
    case 20:
        lv_event_send(btn1, LV_EVENT_FOCUSED, 0);
        lv_label_set_text_fmt(global_text, "Focus1");
        break;
    case 30:
        lv_event_send(btn1, LV_EVENT_DEFOCUSED, 0);
        lv_event_send(btn2, LV_EVENT_FOCUSED, 0);
        lv_label_set_text_fmt(global_text, "Focus2");
        break;
    case 40:
        lv_event_send(btn2, LV_EVENT_DEFOCUSED, 0);
        lv_label_set_text_fmt(global_text, "Unfocus");
        break;
    case 50:
        lv_event_send(btn1, LV_EVENT_CLICKED, 0);
        lv_label_set_text_fmt(global_text, "click1");
        break;
    case 60:
        lv_event_send(btn1, LV_EVENT_DEFOCUSED, 0);
        lv_label_set_text_fmt(global_text, "default?");
        break;
    case 80:
        lv_event_send(btn2, LV_EVENT_VALUE_CHANGED, 0);
        lv_label_set_text_fmt(global_text, "change value btn2");
        break;

    case 120:
        count = 0;
        lv_label_set_text_fmt(global_text, "Reset cycle");
        break;

    default:
        break;
    }
}