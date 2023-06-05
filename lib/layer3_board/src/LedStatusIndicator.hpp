#pragma once

#include "IStatusIndicator.hpp"
#include "board_types.hpp"

class LedStatusIndicator : public IStatusIndicator
{
  public:
    LedStatusIndicator(const board::PinType pin);
    virtual void off() override;
    virtual void on() override;

  private:
    const board::PinType pin;
};
