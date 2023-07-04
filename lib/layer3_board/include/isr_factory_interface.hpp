#include "IInputShiftRegister.hpp"
#include <cstdint>

namespace isr
{
IInputShiftRegister<8u> &getInputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin);
}
