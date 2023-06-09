#pragma once

#include "IOutputShiftRegister.hpp"
#include <cstdint>

namespace osr
{
IOutputShiftRegister &getOutputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin);
}
