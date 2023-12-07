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

typedef std::function<void(HmiSelection)> HmiHandler;

void setHmiHandler(const HmiHandler callbackFunction);

void setup();

void playTone(const unsigned int frequency, const std::chrono::milliseconds duration);

} // namespace board
