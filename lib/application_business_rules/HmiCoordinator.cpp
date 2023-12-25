#include "HmiCoordinator.hpp"
#include "board_interface.hpp"
#include "serial_port_interface.hpp"
#include "tasks/Task.hpp"
#include <cstddef>
#include <iterator>
#include <pitches.hpp>
#include <stdexcept>
#include <thread>

template <class Enum>
static constexpr auto enumToInteger(const Enum enumerator)
{
    return static_cast<std::underlying_type_t<Enum>>(enumerator);
}

static Task *mapTaskSelectionToTask(const board::HmiSelection taskSelection)
{
    // TODO lookup which task selection belongs to which task object
    const std::size_t taskIndex = enumToInteger(taskSelection) - enumToInteger(board::HmiSelection::TASK1);
    if (taskIndex < device::tasks.size())
    {
        return &(std::next(std::begin(device::tasks), taskIndex)->second);
    }
    else
    {
        return nullptr;
    }
}

static unsigned int mapSelectionToFrequency(const board::HmiSelection selection)
{
    constexpr decltype(note::c3) notes[] = {note::c3, note::d3, note::e3, note::f3, note::g3, note::a3, note::b3, note::c4};
    return notes[enumToInteger(selection)];
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
        throw std::runtime_error("no status indicator for this task available");
        break;
    }
}

static void handleHmiSelection(const board::HmiSelection selection)
{
    return;
    std::this_thread::yield();
    serial_port::cout << "Handle event nr. " << enumToInteger(selection) << std::endl;
    using namespace std::chrono_literals;
    std::this_thread::yield();
    board::playTone(mapSelectionToFrequency(selection), 1s);
    switch (selection)
    {
    case board::HmiSelection::TASK1:
    case board::HmiSelection::TASK2:
    case board::HmiSelection::TASK3:
    case board::HmiSelection::TASK4: {
        Task *const optTask = mapTaskSelectionToTask(selection);
        if (optTask)
        {
            Task &task = *optTask;
            if (task.isRunning())
            {
                task.stop();
            }
            else
            {
                task.start();
            }
            board::setStatusIndicator(mapTaskToStatusIndicator(selection), task.isRunning());
        }
        else
        {
            std::this_thread::yield();
            serial_port::cout << "No task assigned to selection " << enumToInteger(selection) << std::endl;
        }
        break;
    }
    default:
        break;
    }
}

template <class CONTAINER>
static void initializeTasks(CONTAINER &tasks)
{
    // TODO get tasks from storage

    // workaround for missing storage: initialize dummy tasks
    device::tasks.emplace(31, "Task 1");
    device::tasks.emplace(32, "Task 2");
    device::tasks.emplace(33, "Task 3");
    device::tasks.emplace(34, "Task 4");
}

void hmi_coordinator::setup()
{
    board::setup(&handleHmiSelection);
    initializeTasks(device::tasks);
}
