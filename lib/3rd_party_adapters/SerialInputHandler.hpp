#pragma once

#include <queue>
#include <string>

class Stream;

class SerialInputHandler
{
  public:
    std::string getNextLine();
    static SerialInputHandler &getInstance();

  private:
    std::queue<std::string> messageQueue;
    SerialInputHandler();
    void handleNewSerialData(Stream &stream);

    friend void serialEvent();

  public:
    auto getNumberOfAvailableLines() const
    {
        return messageQueue.size();
    }
};
