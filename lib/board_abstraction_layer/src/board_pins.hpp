/**
 * @file .
 * Provides named objects for the pins provided by the board.
 */
#pragma once
#include "framework_config.hpp"

namespace board
{
/**
 * Pins attached to the output shift register.
 */
namespace osr::pin
{

/**
 * Serial data input to shift register.
 */
constexpr PinType data = 13;
/**
 * Storage (latch) pin.
 */
constexpr PinType latch = 12;

/**
 * Serial clock for shift register.
 */
constexpr PinType clock = 11;
} // namespace osr::pin

/**
 * Pins attached to the input shift register.
 */
namespace isr::pin
{

/**
 * Serial data output from shift register.
 */
constexpr PinType data = 10;

/**
 * Parallel load (latch) pin.
 */
constexpr PinType latch = 9;

/**
 * Serial clock for shift register.
 */
constexpr PinType clock = 11;
} // namespace isr::pin

/**
 * Pins to control the buzzer.
 */
namespace buzzer::pin
{
/**
 * Powers pin of the buzzer.
 */
constexpr PinType on_off = 14;
} // namespace buzzer::pin

/**
 * Pins for I2C controller 1.
 */
namespace i2c_1::pin
{
/**
 * Serial data.
 */
constexpr PinType sda = 42;

/**
 * Serial clock.
 */
constexpr PinType scl = 41;
} // namespace i2c_1::pin

} // namespace board
