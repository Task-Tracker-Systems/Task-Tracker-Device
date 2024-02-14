#pragma once
#include "KeyIds.hpp"

class Presenter;
class IKeypad;

/**
 * Responsible for handling the inputs from human interface devices.
 *
 * Inputs from human interface devices will be processed using the application logic.
 * The \ref Presenter is used to feedback information back to the human user.
 *
 * \dotfile presenter_collaboration.dot "information flow using the Presenter"
 */
class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(Presenter &stateVisualizer, IKeypad &keypad);

  private:
    Presenter &stateVisualizer;
    void handleHmiSelection(const KeyId selection);
};
