#pragma once
#include <cstdint>

namespace board
{
/**
 * Type expected by HAL for accessing pins.
 *
 * This depends on the used framework.
 */
typedef std::uint8_t PinType;
} // namespace board
