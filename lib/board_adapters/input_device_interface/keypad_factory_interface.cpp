#include "Keypad.hpp"
#include <board_pins.hpp>
#include <user_interaction/keypad_factory_interface.hpp>

namespace board
{
const IKeypad &getKeypad(const HmiHandler callbackFunction)
{
    /*
     * This depends on the wiring on the board.
    */
    static constexpr KeyId inputPinToKeyId[]{
        KeyId::RIGHT,
        KeyId::BACK,
        KeyId::LEFT,
        KeyId::ENTER,
        KeyId::TASK4,
        KeyId::TASK3,
        KeyId::TASK2,
        KeyId::TASK1,
    };
    static const Keypad keypad(callbackFunction);
    return keypad;
}
} // namespace board
