#pragma once

#include "IKeypad.hpp"
#include "KeyIds.hpp"
#include <optional>

class Controller
{
  public:
    Controller(const IKeypad &);

    /**
     * Checks if the state of the HMI has changed and generates an event.
     * 
     * @returns an event identifier in case one has been detected
     */
    std::optional<KeyId> checkHmiInput() const;

  private:
    const IKeypad &keypad;
};
