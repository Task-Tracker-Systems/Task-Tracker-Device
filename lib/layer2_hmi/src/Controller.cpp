#include "Controller.hpp"

KeyId Controller::checkHmiInput() const
{
    static KeyId lastPressedKey = KeyId::NONE;
    const KeyId currentlyPressedKey = keypad.getCurrentlyPressedKey();
    KeyId identifiedEvent = KeyId::NONE;
    if (currentlyPressedKey != lastPressedKey && currentlyPressedKey == KeyId::NONE)
    {
        identifiedEvent = currentlyPressedKey;
    }
    lastPressedKey = currentlyPressedKey;
    return identifiedEvent;
}
