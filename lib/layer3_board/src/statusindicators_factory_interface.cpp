#include "statusindicators_factory_interface.hpp"
#include "LedStatusIndicator.hpp"
#include <board_config.hpp>

namespace board
{
IStatusIndicator *const (&getStatusIndicators())[hmi::numberOfStatusIndicators]
{
    static LedStatusIndicator led_task1(board::led::pin::task1);
    static LedStatusIndicator led_task2(board::led::pin::task2);
    static LedStatusIndicator led_task3(board::led::pin::task3);
    static LedStatusIndicator led_task4(board::led::pin::task4);

    static IStatusIndicator *const statusIndicators[]{
        &led_task1,
        &led_task2,
        &led_task3,
        &led_task4,
    };
    return statusIndicators;
}
} // namespace board
