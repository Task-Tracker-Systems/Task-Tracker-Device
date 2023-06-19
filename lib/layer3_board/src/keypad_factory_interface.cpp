#include "keypad_factory_interface.hpp"
#include "Keypad.hpp"
#include "isr_factory_interface.hpp"
#include <board_config.hpp>

namespace board
{
const IKeypad &getKeypad()
{
    /*
     * This depends on the wiring on the board.
    */
    static constexpr KeyId inputPinToKeyId[]{
        KeyId::TASK1,
        KeyId::TASK2,
        KeyId::TASK3,
        KeyId::TASK4,
        KeyId::ENTER,
        KeyId::LEFT,
        KeyId::BACK,
        KeyId::RIGHT,
    };
    static const Keypad keypad(
        ::isr::getInputShiftRegister(
            board::isr::pin::data,
            board::isr::pin::clock,
            board::isr::pin::latch),
        inputPinToKeyId);
    return keypad;
}
} // namespace board
