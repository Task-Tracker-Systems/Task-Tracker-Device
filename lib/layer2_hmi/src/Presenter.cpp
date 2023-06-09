#include "Presenter.hpp"
#include <cassert>

void Presenter::setTaskStatusIndicator(const TaskIndex index, const TaskIndicatorState state)
{
    assert(index <= hmi::numberOfStatusIndicators);
    switch (state)
    {
    case TaskIndicatorState::ACTIVE:
        statusIndicators[index]->on();
        break;
    case TaskIndicatorState::INACTIVE:
        statusIndicators[index]->off();
        break;
    default:
        assert(false);
        break;
    }
}

Presenter::Presenter(Menu &menuToUse, IStatusIndicator *const (&statusIndicatorsToUse)[hmi::numberOfStatusIndicators])
    : menu(menuToUse), statusIndicators(statusIndicatorsToUse)
{
}
