#pragma once

#include "KeyIds.hpp"
#include <optional>

class IController
{
  public:
    typedef KeyId EventType;
    /**
     * Checks if the state of the HMI has changed and generates an event.
     * 
     * @returns an event identifier in case one has been detected
     */
    virtual std::optional<EventType> checkHmiInput() const = 0;
};
