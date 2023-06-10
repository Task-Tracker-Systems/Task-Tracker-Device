#include "ProcessHmiInputs.hpp"
#include <Arduino.h> //!< @TODO this is for debugging only
#include <type_traits>

ProcessHmiInputs::ProcessHmiInputs(IController &controller, IPresenter &presenter)
    : userInputProvider(controller), stateVisualizer(presenter)
{
}

/**
 * @deprecated this if for test purposes only
 * @todo use proper logging interface
 */
static void printEvent(const IController::EventType event)
{
    using EventNumber = std::underlying_type_t<decltype(event)>;
    const EventNumber eventNumber = static_cast<EventNumber>(event);
    Serial.printf("Process event '");
    if constexpr (std::is_signed_v<EventNumber>)
    {
        Serial.printf("%i'.\n", eventNumber);
    }
    else
    {
        Serial.printf("%u'.\n", eventNumber);
    }
}

void ProcessHmiInputs::loop()
{
    const auto event = userInputProvider.checkHmiInput();
    if (event)
    {
        printEvent(event.value());
        stateVisualizer.setTaskStatusIndicator(event2TaskEvent(event.value()), TaskIndicatorState::ACTIVE);
    }
    else
    {
        for (const auto i : {0, 1, 2, 3})
        {
            stateVisualizer.setTaskStatusIndicator(i, TaskIndicatorState::INACTIVE);
        }
    }
}

TaskIndex ProcessHmiInputs::event2TaskEvent(const IController::EventType event)
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
