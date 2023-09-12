
#include "board_pins.hpp"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include "lvgl_ssd1306.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//TODO: lv_log_register_print_cb

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static constexpr unsigned char PROGMEM logo_bmp[] =
    {B00000000, B11000000,
     B00000001, B11000000,
     B00000001, B11000000,
     B00000011, B11100000,
     B11110011, B11100000,
     B11111110, B11111000,
     B01111110, B11111111,
     B00110011, B10011111,
     B00011111, B11111100,
     B00001101, B01110000,
     B00011011, B10100000,
     B00111111, B11100000,
     B00111111, B11110000,
     B01111100, B11110000,
     B01110000, B01110000,
     B00000000, B00110000};

#define XPOS 0 // Indexes into the 'icons' array in function below
#define YPOS 1
#define DELTAY 2

static int8_t icons[NUMFLAKES][3];

#if LV_USE_LOG != 0
/**
 * Log print function. Receives "Log Level", "File path", "Line number", "Function name" and "Description".
 */
void lvgl_log_to_serial(lv_log_level_t level, const char * file, uint32_t line, const char * function, const char * desc)
{
    Serial.printf("D%02i: %s <- %s@%s:%i ", level, desc, function, file, line);
    Serial.flush();
}
#endif

void setup_display()
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb( lvgl_log_to_serial ); /* register print function for debugging */
#endif
    Serial.println(F("LVGL: calling lv_init()"));
    lv_init();
    Serial.println(F("LVGL: lv_init done"));

    delay(100); // maybe this delay can be replaced by using a reset signal for the display (requires hardware modifications)
    Wire.begin(board::i2c_1::pin::sda, board::i2c_1::pin::scl);

    Serial.println(F("LVGL: calling lv_ssd1306_init()"));
    lv_ssd1306_init();
    Serial.println(F("LVGL: lv_ssd1306_init() done"));


    Serial.println(F("LVGL: calling lv_disp_buf_init()"));
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf_1[SCREEN_WIDTH * SCREEN_HEIGHT];
    lv_disp_buf_init(&disp_buf, buf_1, nullptr, SCREEN_WIDTH * SCREEN_HEIGHT);
    Serial.println(F("LVGL: lv_disp_buf_init() done"));

    Serial.println(F("LVGL: calling lv_disp_drv_register()"));
    lv_disp_drv_t disp_drv;                 /*A variable to hold the drivers. Can be local variable*/
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf;            /*Set an initialized buffer*/
    disp_drv.flush_cb = lv_ssd1306_flush;   /*Set a flush callback to draw to the display*/
    disp_drv.rounder_cb = lv_ssd1306_rounder;
    disp_drv.set_px_cb = lv_ssd1306_set_px_cb;
    lv_disp_t * disp;
    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
    Serial.println(F("LVGL: lv_disp_drv_register() done"));

    Serial.println(F("LVGL: calling lv_label_create()"));
    // create first text in lvgl
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, reinterpret_cast<const char *>(F("LVGL is up")));
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
    Serial.println(F("LVGL: lv_label_create() done"));

    lv_task_handler();

    // // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    // if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
    // {
    //     Serial.println(F("SSD1306 allocation failed"));
    //     for (;;)
    //         ; // Don't proceed, loop forever
    // }

    // // Show initial display buffer contents on the screen --
    // // the library initializes this with an Adafruit splash screen.
    // display.display();
    // delay(2000); // Pause for 2 seconds

    // // Clear the buffer
    // //display.clearDisplay();

    // // Show the display buffer on the screen. You MUST call display() after
    // // drawing commands to make them visible on screen!
    // display.display();

    // int8_t f;

    // // Initialize 'snowflake' positions
    // for (f = 0; f < NUMFLAKES; f++)
    // {
    //     icons[f][XPOS] = random(1 - LOGO_WIDTH, display.width());
    //     icons[f][YPOS] = -LOGO_HEIGHT;
    //     icons[f][DELTAY] = random(1, 6);
    //     Serial.print(F("x: "));
    //     Serial.print(icons[f][XPOS], DEC);
    //     Serial.print(F(" y: "));
    //     Serial.print(icons[f][YPOS], DEC);
    //     Serial.print(F(" dy: "));
    //     Serial.println(icons[f][DELTAY], DEC);
    // }

}

void refresh_display()
{
    lv_task_handler();
}