#include "status_indicators.hpp"
#include <Arduino.h>
#include <board_pins.hpp>
#include <user_interaction/board_interface.hpp>

void board::setup_status_indicators()
{
    // output pins
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
    };
    for (const auto outputPin : outputPins)
    {
        pinMode(outputPin, OUTPUT);
    }
}

static PinType mapIndicatorIdToPin(const board::StatusIndicatorId id)
{
    switch (id)
    {
    case board::StatusIndicatorId::TASK1:
        return board::led::pin::task1;
        break;
    case board::StatusIndicatorId::TASK2:
        return board::led::pin::task2;
        break;
    case board::StatusIndicatorId::TASK3:
        return board::led::pin::task3;
        break;
    case board::StatusIndicatorId::TASK4:
        return board::led::pin::task4;
        break;
    default:
        throw std::runtime_error("no pin defined for selected status indicator");
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
