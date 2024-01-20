#pragma once
#include "KeyIds.hpp"

class Presenter;
class IKeypad;

class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(Presenter &stateVisualizer, IKeypad &keypad);

  private:
    Presenter &stateVisualizer;
    void handleHmiSelection(const KeyId selection);
};
