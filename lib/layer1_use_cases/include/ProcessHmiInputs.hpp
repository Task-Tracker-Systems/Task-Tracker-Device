#pragma once

#include "IController.hpp"
#include "IPresenter.hpp"

class ProcessHmiInputs
{
  public:
    void loop();

  private:
    IController &userInputProvider;
    IPresenter &stateVisualizer;
};
