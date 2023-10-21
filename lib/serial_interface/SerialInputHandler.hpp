#pragma once

#include <queue>
#include <string>

class Stream;

class SerialInputHandler
{
  public:
    SerialInputHandler();
    void handleNewSerialData(Stream &stream);

  private:
    std::queue<std::string> messageQueue;
};
