#pragma once

#include <user_interaction/IKeypad.hpp>

class Keypad : public IKeypad
{
  public:
    Keypad(const HmiHandler callbackFunction);
};
