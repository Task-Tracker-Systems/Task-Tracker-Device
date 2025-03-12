#include <Arduino.h>
#include <board_pins.hpp>
#include <type_traits>

/**
 * Makes sure that the defined pin type matches to the framework.
 */
static_assert(std::is_same_v<board::PinType, std::remove_cv_t<std::remove_reference_t<decltype(MOSI)>>>);
