#pragma once

#include "IInputShiftRegister.hpp"
#include <RoxMux.h>
#include <cstddef>

template <std::size_t numberOfBytes>
class InputShiftRegister : public IInputShiftRegister<numberOfBytes * 8>
{
  public:
    //! Alias to the interface this class implements
    using Base = IInputShiftRegister<numberOfBytes * 8>;
    typedef Rox74HC165<numberOfBytes> Backend;
    InputShiftRegister(Backend &isrImplementation)
        : isrBackend(isrImplementation)
    {
    }

    virtual typename Base::Data readRegister() const override
    {
        typename Base::Data data{};
        isrBackend.update();
        std::uint16_t pinIndex = Base::numberOfBits - 1;
        for (auto pin : data)
        {
            pin = isrBackend.readPin(pinIndex--);
        }
        return data;
    }

  private:
    Backend &isrBackend;
};
