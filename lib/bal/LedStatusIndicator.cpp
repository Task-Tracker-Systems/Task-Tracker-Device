#include "LedStatusIndicator.hpp"
#include <Arduino.h>

void LedStatusIndicator::off()
{
    digitalWrite(pin, LOW);
}

LedStatusIndicator::LedStatusIndicator(const PinType pinToUse)
    : pin(pinToUse)
{
}

void LedStatusIndicator::on()
{
    digitalWrite(pin, HIGH);
}
