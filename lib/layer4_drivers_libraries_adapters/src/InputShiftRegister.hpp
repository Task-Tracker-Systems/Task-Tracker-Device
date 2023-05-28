#pragma once

#include "IInputShiftRegister.hpp"
#include <RoxMux.h>
#include <cstddef>

template <std::size_t numberOfBytes>
class InputShiftRegister : public IInputShiftRegister<numberOfBytes * 8>
{
  public:
    InputShiftRegister(const std::uint8_t pinNr_data, const std::uint8_t pinNr_load, const std::uint8_t pinNr_clock)
        : isrBackend()
    {
        isrBackend.begin(pinNr_data, pinNr_load, pinNr_clock);
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
    mutable Rox74HC165<numberOfBytes> isrBackend;
};
