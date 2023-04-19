#include "main.hpp"
#include "display.h"
#include "pitches.hpp"
//#include <ESP32Tone.h>
#include <Arduino.h>
#include <cmath>
#include <cstdint>

namespace OutputShiftRegister
{
constexpr int pin_data = 14;
constexpr int pin_latch = 13;
constexpr int pin_clock = 5;

void setup()
{
    pinMode(pin_data, OUTPUT);
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
}

void setRegister(const uint8_t data)
{
    digitalWrite(pin_latch, LOW);
    shiftOut(pin_data, pin_clock, LSBFIRST, data);
    digitalWrite(pin_latch, HIGH);
}
} // namespace OutputShiftRegister

namespace InputShiftRegister
{
constexpr int pin_data = 34;
constexpr int pin_latch = 12;
constexpr int pin_clock = 23;

void setup()
{
    pinMode(pin_data, INPUT);
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
}

typedef decltype(shiftIn(0, 0, LSBFIRST)) RegisterType;

RegisterType getRegister()
{
    digitalWrite(pin_latch, LOW);
    delayMicroseconds(1); // at least 200ns
    digitalWrite(pin_latch, HIGH);
    return shiftIn(pin_data, pin_clock, LSBFIRST);
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

constexpr int pin_buzzer = 4;

namespace main
{
void setup(char const *programIdentificationString)
{
    // put your setup code here, to run once:
    pinMode(pin_buzzer, OUTPUT);
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
        constexpr std::uint16_t notes[] = {note::C4, note::G3, note::A3, note::B3, note::D1, note::E1, note::F1, note::G1};
        OutputShiftRegister::setRegister(1 << (event - 1));
        tone(pin_buzzer, notes[event - 1], 250);
    }
    else
    {
        OutputShiftRegister::setRegister(0);
    }
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
