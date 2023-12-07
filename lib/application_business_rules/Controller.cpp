#include "Controller.hpp"

Controller::Controller()
{
    board::setHmiHandler(std::bind(&Controller::handleHmiSelection, this, std::placeholders::_1));
}

void Controller::handleHmiSelection(const board::HmiSelection selection)
{
    using namespace std::chrono_literals;
    board::playTone(mapSelectionToFrequency(selection), 1s);
    switch (selection)
    {
    case board::HmiSelection::TASK1:
    case board::HmiSelection::TASK2:
    case board::HmiSelection::TASK3:
    case board::HmiSelection::TASK4:

        break;
    default:
        break;
    }
}
