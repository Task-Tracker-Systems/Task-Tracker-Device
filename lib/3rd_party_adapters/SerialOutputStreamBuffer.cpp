#include "SerialOutputStreamBuffer.hpp"
#include <Arduino.h>
#include <iterator>

SerialOutputStreamBuffer::SerialOutputStreamBuffer(char_type *const buffer_begin, char_type *const buffer_end)
    : std::streambuf(), buffer_begin(buffer_begin), buffer_end(buffer_end)
{
    resetOutputPointers();
}

SerialOutputStreamBuffer::int_type SerialOutputStreamBuffer::overflow(const int_type input)
{
    flush();
    sputc(input);
    return input;
}

int SerialOutputStreamBuffer::sync()
{
    flush();
    return 0;
}

void SerialOutputStreamBuffer::flush()
{
    const auto begin = pbase();
    const auto end = pptr();
    Serial.write(begin, std::distance(begin, end));
    resetOutputPointers();
}

void SerialOutputStreamBuffer::resetOutputPointers()
{
    setp(buffer_begin, buffer_end);
}
