#pragma once

#include <array>
#include <cstddef>

template <std::size_t NumberOfBits>
class IInputShiftRegister
{
  public:
    /**
     * The state of the data bits D0...D7.
     */
    typedef std::array<bool, NumberOfBits> Data;
    static constexpr std::size_t numberOfBits = NumberOfBits;
    virtual Data readRegister() const = 0;
    virtual ~IInputShiftRegister()
    {
    }
};
