#include "Controller.hpp"
#include "StatusIndicator.hpp"
#include "Task.hpp"
#include <cassert>

Controller::Controller(const StatusIndicators &statusIndicatorsToBeControlled, const Tasks &tasksToBeControlled)
    : statusIndicators(statusIndicatorsToBeControlled), tasks(tasksToBeControlled)
{
}

void Controller::processEvent(const Event event)
{
    switch (event)
    {
    case NONE:
        break;
    case TASK_1:
        processTask(0);
        break;
    case TASK_2:
        processTask(1);
        break;
    case TASK_3:
        processTask(2);
        break;
    case TASK_4:
        processTask(3);
        break;
    default:
        assert(false); // unknown event
        break;
    }
}

void Controller::processTask(const std::size_t taskIndex)
{
    if (!tasks[taskIndex]->isIsRunning())
    {
        tasks[taskIndex]->start();
        statusIndicators[getStatusIndicatorIndexForTask(taskIndex)]->on();
    }
    else
    {
        tasks[taskIndex]->stop();
        statusIndicators[getStatusIndicatorIndexForTask(taskIndex)]->off();
    }
}

// TODO find a reasonable way to configure the mapping of tasks to status indicators
std::size_t Controller::getStatusIndicatorIndexForTask(const std::size_t taskIndex) const
{
    return taskIndex;
}
