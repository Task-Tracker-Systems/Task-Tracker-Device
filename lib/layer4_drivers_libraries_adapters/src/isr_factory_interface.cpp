#include "isr_factory_interface.hpp"
#include "InputShiftRegister.hpp"

namespace isr
{
IInputShiftRegister<8> &getInputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin)
{
    static InputShiftRegister<1U> singleton(serialDataPin, clockPin, latchPin);
    return singleton;
}

} // namespace isr
