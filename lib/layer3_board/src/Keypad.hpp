#pragma once

#include "IInputShiftRegister.hpp"
#include "IKeypad.hpp"
#include <array>
#include <optional>

class Keypad : public IKeypad
{
  public:
    typedef IInputShiftRegister<8U> InputShiftRegister;
    typedef std::array<KeyId, InputShiftRegister::numberOfBits> InputMapping;

    virtual KeyId getCurrentlyPressedKey() const override;
    Keypad(const InputShiftRegister &inputShiftRegisterToUse, const InputMapping &inputMappingToUse);

  private:
    const InputShiftRegister &inputShiftRegister;
    const InputMapping &inputMapping;
    std::optional<InputMapping::size_type> getFirstPressedKeyIndex() const;
};
