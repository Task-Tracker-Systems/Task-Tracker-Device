#pragma once

#include "IStatusIndicator.hpp"
#include "hmi_configuration.hpp"

namespace board
{
IStatusIndicator *const (&getStatusIndicators())[hmi::numberOfStatusIndicators];
}