#include "logging.hpp"
#include "SerialOutputStreamBuffer.hpp"

static SerialOutputStreamBuffer serialOutputStreamBuffer;
std::ostream loggingStream(&serialOutputStreamBuffer);
std::ostream &logging = loggingStream;
