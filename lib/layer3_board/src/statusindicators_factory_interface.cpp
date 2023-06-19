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
    static ShiftRegisterStatusIndicator srSi1(outputShiftRegister, board::osr::bit::task1);
    static ShiftRegisterStatusIndicator srSi2(outputShiftRegister, board::osr::bit::task2);
    static ShiftRegisterStatusIndicator srSi3(outputShiftRegister, board::osr::bit::task3);
    static ShiftRegisterStatusIndicator srSi4(outputShiftRegister, board::osr::bit::task4);
    static IStatusIndicator *const statusIndicators[]{
        &srSi1,
        &srSi2,
        &srSi3,
        &srSi4,
    };
    return statusIndicators;
}
} // namespace board
