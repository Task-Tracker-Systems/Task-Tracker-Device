#pragma once

#include "IDisplay.hpp"
#include <Adafruit_SSD1306.h>
#include <cstdint>

class Display : public IDisplay
{
  public:
    Display();

  private:
    Adafruit_SSD1306 display;
};
