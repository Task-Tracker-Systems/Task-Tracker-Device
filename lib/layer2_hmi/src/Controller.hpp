#pragma once

#include "IKeypad.hpp"
#include "KeyIds.hpp"

class Controller
{
    KeyId checkHmiInput() const;

  private:
    IKeypad &keypad;
};
