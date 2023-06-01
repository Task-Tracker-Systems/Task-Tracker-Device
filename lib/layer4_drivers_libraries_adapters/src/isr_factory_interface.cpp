#include "isr_factory_interface.hpp"
#include "InputShiftRegister.hpp"
#include <RoxMux.h>

namespace isr
{
IInputShiftRegister<8> &getInputShiftRegister(const std::uint8_t pinNr_data, const std::uint8_t pinNr_load, const std::uint8_t pinNr_clock)
{
    static InputShiftRegister<1U>::Backend isrBackend;
    isrBackend.begin(pinNr_data, pinNr_load, pinNr_clock);
    static InputShiftRegister<1U> singleIsr(isrBackend);
    return isrBackend;
}

} // namespace isr