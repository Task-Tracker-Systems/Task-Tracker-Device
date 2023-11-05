#include "serial_port.hpp"
#include "SerialOutputStreamBuffer.hpp"
#include <iterator>

static SerialOutputStreamBuffer::char_type serial_output_buffer[255];
static SerialOutputStreamBuffer serialOutputStreamBuffer(std::begin(serial_output_buffer), std::end(serial_output_buffer));
static std::ostream serialOutputStream(&serialOutputStreamBuffer);
std::ostream &serial_port::cout = serialOutputStream;
