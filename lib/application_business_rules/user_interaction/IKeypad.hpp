#pragma once

#include "KeyIds.hpp"
#include <functional>

typedef std::function<void(KeyId)> HmiHandler;

class IKeypad
{
  public:
    virtual void setCallback(const HmiHandler callbackFunction) = 0;
    virtual ~IKeypad(){};
};
