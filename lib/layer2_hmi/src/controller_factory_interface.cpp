#pragma once

#include "controller_factory_interface.hpp"
#include "Controller.hpp"
#include "keypad_factory_interface.hpp"

namespace hmi
{
IController &getController()
{
    static Controller singleton(board::getKeypad());
    return singleton;
}
} // namespace hmi