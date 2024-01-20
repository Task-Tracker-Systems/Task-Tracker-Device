#include "sound_output.hpp"
#include <Arduino-wrapper.h>
#include <board_pins.hpp>
#include <user_interaction/board_interface.hpp>

void board::setup_sound()
{
    pinMode(board::buzzer::pin::on_off, OUTPUT);
}

void board::playTone(const unsigned int frequency, const std::chrono::milliseconds duration)
{
    tone(board::buzzer::pin::on_off, frequency, duration.count());
}
