/**
 * \file .
 */
#pragma once
#include <chrono>
#include <string>

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
    Task(const String &newLabel, const Duration elapsedTime = Duration(0U));

    /**
     * Starts/continues capturing duration
     *
     * Sets the state of the task to "running".
     * Has no effect if already running.
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
    Duration getRecordedDuration() const;

  private:
    Duration recordedDuration;
    String label;
    bool isRunning;
    typedef std::chrono::system_clock Clock;
    typedef std::chrono::milliseconds DurationFraction;
    std::chrono::time_point<Clock, DurationFraction> timestampStart;
};
