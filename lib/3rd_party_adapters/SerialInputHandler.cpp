#include "SerialInputHandler.hpp"
#include <Arduino.h>

SerialInputHandler::SerialInputHandler()
{
    // TODO Auto-generated constructor stub
}

void SerialInputHandler::handleNewSerialData(Stream &stream)
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
            static std::string begunLine;
            begunLine += inChar;
            if (inChar == '\n' || inChar == '\r')
            {
                messageQueue.push(begunLine);
                begunLine.clear();
            }
        }
    }
}

std::string SerialInputHandler::getNextLine()
{
    const std::string line = messageQueue.front();
    messageQueue.pop();
    return line;
}

SerialInputHandler &SerialInputHandler::getInstance()
{
    static SerialInputHandler instance;
    return instance;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
    auto handler = SerialInputHandler::getInstance();
    handler.handleNewSerialData(Serial);
}
