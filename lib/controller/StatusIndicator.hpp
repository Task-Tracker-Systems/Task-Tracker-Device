#pragma once

class StatusIndicator
{
  public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual ~StatusIndicator(){};
};
