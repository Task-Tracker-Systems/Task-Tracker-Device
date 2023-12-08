/**
 * \file .
 */
#pragma once
#include <chrono>
#include <map>
#include <string>

/**
 * Task ID.
 */
typedef unsigned int TaskId;

/**
 * Manages a task.
 * 
 * A task consists of a label and a duration.
 * The duration is incremented while the task is running.
 * The duration is stored in seconds.
 */
class Task
{
  public:
    /**
     * Type used to record durations.
     *
     * \internal
     * Must be big enough to hold a duration of 10 years in milliseconds.
     * \endinternal
     */
    typedef std::chrono::seconds Duration;

    /**
     * String type used for labels of the task.
     *
     * \internal
     * Must be able to hold special non-ANSI characters.
     * \endinternal
     */
    typedef std::wstring String;
    Task(const String &newLabel, const Duration elapsedTime = Duration::zero());

    /**
     * Starts/continues capturing duration
     *
     * Sets the state of the task to "running".
     */
    void start();

    /**
     * Stops/pauses capturing duration.
     *
     * Sets the state of the task to "stopped".
     * Has no effect if already stopped.
     */
    void stop();
    const String &getLabel() const;
    void setLabel(const String &label);

    /**
     * Gets the recorded duration.
     * 
     * Does also consider an already begun interval if the task is running.
     * 
     * \returns the accumulated duration
     */
    Duration getRecordedDuration();
    bool isRunning() const;

  private:
    String label;
    enum class State
    {
        IDLE,
        RUNNING,
    };
    State state;

    /**
     * Internal representation in order to reduce loss of precision.
     */
    typedef std::chrono::milliseconds DurationFraction;
    DurationFraction recordedDuration;
    typedef std::chrono::system_clock Clock;
    std::chrono::time_point<Clock, DurationFraction> timestampStart;
};

namespace device
{
/**
 * *The* collection of tasks to be used by the device application.
 */
extern std::map<TaskId, Task> tasks;
} // namespace device
