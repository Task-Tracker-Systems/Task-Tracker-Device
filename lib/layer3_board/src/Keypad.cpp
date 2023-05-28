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

Keypad::Keypad(const InputShiftRegister &inputShiftRegisterToUse, const InputMapping &inputPinToKeyId)
    : inputShiftRegister(inputShiftRegisterToUse), inputMapping(inputPinToKeyId)
{
}

std::optional<Keypad::InputMapping::size_type> Keypad::getFirstPressedKeyIndex() const
{
    const auto registerValues = inputShiftRegister.readRegister();
    for (InputMapping::size_type index = 0u; index < registerValues.size(); ++index)
    {
        if (registerValues[index])
        {
            return index;
        }
    }
    return std::nullopt;
}
