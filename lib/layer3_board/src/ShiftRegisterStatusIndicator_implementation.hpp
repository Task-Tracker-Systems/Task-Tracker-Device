#pragma once
#include "ShiftRegisterStatusIndicator.hpp"
#include <Arduino.h>

template <class OutputShiftRegister>
void ShiftRegisterStatusIndicator<OutputShiftRegister>::off()
{
    shiftRegister.set(pin, LOW);
}

template <class OutputShiftRegister>
ShiftRegisterStatusIndicator<OutputShiftRegister>::ShiftRegisterStatusIndicator(OutputShiftRegister &outputShiftRegister, const std::uint8_t pinToUse)
    : shiftRegister(outputShiftRegister), pin(pinToUse)
{
}

template <class OutputShiftRegister>
void ShiftRegisterStatusIndicator<OutputShiftRegister>::on()
{
    shiftRegister.set(pin, HIGH);
}
