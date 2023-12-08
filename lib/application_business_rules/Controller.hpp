/** \file */
#pragma once
#include "board_interface.hpp"

class Task;

class Controller
{
  public:
    Controller();

  private:
    void handleHmiSelection(const board::HmiSelection selection);
    Task &mapTaskSelectionToTask(const board::HmiSelection taskSelection);
    unsigned int mapSelectionToFrequency(const board::HmiSelection selection);
};
