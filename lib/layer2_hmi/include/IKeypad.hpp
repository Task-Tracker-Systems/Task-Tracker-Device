#pragma once

#include "KeyIds.hpp"

class IKeypad
{
  public:
    virtual KeyId getCurrentlyPressedKey() const = 0;
    virtual ~IKeypad(){};
};
