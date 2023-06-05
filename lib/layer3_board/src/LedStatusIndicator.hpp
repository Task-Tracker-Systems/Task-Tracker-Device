#pragma once

#include "StatusIndicator.hpp"
#include "board_types.hpp"

class LedStatusIndicator : public StatusIndicator
{
  public:
    LedStatusIndicator(const board::PinType pin);
    virtual void off() override;
    virtual void on() override;

  private:
    const board::PinType pin;
};
