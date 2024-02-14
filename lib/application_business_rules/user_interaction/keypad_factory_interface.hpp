/**
 * \file .
 * Dependency injection for the keypad.
 *
 * \see \ref dependency_injection
 */

#pragma once

#include "IKeypad.hpp"

namespace board
{
IKeypad &getKeypad();
}
