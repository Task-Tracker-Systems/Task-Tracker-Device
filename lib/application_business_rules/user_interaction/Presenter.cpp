#include "Presenter.hpp"
#include "board_interface.hpp"
#include <cassert>
#include <chrono>
#include <pitches.hpp>

static unsigned int mapSelectionToFrequency(const TaskIndex index)
{
    constexpr decltype(note::c3) notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
    return notes[index];
}

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
    using namespace std::chrono_literals;
    board::playTone(mapSelectionToFrequency(index), 250ms);
}

Presenter::Presenter(Menu &menuToUse, IStatusIndicator *const (&statusIndicatorsToUse)[hmi::numberOfStatusIndicators])
    : menu(menuToUse), statusIndicators(statusIndicatorsToUse)
{
    board::setup();
}

void Presenter::loop()
{
    menu.loop();
}
