#include "serial_port.hpp"
#include "SerialOutputStreamBuffer.hpp"
#include <Arduino.h>
#include <iterator>

static SerialOutputStreamBuffer::char_type serial_output_buffer[255];
static SerialOutputStreamBuffer serialOutputStreamBuffer(std::begin(serial_output_buffer), std::end(serial_output_buffer));
static std::ostream serialOutputStream(&serialOutputStreamBuffer);

namespace serial_port
{
static StringHandler incomingStringHandler;

std::ostream &cout = serialOutputStream;

void initialize()
{
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
}

std::string readLine()
{
    return std::string(Serial.readStringUntil('\n').c_str());
}

std::optional<std::string> getLine()
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

void subscribeToIncomingLine(StringHandler &callback)
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
        if (inData < 0)
        {
            break;
        }
        else
        {
            const auto inChar = static_cast<char>(inData);
            inputString += inChar;
            // if the incoming character is a newline, set a flag so the main loop can
            // do something about it:
            if (inChar == '\n' || inChar == '\r')
            {
                stringComplete = true;
            }
        }
    }
}
