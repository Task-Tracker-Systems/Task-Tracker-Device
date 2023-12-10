#include "HmiCoordinator.hpp"
#include "board_interface.hpp"
#include "serial_port_interface.hpp"
#include "tasks/Task.hpp"
#include <cstddef>
#include <iterator>
#include <pitches.hpp>
#include <stdexcept>

static Task &mapTaskSelectionToTask(const board::HmiSelection taskSelection)
{
    const std::size_t taskIndex = taskSelection - board::HmiSelection::TASK1;
    return *std::next(std::begin(device::tasks), taskIndex);
}

static unsigned int mapSelectionToFrequency(const board::HmiSelection selection)
{
    constexpr auto notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
    return notes[selection];
}

static board::StatusIndicatorId mapTaskToStatusIndicator(const board::HmiSelection selection)
{
    switch (selection)
    {
    case board::HmiSelection::TASK1:
        return board::StatusIndicatorId::TASK1;
        break;
    case board::HmiSelection::TASK2:
        return board::StatusIndicatorId::TASK2;
        break;
    case board::HmiSelection::TASK3:
        return board::StatusIndicatorId::TASK3;
        break;
    case board::HmiSelection::TASK4:
        return board::StatusIndicatorId::TASK4;
        break;
    default:
        throw std::runtime_error();
        break;
    }
}

static void handleHmiSelection(const board::HmiSelection selection)
{
    serial_port::cout << "Handle event nr. " << selection << std::endl;
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
        board::setStatusIndicator(mapTaskToStatusIndicator(selection), task.isRunning());
        break;
    default:
        break;
    }
}

void hmi_coordinator::setup()
{
    board::setup(std::bind(&handleHmiSelection, std::placeholders::_1));
}
