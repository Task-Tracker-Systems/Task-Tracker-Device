#include "Task.hpp"
#include <type_traits>

const Task::String &Task::getLabel() const
{
    return label;
}

Task::Task(const String &newLabel, const Duration elapsedTime)
    : recordedDuration(elapsedTime), label(newLabel), isRunning(false)
{
}

void Task::start()
{
    if (!isRunning)
    {
        isRunning = true;
        timestampStart = std::chrono::round<DurationFraction>(Clock::now());
    }
}

void Task::stop()
{
    if (isRunning)
    {
        isRunning = false;
        recordedDuration += std::chrono::duration_cast<Duration>(Clock::now() - timestampStart);
    }
}

void Task::setLabel(const String &label)
{
    this->label = label;
}

Task::Duration Task::getRecordedDuration() const
{
    return recordedDuration;
}
