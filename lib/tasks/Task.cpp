#include "Task.hpp"
#include <type_traits>

const Task::String &Task::getLabel() const
{
    return label;
}

Task::Task(const ID id, const String &newLabel, const Duration elapsedTime)
    : id(id), recordedDuration(elapsedTime), label(newLabel), state(State::IDLE)
{
}

void Task::start()
{
    state = State::RUNNING;
    timestampStart = std::chrono::round<DurationFraction>(Clock::now());
}

void Task::stop()
{
    // this check is necessary, as else the timestamp using for comparison will be invalid
    if (state == State::RUNNING)
    {
        state = State::IDLE;
        recordedDuration += std::chrono::duration_cast<DurationFraction>(Clock::now() - timestampStart);
    }
}

bool Task::isRunning() const
{
    return state == State::RUNNING;
}

void Task::setLabel(const String &label)
{
    this->label = label;
}

Task::Duration Task::getRecordedDuration()
{
    if (isRunning())
    {
        stop();
        start();
    }
    return std::chrono::round<Duration>(recordedDuration);
}

Task::ID Task::getId() const
{
    return id;
}

std::vector<Task> device::tasks;
