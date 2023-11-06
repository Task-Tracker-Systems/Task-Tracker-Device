#include "serial_port.hpp"
#include "SerialInputStreamBuffer.hpp"
#include "SerialOutputStreamBuffer.hpp"
#include <Arduino.h>
#include <cstddef>
#include <iterator>

static SerialOutputStreamBuffer::char_type serial_output_buffer[255];
static SerialOutputStreamBuffer serialOutputStreamBuffer(std::begin(serial_output_buffer), std::end(serial_output_buffer));
static std::ostream serialOutputStream(&serialOutputStreamBuffer);

/**
 * According to the Arduino reference, the input buffer has a size of 64 bytes.
 */
static constexpr std::size_t sizeOfInputBuffer = 64;
static SerialInputStreamBuffer::char_type serial_input_buffer[sizeOfInputBuffer];
static SerialInputStreamBuffer serialInputStreamBuffer(std::begin(serial_input_buffer), std::end(serial_input_buffer));
static std::istream serialInputStream(&serialInputStreamBuffer);

namespace serial_port
{
std::ostream &cout = serialOutputStream;
std::istream &cin = serialInputStream;

void initialize()
{
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
}
} // namespace serial_port
