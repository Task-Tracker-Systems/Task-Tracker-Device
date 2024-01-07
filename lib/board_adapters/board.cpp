#include "input_device_interface/input_devices.hpp"
#include "sound_output_interface/sound_output.hpp"
#include "status_indicators/status_indicators.hpp"
#include <user_interaction/board_interface.hpp>

void board::setup(const HmiHandler callbackFunction)
{
    board::setup_input_devices(callbackFunction);
    board::setup_status_indicators();
    board::setup_sound();
}
