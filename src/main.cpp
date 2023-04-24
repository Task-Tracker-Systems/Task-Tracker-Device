#include "main.hpp"
#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
//#include <ESP32Tone.h>
#include <Arduino.h>
#include <cmath>
#include <cstdint>

namespace OutputShiftRegister
{
using namespace board::osr;

void setup()
{
    pinMode(pin::data, OUTPUT);
    pinMode(pin::latch, OUTPUT);
    pinMode(pin::clock, OUTPUT);
}

void setRegister(const uint8_t data)
{
    digitalWrite(pin::latch, LOW);
    shiftOut(pin::data, pin::clock, LSBFIRST, data);
    digitalWrite(pin::latch, HIGH);
}
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
    OutputShiftRegister::setup();
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
        OutputShiftRegister::setRegister(1 << (event - 1));
        tone(board::buzzer::pin::on_off, notes[event - 1], 250);
    }
    else
    {
        OutputShiftRegister::setRegister(0);
    }
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
