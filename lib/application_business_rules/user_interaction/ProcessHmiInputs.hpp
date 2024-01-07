#pragma once
#include "KeyIds.hpp"

class Presenter;

class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(Presenter &stateVisualizer);

  private:
    Presenter &stateVisualizer;
    void handleHmiSelection(const KeyId selection);
};
