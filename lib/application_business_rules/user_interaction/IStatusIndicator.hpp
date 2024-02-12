#pragma once

/**
 * Interface to an indicator capable to visualize a state.
 *
 * This is used to implement the \ref plugin_architecture.
 */
class IStatusIndicator
{
  public:
    virtual void on() = 0;
    virtual void off() = 0;
    virtual ~IStatusIndicator(){};
};
