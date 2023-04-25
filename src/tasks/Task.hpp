/**
 * \file .
 */
#pragma once
#include <cstdint>
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
    typedef std::uint32_t Duration;

    /**
     * String type used for labels of the task.
     *
     * \internal
     * Must be able to hold special non-ANSI characters.
     * \endinternal
     */
    typedef std::wstring String;
    Task(const String &newLabel, const Duration elapsedTime = 0U);
    void start();
    void stop();
    const String &getLabel() const;
    void setLabel(const String &label);
    Duration getRecordedDuration() const;

  private:
    Duration recordedDuration;
    String label;
    bool isRunning;
    unsigned long timestampStart;
};
