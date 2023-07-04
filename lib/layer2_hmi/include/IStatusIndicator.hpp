#pragma once

class IStatusIndicator
{
  public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual ~IStatusIndicator(){};
};
