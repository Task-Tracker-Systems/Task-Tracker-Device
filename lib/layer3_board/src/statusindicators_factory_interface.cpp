#pragma once

#include "statusindicators_factory_interface.hpp"
#include "ShiftRegisterStatusIndicator.hpp"
#include "osr_factory_interface.hpp"
#include <board_config.hpp>

namespace board
{
IStatusIndicator *const (&getStatusIndicators())[hmi::numberOfStatusIndicators]
{
    IOutputShiftRegister &outputShiftRegister = ::osr::getOutputShiftRegister(board::osr::pin::data,
                                                                              board::osr::pin::clock,
                                                                              board::osr::pin::latch);
    static ShiftRegisterStatusIndicator srsi1(outputShiftRegister, board::osr::bit::task1);
    static ShiftRegisterStatusIndicator srsi2(outputShiftRegister, board::osr::bit::task2);
    static ShiftRegisterStatusIndicator srsi3(outputShiftRegister, board::osr::bit::task3);
    static ShiftRegisterStatusIndicator srsi4(outputShiftRegister, board::osr::bit::task4);
    static IStatusIndicator *const statusIndicators[]{
        &srsi1,
        &srsi2,
        &srsi3,
        &srsi4,
    };
    return statusIndicators;
}
} // namespace board
