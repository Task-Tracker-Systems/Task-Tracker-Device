#pragma once
#include "ShiftRegisterStatusIndicator.hpp"
#include <Arduino.h>

template <class ShiftRegister>
void ShiftRegisterStatusIndicator<ShiftRegister>::off()
{
    shiftRegister.set(pin, LOW);
}

template <class ShiftRegister>
ShiftRegisterStatusIndicator<ShiftRegister>::ShiftRegisterStatusIndicator(ShiftRegister &outputShiftRegister, const std::uint8_t pinToUse)
    : shiftRegister(outputShiftRegister), pin(pinToUse)
{
}

template <class ShiftRegister>
void ShiftRegisterStatusIndicator<ShiftRegister>::on()
{
    shiftRegister.set(pin, HIGH);
}
