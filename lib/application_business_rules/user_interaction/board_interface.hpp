#pragma once

#include <chrono>
#include <functional>

namespace board
{
enum class HmiSelection
{
    TASK1,
    TASK2,
    TASK3,
    TASK4,
    UP,
    DOWN,
    ENTER,
    BACK,
};

enum class StatusIndicatorId
{
    TASK1,
    TASK2,
    TASK3,
    TASK4,
};

typedef std::function<void(HmiSelection)> HmiHandler;

void setup(const HmiHandler callbackFunction);

void playTone(const unsigned int frequency, const std::chrono::milliseconds duration);

void setStatusIndicator(const StatusIndicatorId indicatorId, const bool toActive);

} // namespace board
