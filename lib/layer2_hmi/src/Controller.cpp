#include "Controller.hpp"

std::optional<KeyId> Controller::checkHmiInput() const
{
    static KeyId lastPressedKey = KeyId::NONE;
    const KeyId currentlyPressedKey = keypad.getCurrentlyPressedKey();
    KeyId identifiedEvent = KeyId::NONE;
    if (currentlyPressedKey != lastPressedKey && currentlyPressedKey == KeyId::NONE)
    {
        identifiedEvent = lastPressedKey;
    }
    lastPressedKey = currentlyPressedKey;
    if (identifiedEvent != KeyId::NONE)
    {
        return identifiedEvent;
    }
    else
    {
        return std::nullopt;
    }
}

Controller::Controller(const IKeypad &keypadToBeUsed)
    : keypad(keypadToBeUsed)
{
}
