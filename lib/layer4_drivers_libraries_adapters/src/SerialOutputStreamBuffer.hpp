#pragma once

#include <Arduino.h>
#include <cstddef>
#include <iterator>
#include <ostream>

template <std::size_t BufferSize = 0xFF>
class SerialOutputStreamBuffer : public std::streambuf
{
  private:
    char_type buffer[BufferSize];

  public:
    SerialOutputStreamBuffer()
        : std::streambuf()
    {
        resetOutputPointers();
    }

  protected:
    int_type overflow(const int_type input) override
    {
        flush();
        sputc(input);
        return input;
    }
    int sync() override
    {
        flush();
        return 0;
    }
    void flush()
    {
        const auto begin = pbase();
        const auto end = pptr();
        for (auto p = begin; p < end; p++)
        {
            Serial.print(*p);
        }
        resetOutputPointers();
    }
    void resetOutputPointers()
    {
        setp(std::begin(buffer), std::end(buffer));
    }
};
