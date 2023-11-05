#include "serial_port.hpp"
#include "SerialOutputStreamBuffer.hpp"
#include <Arduino.h>
#include <iterator>

static SerialOutputStreamBuffer::char_type serial_output_buffer[255];
static SerialOutputStreamBuffer serialOutputStreamBuffer(std::begin(serial_output_buffer), std::end(serial_output_buffer));
static std::ostream serialOutputStream(&serialOutputStreamBuffer);

namespace serial_port
{
std::ostream &cout = serialOutputStream;

void initialize()
{
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
}
} // namespace serial_port
