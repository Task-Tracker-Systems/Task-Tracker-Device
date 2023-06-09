#include "main.hpp"
#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
#include <Arduino.h>
#include <RoxMux.h>
#include <ShiftRegister74HC595.h>
#include <cstdint>

/**
 * Output shift registers with most significant bit first.
 */
ShiftRegister74HC595<1U> outputShiftRegister(board::osr::pin::data, board::osr::pin::clock, board::osr::pin::latch);

/**
 * Input shift register.
 */
Rox74HC165<1U> inputShiftRegister;

/**
 * @returns 1-8 or 0 in case of no event
 */
static std::uint8_t getEvent()
{
    static std::uint8_t oldValue = 0U;
    std::uint8_t newValue = 0U;

    inputShiftRegister.update();
    const auto numberOfPins = inputShiftRegister.getLength();
    for (std::uint8_t newValueCand = numberOfPins; newValueCand > 0; newValueCand--)
    {
        if (inputShiftRegister.readPin(numberOfPins - newValueCand))
        {
            newValue = newValueCand;
            break;
        }
    }

    std::uint8_t result = 0;
    if (newValue != 0 && oldValue == 0)
    {
        result = newValue;
    }
    oldValue = newValue;
    return result;
}

namespace main
{
void setup(char const *programIdentificationString)
{
    pinMode(board::buzzer::pin::on_off, OUTPUT);
    inputShiftRegister.begin(board::isr::pin::data, board::isr::pin::latch, board::isr::pin::clock);
    setup_display();
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
    Serial.printf("\n begin program '%s'\n", programIdentificationString);
}
void loop()
{
    const auto event = getEvent();
    if (event)
    {
        Serial.printf("Process event '%u'.\n", event);
        constexpr std::uint16_t notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
        const std::uint8_t newRegisterValue = 1 << (8 - event);
        outputShiftRegister.setAll(&newRegisterValue);
        tone(board::buzzer::pin::on_off, notes[event - 1], 250);
    }
    else
    {
        outputShiftRegister.setAllLow();
    }
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
