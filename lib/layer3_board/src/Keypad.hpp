#pragma once

#include "IInputShiftRegister.hpp"
#include "IKeypad.hpp"
#include <array>
#include <optional>

class Keypad : public IKeypad
{
  public:
    typedef IInputShiftRegister<8U> InputShiftRegister;
    /**
     * For mapping of input pin to key ID.
     */
    typedef std::array<KeyId, InputShiftRegister::numberOfBits> InputMapping;

    virtual KeyId getCurrentlyPressedKey() const override;
    Keypad(const InputShiftRegister &inputShiftRegisterToUse, const InputMapping &inputPinToKeyId);

  private:
    const InputShiftRegister &inputShiftRegister;
    const InputMapping &inputMapping;
    std::optional<InputMapping::size_type> getFirstPressedKeyIndex() const;
};
