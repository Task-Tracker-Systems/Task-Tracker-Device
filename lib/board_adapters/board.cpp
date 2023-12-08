#include "board.hpp"
#include "board_pins.hpp"
#include <Arduino.h>

void board::setHmiHandler(const HmiHandler callbackFunction)
{
}

static void inputIsr()
{
}

void board::setup()
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
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
        board::buzzer::pin::on_off,
    };
    for (const auto inputPin : inputPins)
    {
        pinMode(inputPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(inputPin), &inputIsr, FALLING);
    }
    for (const auto outputPin : outputPins)
    {
        pinMode(outputPin, OUTPUT);
    }
}

void board::playTone(const unsigned int frequency, const std::chrono::milliseconds duration)
{
    tone(board::buzzer::pin::on_off, frequency, duration.count());
}

static PinType mapIndicatorIdToPin(const board::StatusIndicatorId id)
{
    switch (id)
    {
    case board::StatusIndicatorId::TASK1:
        return board::led::pin::task1;
        break;
    default:
        break;
    }
}

void board::setStatusIndicator(const StatusIndicatorId indicatorId, const bool toActive)
{
    const auto pin = mapIndicatorIdToPin(indicatorId);
    if (!toActive)
    {
        constexpr int minBrightness = 0;
        analogWrite(pin, minBrightness);
    }
    else
    {
        constexpr int maxBrightness = 255;
        constexpr int brightness = maxBrightness * 25 / 100.0;
        analogWrite(pin, brightness);
    }
}
