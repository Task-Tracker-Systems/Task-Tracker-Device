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
     * @return KeyId 
     */
    std::optional<KeyId> checkHmiInput() const;

  private:
    const IKeypad &keypad;
};
