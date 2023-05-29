#pragma once

#include "IInputShiftRegister.hpp"
#include <RoxMux.h>
#include <cstddef>

template <std::size_t numberOfBytes>
class InputShiftRegister : public IInputShiftRegister<numberOfBytes * 8>
{
  public:
    typedef Rox74HC165<numberOfBytes> Backend;
    InputShiftRegister(Backend &isrImplementation)
        : isrBackend(isrImplementation)
    {
    }

    virtual Data readRegister() const override
    {
        Data data{};
        isrBackend.update();
        std::uint16_t pinIndex = numberOfBits - 1;
        for (auto pin : data)
        {
            pin = isrBackend.readPin(pinIndex--);
        }
        return data;
    }

  private:
    mutable Backend &isrBackend;
};
