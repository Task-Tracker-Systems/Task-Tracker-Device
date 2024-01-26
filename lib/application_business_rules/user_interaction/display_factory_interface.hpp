/**
 * \file .
 * Dependency injection for the display.
 *
 * \see \ref dependency_injection
 */

#pragma once

#include "IDisplay.hpp"

namespace board
{
IDisplay &getDisplay();
}
