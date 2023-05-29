#include "IInputShiftRegister.hpp"
#include <cstdint>

namespace isr
{
IInputShiftRegister<8u> &getInputShiftRegister(const std::uint8_t pinNr_data, const std::uint8_t pinNr_load, const std::uint8_t pinNr_clock);
}