#pragma once
#include "StatusIndicator.hpp"
#include <cstdint>

template <class ShiftRegister>
class ShiftRegisterStatusIndicator : public StatusIndicator
{
  public:
    ShiftRegisterStatusIndicator(const ShiftRegister &outputShiftRegister, const std::uint8_t pin);
    virtual void off() override;
    virtual void on() override;

  private:
    ShiftRegister &shiftRegister;
    const std::uint8_t pin;
};

template <class ShiftRegister>
ShiftRegisterStatusIndicator<ShiftRegister> createShiftRegisterStatusIndicator(const ShiftRegister &outputShiftRegister, const std::uint8_t pin)
{
    return ShiftRegisterStatusIndicator<ShiftRegister>(outputShiftRegister, pin);
}

#include "ShiftRegisterStatusIndicator_implementation.hpp"
