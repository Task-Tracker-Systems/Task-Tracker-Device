#pragma once

#include "StatusIndicator.hpp"
#include "framework_config.hpp"

class LedStatusIndicator : public StatusIndicator
{
  public:
    LedStatusIndicator(const PinType pin);
    virtual void off() override;
    virtual void on() override;

  private:
    PinType pin;
};
