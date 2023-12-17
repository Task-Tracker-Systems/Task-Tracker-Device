#pragma once

#include "Controller.hpp"
#include "IPresenter.hpp"

class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(Controller &userInputProvider, IPresenter &stateVisualizer);
    void loop();

  private:
    static TaskIndex event2TaskEvent(const Controller::EventType event);
    Controller &userInputProvider;
    IPresenter &stateVisualizer;
    bool isEventTask(const Controller::EventType event);
};
