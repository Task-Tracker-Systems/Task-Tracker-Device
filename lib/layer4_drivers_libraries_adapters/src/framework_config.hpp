/**
 * @file .
 * @brief Framework adapters
 */
#pragma once
#include "board_types.hpp"
#include <Arduino.h>
#include <type_traits>

static_assert(std::is_same_v<board::PinType, std::remove_cv<std::remove_reference<decltype(MOSI)>::type>::type>);
