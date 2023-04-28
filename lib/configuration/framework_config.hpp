/**
 * @file .
 * Provides defintions which are specific for the used framework.
 * 
 * Those definitions may be changed when switching to a different framework.
 */
#pragma once
#include <Arduino.h>
#include <cstdint>

/** 
 * Type expected by HAL for acessing pins.
 * 
 * This depends on the used framework.
 */
typedef decltype(MOSI) PinType;
