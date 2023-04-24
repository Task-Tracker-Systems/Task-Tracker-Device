#include "main.hpp"
#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
//#include <ESP32Tone.h>
#include <Arduino.h>
#include <ShiftRegister74HC595.h>
#include <cmath>
#include <cstdint>

namespace OutputShiftRegister
{
using namespace board::osr;

/**
 * Output shift registers with most significant bit first.
 */
ShiftRegister74HC595<1U> outputShiftRegister(pin::data, pin::clock, pin::latch);

} // namespace OutputShiftRegister

namespace InputShiftRegister
{
using namespace board::isr;

void setup()
{
    pinMode(pin::data, INPUT);
    pinMode(pin::latch, OUTPUT);
    pinMode(pin::clock, OUTPUT);
}

typedef decltype(shiftIn(0, 0, LSBFIRST)) RegisterType;

RegisterType getRegister()
{
    digitalWrite(pin::latch, LOW);
    delayMicroseconds(1); // at least 200ns
    digitalWrite(pin::latch, HIGH);
    return shiftIn(pin::data, pin::clock, LSBFIRST);
}
} // namespace InputShiftRegister

/**
 *
 * @return 1-8 or 0 in case of no event
 */
static std::uint8_t getEvent()
{
    static InputShiftRegister::RegisterType oldValue = 0;
    const InputShiftRegister::RegisterType newValue = InputShiftRegister::getRegister();
    std::uint8_t result = 0;
    if (newValue != 0 && oldValue == 0)
    {
        result = std::log2(newValue << 1);
    }
    oldValue = newValue;
    return result;
}

namespace main
{
void setup(char const *programIdentificationString)
{
    // put your setup code here, to run once:
    pinMode(board::buzzer::pin::on_off, OUTPUT);
    InputShiftRegister::setup();
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
        constexpr std::uint16_t notes[] = {note::c4, note::g3, note::a3, note::b3, note::d1, note::e1, note::f1, note::g1};
        const std::uint8_t newRegisterValue = 1 << (8 - event);
        OutputShiftRegister::outputShiftRegister.setAll(&newRegisterValue);
        tone(board::buzzer::pin::on_off, notes[event - 1], 250);
    }
    else
    {
        OutputShiftRegister::outputShiftRegister.setAllLow();
    }
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
