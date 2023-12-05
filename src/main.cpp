#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
#include <Arduino.h>
#include <cstdint>
#include <serial_port_interface.hpp>

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

void setup()
{
    serial_port::initialize();
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
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
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
            constexpr int minBrightness = 0;
            analogWrite(outputPins[event - 1], minBrightness);
        }
    }
    else
    {
        for (const auto pin : outputPins)
        {
            constexpr int maxBrightness = 255;
            constexpr int brightness = maxBrightness * 25 / 100.0;
            analogWrite(pin, brightness);
        }
    }
    delay(loopDurationMs);
    refresh_display(); // Animate bitmaps
}
