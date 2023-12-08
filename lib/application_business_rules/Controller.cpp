#include "Controller.hpp"
#include "tasks/Task.hpp"
#include <cstddef>
#include <iterator>
#include <pitches.hpp>

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
        auto &task = mapTaskSelectionToTask(selection);
        if (task.isRunning())
        {
            task.stop();
        }
        else
        {
            task.start();
        }
        break;
    default:
        break;
    }
}

Task &Controller::mapTaskSelectionToTask(const board::HmiSelection taskSelection)
{
    const std::size_t taskIndex = taskSelection - board::HmiSelection::TASK1;
    return *std::next(std::begin(device::tasks), taskIndex);
}

unsigned int Controller::mapSelectionToFrequency(const board::HmiSelection selection)
{
    constexpr auto notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
    return notes[selection];
}
