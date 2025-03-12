#include "Keypad.hpp"
#include <user_interaction/keypad_factory_interface.hpp>

namespace board
{
IKeypad &getKeypad()
{
    static Keypad keypad;
    return keypad;
}
} // namespace board
