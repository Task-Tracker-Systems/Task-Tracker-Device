#pragma once
#include "IStatusIndicator.hpp"
#include <cstdint>

template <class OutputShiftRegister>
class ShiftRegisterStatusIndicator : public IStatusIndicator
{
  public:
    ShiftRegisterStatusIndicator(OutputShiftRegister &outputShiftRegister, const std::uint8_t pin);
    virtual void off() override;
    virtual void on() override;

  private:
    OutputShiftRegister &shiftRegister;
    const std::uint8_t pin;
};

template <class OutputShiftRegister>
ShiftRegisterStatusIndicator<OutputShiftRegister> createShiftRegisterStatusIndicator(OutputShiftRegister &outputShiftRegister, const std::uint8_t pin)
{
    return ShiftRegisterStatusIndicator<OutputShiftRegister>(outputShiftRegister, pin);
}

#include "ShiftRegisterStatusIndicator_implementation.hpp"
