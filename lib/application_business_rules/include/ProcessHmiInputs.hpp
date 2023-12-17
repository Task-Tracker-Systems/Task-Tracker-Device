#pragma once

#include "Controller.hpp"
#include "Presenter.hpp"

class ProcessHmiInputs
{
  public:
    ProcessHmiInputs(Controller &userInputProvider, Presenter &stateVisualizer);
    void loop();

  private:
    static TaskIndex event2TaskEvent(const Controller::EventType event);
    Controller &userInputProvider;
    Presenter &stateVisualizer;
    bool isEventTask(const Controller::EventType event);
};
