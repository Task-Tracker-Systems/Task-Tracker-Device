#pragma once
#include <chrono>
#include <string>

namespace task_tracker_systems
{

/**
 * task object
 */
struct TaskObject
{
    /**
     * duration in seconds
     */
    std::chrono::seconds::rep duration;
    /**
     * unique identifier
     */
    unsigned int id;
    /**
     * name or summary; ASCII only, no line breaks
     */
    std::string label;
};
} // namespace task_tracker_systems
