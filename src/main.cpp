#include "main.hpp"
#include "Controller.hpp"
#include "board_pins.hpp"
#include "display.h"
#include "keypad_factory_interface.hpp"
#include "pitches.hpp"
#include <Arduino.h>
#include <ShiftRegister74HC595.h>
#include <cstdint>

/**
 * Output shift registers with most significant bit first.
 */
ShiftRegister74HC595<1U> outputShiftRegister(board::osr::pin::data, board::osr::pin::clock, board::osr::pin::latch);

namespace main
{
void setup(char const *programIdentificationString)
{
    pinMode(board::buzzer::pin::on_off, OUTPUT);
    setup_display();
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
    Serial.printf("\n begin program '%s'\n", programIdentificationString);
}
void loop()
{
    static const Controller controller(board::getKeypad());
    const auto event = controller.checkHmiInput();
    if (event)
    {
        const std::uint8_t eventNumber = static_cast<std::uint8_t>(event.value()); //!< @TODO process ID instead of integer
        Serial.printf("Process event '%u'.\n", eventNumber);
        constexpr std::uint16_t notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
        const std::uint8_t newRegisterValue = 1 << (8 - eventNumber);
        outputShiftRegister.setAll(&newRegisterValue);
        tone(board::buzzer::pin::on_off, notes[eventNumber - 1], 250);
    }
    else
    {
        outputShiftRegister.setAllLow();
    }
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
