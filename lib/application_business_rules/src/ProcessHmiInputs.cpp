#include "ProcessHmiInputs.hpp"
#include "serial_port.hpp"
#include <cassert>
#include <type_traits>

ProcessHmiInputs::ProcessHmiInputs(Controller &controller, Presenter &presenter)
    : userInputProvider(controller), stateVisualizer(presenter)
{
}

void ProcessHmiInputs::loop()
{
    const auto event = userInputProvider.checkHmiInput();
    if (event)
    {
        serial_port::cout << "Process event '" << event.value() << "'." << std::endl;
        if (isEventTask(event.value()))
        {
            stateVisualizer.setTaskStatusIndicator(event2TaskEvent(event.value()), TaskIndicatorState::ACTIVE);
        }
    }
    else
    {
        for (const auto i : {0, 1, 2, 3})
        {
            stateVisualizer.setTaskStatusIndicator(i, TaskIndicatorState::INACTIVE);
        }
    }
}

TaskIndex ProcessHmiInputs::event2TaskEvent(const Controller::EventType event)
{
    switch (event)
    {
    case KeyId::TASK1:
        return 0;
        break;
    case KeyId::TASK2:
        return 1;
        break;
    case KeyId::TASK3:
        return 2;
        break;
    case KeyId::TASK4:
        return 3;
        break;
    default:
        assert(false);
        return 0;
        break;
    }
}

bool ProcessHmiInputs::isEventTask(const Controller::EventType event)
{
    return event == KeyId::TASK1 || event == KeyId::TASK2 || event == KeyId::TASK3 || event == KeyId::TASK4;
}
