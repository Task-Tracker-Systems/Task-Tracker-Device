#include "keypad_factory_interface.hpp"
#include "Keypad.hpp"
#include <board_config.hpp>

namespace board
{
const IKeypad &getKeypad()
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
    static const Keypad keypad(inputPinToKeyId);
    return keypad;
}
} // namespace board
