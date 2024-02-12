#include "status_indicators.hpp"
#include <board_pins.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>

namespace board
{
std::vector<IStatusIndicator *> getStatusIndicators()
{
    static LedStatusIndicator led_task1(board::led::pin::task1);
    static LedStatusIndicator led_task2(board::led::pin::task2);
    static LedStatusIndicator led_task3(board::led::pin::task3);
    static LedStatusIndicator led_task4(board::led::pin::task4);

    return {
        &led_task1,
        &led_task2,
        &led_task3,
        &led_task4,
    };
}
} // namespace board
