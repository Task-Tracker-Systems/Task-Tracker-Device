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
 * 
 * Label on board: D14
 */
constexpr PinType data = 14;

/**
 * Storage (latch) pin.
 * 
 * Label on board: D13
 */
constexpr PinType latch = 13;

/**
 * Serial clock for shift register.
 * 
 * Label on board: D5
 */
constexpr PinType clock = 5;
} // namespace osr::pin

/**
 * Pins attached to the input shift register.
 */
namespace isr::pin
{

/**
 * Serial data output from shift register.
 * 
 * Label on board: D34
 */
constexpr PinType data = 34;

/**
 * Parallel load (latch) pin.
 * 
 * Label on board: D12
 */
constexpr PinType latch = 12;

/**
 * Serial clock for shift register.
 * 
 * Label on board: D23
 */
constexpr PinType clock = 23;
} // namespace isr::pin

/**
 * Pins to control the buzzer.
 */
namespace buzzer::pin
{
/**
 * Powers pin of the buzzer.
 * 
 * Label on board: D4
 */
constexpr PinType on_off = 4;
} // namespace buzzer::pin

} // namespace board
