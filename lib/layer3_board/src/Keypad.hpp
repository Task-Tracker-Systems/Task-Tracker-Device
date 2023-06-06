#pragma once

#include "IInputShiftRegister.hpp"
#include "IKeypad.hpp"
#include <cstddef>
#include <optional>

class Keypad : public IKeypad
{
  public:
    typedef IInputShiftRegister<8U> InputShiftRegister;

    virtual KeyId getCurrentlyPressedKey() const override;
    Keypad(const InputShiftRegister &inputShiftRegisterToUse, const KeyId (&inputPinToKeyId)[InputShiftRegister::numberOfBits]);

  private:
    const InputShiftRegister &inputShiftRegister;
    const KeyId (&inputMapping)[InputShiftRegister::numberOfBits];
    std::optional<std::size_t> getFirstPressedKeyIndex() const;
};
