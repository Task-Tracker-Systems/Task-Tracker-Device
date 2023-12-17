#pragma once

#include "IDisplay.hpp"
#include <Adafruit_SSD1306.h>
#include <cstdint>
#include <lvgl.h>
#include <memory>

class Display : public IDisplay
{
  public:
    struct Configuration
    {
        std::uint8_t screen_width;
        std::uint8_t screen_height;
        bool generateDisplayVoltageInternally;
        std::uint8_t display_i2c_address;
    };
    Display(const Configuration &configuration, TwoWire &i2c);
    virtual void refresh() override;

    Adafruit_SSD1306 display;

  private:
    const std::unique_ptr<lv_color_t[]> buf;
};
