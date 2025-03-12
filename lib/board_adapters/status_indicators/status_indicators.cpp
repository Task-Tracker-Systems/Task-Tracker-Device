#include "status_indicators.hpp"
#include <Arduino.h>

void LedStatusIndicator::off()
{
    constexpr int minBrightness = 0;
    analogWrite(pin, minBrightness);
}

LedStatusIndicator::LedStatusIndicator(const board::PinType pinToUse)
    : pin(pinToUse)
{
    pinMode(pin, OUTPUT);
}

void LedStatusIndicator::on()
{
    constexpr int maxBrightness = 255;
    constexpr int brightness = maxBrightness * 25 / 100.0;
    analogWrite(pin, brightness);
}
