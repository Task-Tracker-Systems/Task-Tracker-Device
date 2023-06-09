#include "Keypad.hpp"

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

Keypad::Keypad(const InputShiftRegister &inputShiftRegisterToUse, const KeyId (&inputPinToKeyId)[InputShiftRegister::numberOfBits])
    : inputShiftRegister(inputShiftRegisterToUse), inputMapping(inputPinToKeyId)
{
}

std::optional<std::size_t> Keypad::getFirstPressedKeyIndex() const
{
    const auto registerValues = inputShiftRegister.readRegister();
    for (std::size_t index = 0u; index < registerValues.size(); ++index)
    {
        if (registerValues[index])
        {
            return index;
        }
    }
    return std::nullopt;
}
