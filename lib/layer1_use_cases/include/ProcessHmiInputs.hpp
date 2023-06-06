#pragma once

#include "IController.hpp"
#include "IPresenter.hpp"

class ProcessUserInputs
{
  public:
    void loop();

  private:
    IController &userInputProvider;
    IPresenter &stateVisualizer;
};
