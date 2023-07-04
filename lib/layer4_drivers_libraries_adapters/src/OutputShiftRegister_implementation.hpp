#include "OutputShiftRegister.hpp"
#include "framework_config.hpp"

template <std::size_t numberOfBytes>
OutputShiftRegister<numberOfBytes>::OutputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin)
    : outputShiftRegister(serialDataPin, clockPin, latchPin)
{
}

template <std::size_t numberOfBytes>
void OutputShiftRegister<numberOfBytes>::writePin(const IOutputShiftRegister::BitNumber bitNumber, const bool set)
{
    outputShiftRegister.set(bitNumber, set ? high_level : low_level);
}
