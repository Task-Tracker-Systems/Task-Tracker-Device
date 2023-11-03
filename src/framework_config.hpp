/**
 * @file .
 * Provides defintions which are specific for the used framework.
 * 
 * Those definitions may be changed when switching to a different framework.
 */
#pragma once
#include <Arduino.h>
#include <cstdint>
#include <type_traits>

/** 
 * Type expected by HAL for acessing pins.
 * 
 * This depends on the used framework.
 */
typedef typename std::remove_cv<decltype(MOSI)>::type PinType;
