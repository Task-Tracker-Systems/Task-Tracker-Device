
#include "board_pins.hpp"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 16];

#if LV_USE_LOG != 0
/* Serial debugging */
void lvgl_log_to_serial(const char *buf)
{
    Serial.printf(buf);
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
        Serial.println(F("SSD1306 allocation failed"));
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
    delay(200);

    // create first text in lvgl
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL is up");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // display lvgl screen
    lv_timer_handler();
}

void refresh_display()
{
    lv_timer_handler();
}