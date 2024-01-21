
#include "HMI_Menu.hpp"
#include "board_pins.hpp"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

static menu mainMenu;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, board::i2c_1::pin::res);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 16];

#if LV_USE_LOG != 0
/* Serial debugging */
void lvgl_log_to_serial(const char *buf)
{
    Serial.printf("%s\r", buf);
    Serial.flush();
}
#endif

/* Display flushing */
void flushSSD1306Adafruit(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t width = (area->x2 - area->x1 + 1);
    uint32_t height = (area->y2 - area->y1 + 1);

    for (uint16_t row = 0; row < height; row++)
    {
        for (uint16_t col = 0; col < width; col++)
        {
            display.drawPixel(area->x1 + col, area->y1 + row, (color_p->full) ? SSD1306_WHITE : SSD1306_BLACK);
            color_p++;
        }
    }
    display.display();
    lv_disp_flush_ready(disp_drv);
}

extern uint8_t KEYID;

static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = KEYID;
    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;

        /*Translate the keys to LVGL control characters according to your key definitions*/
        switch (act_key)
        {
        case 5:
            act_key = LV_KEY_LEFT;
            break;
        case 6:
            act_key = LV_KEY_RIGHT;
            break;
        case 7:
            act_key = LV_KEY_ENTER;
            break;
        case 8:
            act_key = LV_KEY_ESC;
            break;
        }

        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    data->key = last_key;
}

lv_indev_t *my_indev;

void setup_display()
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb(lvgl_log_to_serial); /* register print function for debugging */
#endif

    delay(100); // maybe this delay can be replaced by using a reset signal for the display (requires hardware modifications)
    Wire.begin(board::i2c_1::pin::sda, board::i2c_1::pin::scl);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
    {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(500); // Pause for short time

    // Clear the buffer
    display.clearDisplay();

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();

    // Initialize lvgl library
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 16);

    // Initialize the display driver for lvgl
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = flushSSD1306Adafruit;
    disp_drv.draw_buf = &draw_buf;

    // Register display driver to lvgl
    lv_disp_drv_register(&disp_drv);
    lv_theme_t *mono_theme = lv_theme_mono_init(0, false, &lv_font_unscii_8);
    lv_disp_set_theme(0, mono_theme);

    delay(200);

    /*Register at least one display before you register any input devices*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = keypad_read;
    /*Register the driver in LVGL and save the created input device object*/
    my_indev = lv_indev_drv_register(&indev_drv);

    // create first text in lvgl
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL is up");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // display lvgl screen
    lv_timer_handler();

    delay(2000);
    lv_obj_clean(lv_scr_act());

    mainMenu.initialize();
    Serial.print("init done");
}

void refresh_display()
{
    mainMenu.cyclic();
    lv_timer_handler();
}