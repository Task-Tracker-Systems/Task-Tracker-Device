#include "SerialInputHandler.hpp"
#include <Arduino.h>

SerialInputHandler::SerialInputHandler()
{
    // TODO Auto-generated constructor stub
}

void SerialInputHandler::handleNewSerialData(Stream &stream)
{
}

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
