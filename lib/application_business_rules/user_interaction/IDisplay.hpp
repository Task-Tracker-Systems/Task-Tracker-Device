#pragma once

/**
 * Interface to a display capable of displaying various information to a human.
 *
 * This is used to implement the \ref plugin_architecture.
 */
class IDisplay
{
  public:
    virtual void refresh() = 0;
};
