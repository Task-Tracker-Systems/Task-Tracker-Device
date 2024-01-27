/**
 * \file .
 * Dependency injection for the keypad.
 *
 * \see \ref dependency_injection
 */

#pragma once

#include "IStatusIndicator.hpp"
#include <vector>

namespace board
{
std::vector<IStatusIndicator *> getStatusIndicators();
}
