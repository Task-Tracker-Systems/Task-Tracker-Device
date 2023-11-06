#include "SerialInputStreamBuffer.hpp"
#include <Arduino.h>
#include <algorithm>
#include <iterator>

SerialInputStreamBuffer::SerialInputStreamBuffer(char_type *const buffer_begin, char_type *const buffer_end)
    : buffer_begin(buffer_begin), buffer_end(buffer_end)
{
    setg(buffer_begin, buffer_end, buffer_end);
}

SerialInputStreamBuffer::int_type SerialInputStreamBuffer::underflow()
{
    const auto availableBytes = Serial.available();
    if (availableBytes <= 0)
    {
        return traits_type::eof();
    }
    static const auto bufferLength = std::distance(buffer_begin, buffer_end);
    static const auto bufferSize = bufferLength / sizeof(*buffer_begin);
    const auto readBytes = Serial.readBytes(buffer_begin, std::min(static_cast<int>(bufferSize), availableBytes));
    if (readBytes <= 0)
    {
        return traits_type::eof();
    }
    setg(buffer_begin, buffer_begin, std::next(buffer_begin, readBytes / sizeof(*buffer_begin)));
    return traits_type::to_int_type(*gptr());
}
