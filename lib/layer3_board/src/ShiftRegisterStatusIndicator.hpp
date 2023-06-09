#pragma once
#include "IOutputShiftRegister.hpp"
#include "IStatusIndicator.hpp"
#include <cstddef>

class ShiftRegisterStatusIndicator : public IStatusIndicator
{
  public:
    ShiftRegisterStatusIndicator(IOutputShiftRegister &outputShiftRegister, const IOutputShiftRegister::BitNumber bit);
    virtual void off() override;
    virtual void on() override;

  private:
    IOutputShiftRegister &shiftRegister;
    const IOutputShiftRegister::BitNumber bit;
};
