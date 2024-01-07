#pragma once

#include "IKeypad.hpp"

namespace board
{
const IKeypad &getKeypad(const HmiHandler callbackFunction);
}
