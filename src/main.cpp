#include "main.hpp"
#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
#include <Arduino.h>
#include <cstdint>

/**
 * Events are the beginning of a pressed button.
 * 
 * @returns 1-8 or 0 in case of no event
 */
static std::uint8_t getEvent()
{
    constexpr PinType inputPins[] = {
        board::button::pin::task1,
        board::button::pin::task2,
        board::button::pin::task3,
        board::button::pin::task4,
        board::button::pin::up,
        board::button::pin::down,
        board::button::pin::enter,
        board::button::pin::back,
    };
    static PinType oldValue = 0U;
    PinType newValue = 0U;
    std::uint8_t result = 0U;

    std::uint8_t candidateEvent = 1;
    for (const auto pin : inputPins)
    {
        if (digitalRead(pin) == LOW) // buttons are active low
        {
            newValue = candidateEvent;
        }
        candidateEvent++;
    }

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
    pinMode(board::button::pin::up, INPUT_PULLUP);
    pinMode(board::button::pin::down, INPUT_PULLUP);
    pinMode(board::button::pin::enter, INPUT_PULLUP);
    pinMode(board::button::pin::back, INPUT_PULLUP);
    pinMode(board::button::pin::task1, INPUT_PULLUP);
    pinMode(board::button::pin::task2, INPUT_PULLUP);
    pinMode(board::button::pin::task3, INPUT_PULLUP);
    pinMode(board::button::pin::task4, INPUT_PULLUP);
    pinMode(board::led::pin::task1, OUTPUT);
    pinMode(board::led::pin::task2, OUTPUT);
    pinMode(board::led::pin::task3, OUTPUT);
    pinMode(board::led::pin::task4, OUTPUT);
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
    constexpr unsigned long loopDurationMs = 250;
    const auto event = getEvent();
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
    };
    if (event)
    {
        Serial.printf("Process event '%u'.\n", event);
        constexpr std::uint16_t notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
        tone(board::buzzer::pin::on_off, notes[event - 1], loopDurationMs);
        if (event < sizeof(outputPins) / sizeof(outputPins[0]) + 1)
        {
            digitalWrite(outputPins[event - 1], HIGH);
        }
    }
    else
    {
        for (const auto pin : outputPins)
        {
            digitalWrite(pin, LOW);
        }
    }
    delay(loopDurationMs);
    testanimate(); // Animate bitmaps
}
} // namespace main
