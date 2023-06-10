#include "Display.hpp"
#include "board_config.hpp"
#include <Adafruit_SSD1306.h>

static constexpr unsigned int screen_width_pixel = 128;
static constexpr unsigned int screen_height_pixel = 64;

Display::Display()
    : display(screen_width_pixel, screen_height_pixel, &Wire)
{
    Wire.begin(board::i2c_1::pin::sda, board::i2c_1::pin::scl);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    //display.clearDisplay();

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
}
