#pragma once

#include "IController.hpp"
#include "IKeypad.hpp"
#include "KeyIds.hpp"
#include <optional>

class Controller : public IController
{
  public:
    Controller(const IKeypad &);

    /**
     * Checks if the state of the HMI has changed and generates an event.
     * 
     * @returns an event identifier in case one has been detected
     */
    virtual std::optional<IController::EventType> checkHmiInput() const override;
    virtual ~Controller()
    {
    }

  private:
    const IKeypad &keypad;
};
