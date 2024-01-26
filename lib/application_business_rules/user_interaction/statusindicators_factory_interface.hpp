/**
 * \file .
 * Dependency injection for the keypad.
 *
 * \see \ref dependency_injection
 */

#pragma once

#include "IStatusIndicator.hpp"
#include "hmi_configuration.hpp"

namespace board
{
IStatusIndicator *const (&getStatusIndicators())[hmi::numberOfStatusIndicators];
}
