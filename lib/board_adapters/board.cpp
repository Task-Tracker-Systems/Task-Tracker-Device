#include "board.hpp"
#include "board_pins.hpp"
#include <Arduino.h>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <utility>

void board::setup(const HmiHandler callbackFunction)
{
    constexpr std::pair<PinType, board::HmiSelection> selectionForPins[] = {
        {board::button::pin::task1, board::HmiSelection::TASK1},
        {board::button::pin::task2, board::HmiSelection::TASK2},
        {board::button::pin::task3, board::HmiSelection::TASK3},
        {board::button::pin::task4, board::HmiSelection::TASK4},
        {board::button::pin::up, board::HmiSelection::UP},
        {board::button::pin::down, board::HmiSelection::DOWN},
        {board::button::pin::enter, board::HmiSelection::ENTER},
        {board::button::pin::back, board::HmiSelection::BACK},
    };
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
        board::buzzer::pin::on_off,
    };
    for (const auto &selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(selectionForPin.first),
                        std::bind(callbackFunction, selectionForPin.second),
                        FALLING);
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
        throw std::runtime_error();
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
