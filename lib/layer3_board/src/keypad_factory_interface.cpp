#include <board_config.hpp>
#include "keypad_factory_interface.hpp"
#include "Keypad.hpp"
#include "isr_factory_interface.hpp"

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
            board::isr::pin::latch,
            board::isr::pin::clock),
        inputPinToKeyId);
    return keypad;
}
} // namespace board