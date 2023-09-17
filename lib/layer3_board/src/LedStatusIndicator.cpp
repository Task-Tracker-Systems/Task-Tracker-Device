#include "LedStatusIndicator.hpp"
#include <Arduino.h>

void LedStatusIndicator::off()
{
    constexpr int maxBrightness = 255;
    constexpr int brightness = maxBrightness * 25 / 100.0;
    analogWrite(pin, brightness);
}

LedStatusIndicator::LedStatusIndicator(const board::PinType pinToUse)
    : pin(pinToUse)
{
    pinMode(pin, OUTPUT);
}

void LedStatusIndicator::on()
{
    constexpr int minBrightness = 0;
    analogWrite(pin, minBrightness);
}
