#include "Keypad.hpp"
#include <Arduino.h>

KeyId Keypad::getCurrentlyPressedKey() const
{
    const auto keyIndex = getFirstPressedKeyIndex();
    if (keyIndex)
    {
        return inputMapping[keyIndex.value()];
    }
    else
    {
        return KeyId::NONE;
    }
}

Keypad::Keypad(const KeyId (&inputPinToKeyId)[numberOfPins])
    : inputMapping(inputPinToKeyId)
{
    for (const auto pin : inputPins)
    {
        pinMode(pin, INPUT_PULLUP);
    }
}

std::optional<std::size_t> Keypad::getFirstPressedKeyIndex() const
{
    static board::PinType oldValue = 0U;
    const board::PinType currentValue = oldValue;
    board::PinType newValue = 0U;

    std::uint8_t candidateEvent = 1;
    for (const auto pin : inputPins)
    {
        if (digitalRead(pin) == LOW) // buttons are active low
        {
            newValue = candidateEvent;
            break;
        }
        candidateEvent++;
    }

    oldValue = newValue;
    if (newValue != 0 && currentValue == 0)
    {
        return newValue-1;
    }
    else
    {
        return std::nullopt;
    }
}
