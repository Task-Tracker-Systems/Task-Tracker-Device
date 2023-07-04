#include "Display.hpp"
#include <Adafruit_SSD1306.h>
#include <cassert>

Display::Display(const Configuration &configuration, TwoWire &i2c)
    : display(configuration.screen_width, configuration.screen_height, &i2c)
{
    const std::uint8_t switchVcc = configuration.generateDisplayVoltageInternally ? SSD1306_SWITCHCAPVCC : SSD1306_EXTERNALVCC;
    const bool allocationSuccessful = display.begin(switchVcc, configuration.display_i2c_address);
    assert(allocationSuccessful);
    display.display();
}
