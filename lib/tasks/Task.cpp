#include "Task.hpp"
#include <Arduino.h>
#include <type_traits>

const Task::String &Task::getLabel() const
{
    return label;
}

Task::Task(const String &newLabel, const Duration elapsedTime)
    : recordedDuration(elapsedTime), label(newLabel), isRunning(false), timestampStart(0)
{
}

void Task::start()
{
    if (!isRunning)
    {
        isRunning = true;
        timestampStart = millis() / 1000;
    }
}

void Task::stop()
{
    if (isRunning)
    {
        isRunning = false;
        recordedDuration += (millis() / 1000) - timestampStart;
    }
}

void Task::setLabel(const String &label)
{
    this->label = label;
}

bool Task::isIsRunning() const
{
    return isRunning;
}

Task::Duration Task::getRecordedDuration() const
{
    return recordedDuration;
}
