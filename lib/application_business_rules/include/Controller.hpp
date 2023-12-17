#pragma once

#include "IKeypad.hpp"
#include "KeyIds.hpp"
#include <optional>

class Controller
{
  public:
    typedef KeyId EventType;
    Controller(const IKeypad &);

    /**
     * Checks if the state of the HMI has changed and generates an event.
     * 
     * @returns an event identifier in case one has been detected
     */
    std::optional<EventType> checkHmiInput() const;
    virtual ~Controller()
    {
    }

  private:
    const IKeypad &keypad;
};
