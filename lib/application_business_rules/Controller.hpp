/** \file */
#pragma once
#include "board_interface.hpp"
#include "tasks/Task.hpp"

class Controller
{
  public:
    Controller();

  private:
    void handleHmiSelection(const board::HmiSelection selection);
    Task::ID mapTaskSelectionToTaskId(const board::HmiSelection taskSelection);
    unsigned int mapSelectionToFrequency(const board::HmiSelection selection);
};
