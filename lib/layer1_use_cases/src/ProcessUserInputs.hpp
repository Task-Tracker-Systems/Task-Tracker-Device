#pragma once

#include "IController.hpp"
#include "IPresenter.hpp"

class ProcessUserInputs
{
  private:
    IController &userInputProvider;
    IPresenter &stateVisualizer;
};
