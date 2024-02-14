#pragma once

#include "KeyIds.hpp"
#include <chrono>
#include <functional>

namespace board
{

enum class StatusIndicatorId
{
    TASK1,
    TASK2,
    TASK3,
    TASK4,
};

void setup();

void playTone(const unsigned int frequency, const std::chrono::milliseconds duration);

} // namespace board
