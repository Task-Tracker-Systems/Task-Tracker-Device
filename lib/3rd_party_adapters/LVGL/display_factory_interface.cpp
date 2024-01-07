#include "Display.hpp"
#include <Arduino.h>
#include <board_pins.hpp>
#include <user_interaction/display_factory_interface.hpp>

namespace board
{
IDisplay &getDisplay()
{
    Wire.begin(board::i2c_1::pin::sda, board::i2c_1::pin::scl);
    constexpr Display::Configuration configuration = {
        .screen_width = 128,
        .screen_height = 64,
        .generateDisplayVoltageInternally = true,
        .display_i2c_address = 0x3C,
    };
    static Display singleton(configuration, Wire);
    return singleton;
}
} // namespace board
