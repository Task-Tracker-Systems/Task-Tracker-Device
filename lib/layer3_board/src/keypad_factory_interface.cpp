#include "keypad_factory_interface.hpp"
#include "Keypad.hpp"
#include "board_pins.hpp"
#include "isr_factory_interface.hpp"

namespace board
{
const IKeypad &getKeypad()
{
    constexpr Keypad::InputMapping inputPinToKeyId{KeyId::BACK,
                                                   KeyId::ENTER,
                                                   KeyId::LEFT,
                                                   KeyId::RIGHT,
                                                   KeyId::TASK1,
                                                   KeyId::TASK2,
                                                   KeyId::TASK3,
                                                   KeyId::TASK4};
    static const Keypad keypad(
        ::isr::getInputShiftRegister(
            board::isr::pin::data,
            board::isr::pin::latch,
            board::isr::pin::clock),
        inputPinToKeyId);
    return keypad;
}
} // namespace board