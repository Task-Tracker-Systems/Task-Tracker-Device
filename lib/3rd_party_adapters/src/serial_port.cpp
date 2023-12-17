#include "serial_port.hpp"
#include "SerialOutputStreamBuffer.hpp"
#include <Arduino.h>
#include <iterator>
#include <type_traits>

static SerialOutputStreamBuffer::char_type serial_output_buffer[255];
static SerialOutputStreamBuffer serialOutputStreamBuffer(std::begin(serial_output_buffer), std::end(serial_output_buffer));
static std::basic_ostream<SerialOutputStreamBuffer::char_type> serialOutputStream(&serialOutputStreamBuffer);

namespace serial_port
{
static_assert(std::is_same_v<CharType, std::remove_cv_t<std::remove_reference_t<decltype(*Serial.readString().c_str())>>>);

static StringHandler incomingStringHandler;

std::basic_ostream<CharType> &cout = serialOutputStream;

void initialize()
{
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
}

String readLine()
{
    return String(Serial.readStringUntil('\n').c_str());
}

std::optional<String> getLine()
{
    if (Serial.available() > 0)
    {
        return readLine();
    }
    else
    {
        return {};
    }
}

void setCallbackForLineReception(const StringHandler &callback)
{
    incomingStringHandler = callback;
}

} // namespace serial_port

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
    while (Serial.available() > 0)
    {
        const auto inData = Serial.read();
        if (inData == -1)
        {
            break;
        }
        else
        {
            static serial_port::String inputBuffer{};
            const serial_port::String::value_type inChar = inData;
            // if the incoming character is a newline, call handler
            if (inChar == '\n' || inChar == '\r')
            {
                if (!inputBuffer.empty())
                {
                    serial_port::incomingStringHandler(inputBuffer);
                }
                inputBuffer.clear();
            }
            else
            {
                inputBuffer += inChar;
            }
        }
    }
}
