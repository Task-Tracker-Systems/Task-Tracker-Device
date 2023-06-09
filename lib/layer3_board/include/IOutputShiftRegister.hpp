#pragma once

#include <cstddef>

class IOutputShiftRegister
{
  public:
    typedef std::size_t BitNumber;
    virtual void writePin(const BitNumber bitNumber, const bool set) = 0;
    virtual ~IOutputShiftRegister()
    {
    }
};
