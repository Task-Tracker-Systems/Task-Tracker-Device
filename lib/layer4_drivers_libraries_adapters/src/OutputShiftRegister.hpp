#pragma once

#include "IOutputShiftRegister.hpp"
#include <ShiftRegister74HC595.h>
#include <cstddef>

template <std::size_t numberOfBytes>
class OutputShiftRegister : public IOutputShiftRegister
{
  public:
    OutputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin);
    virtual void writePin(const BitNumber bitNumber, const bool set) override;

  private:
    ShiftRegister74HC595<numberOfBytes> outputShiftRegister;
};

#include "OutputShiftRegister_implementation.hpp"
