#pragma once
#include <array>
#include <cstddef>
#include <vector>

class StatusIndicator;
class Task;

class Controller
{
  public:
    constexpr std::size_t numberOfStatusIndicators = 4;
    typedef std::array<StatusIndicator *const, numberOfStatusIndicators> StatusIndicators;
    typedef std::vector<Task &> Tasks;
    enum Event
    {
        TASK_1,
        TASK_2,
        TASK_3,
        TASK_4,
        UP,
        DOWN,
        ENTER,
        BACK,
    };
    Controller(const StatusIndicators &statusIndicatorsToBeControlled, const Tasks &tasksToBeControlled);
    void processEvent(const Event event);

  private:
    const StatusIndicators statusIndicators;
    Tasks tasks;
    void processTask(const std::size_t taskIndex);
    std::size_t getStatusIndicatorIndexForTask(const std::size_t taskIndex) const;
};
