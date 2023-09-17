#include "Presenter.hpp"
#include "board_config.hpp"
#include "pitches.hpp"
#include <Arduino.h>
#include <cassert>

void Presenter::setTaskStatusIndicator(const TaskIndex index, const TaskIndicatorState state)
{
    assert(index <= hmi::numberOfStatusIndicators);
    switch (state)
    {
    case TaskIndicatorState::ACTIVE:
        statusIndicators[index]->on();
        constexpr auto notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
        tone(board::buzzer::pin::on_off, notes[index - 1], 250);
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
    pinMode(board::buzzer::pin::on_off, OUTPUT);
}
