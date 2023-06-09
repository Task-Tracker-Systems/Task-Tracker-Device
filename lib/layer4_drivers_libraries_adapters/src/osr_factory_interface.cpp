#pragma once

#include "osr_factory_interface.hpp"
#include "OutputShiftRegister.hpp"

namespace osr
{
IOutputShiftRegister &getOutputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin)
{
    static OutputShiftRegister<1U> singleton(serialDataPin, clockPin, latchPin);
    return singleton;
}
} // namespace osr
