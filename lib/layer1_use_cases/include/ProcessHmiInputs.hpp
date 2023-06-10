#pragma once

#include "IController.hpp"
#include "IPresenter.hpp"

class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(IController &userInputProvider, IPresenter &stateVisualizer);
    void loop();

  private:
    static TaskIndex event2TaskEvent(const IController::EventType event);
    IController &userInputProvider;
    IPresenter &stateVisualizer;
};
