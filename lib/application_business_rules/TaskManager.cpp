#include "TaskManager.hpp"
#include <algorithm>
#include <iterator>

Task *TaskManager::getTask(const Task::ID id)
{
    const auto taskCandidate = std::find_if(std::begin(tasks), std::end(tasks), [&id](auto &task) {
        return task.getId() == id;
    });
    if (taskCandidate != std::end(tasks))
    {
        return &*taskCandidate;
    }
    else
    {
        return nullptr;
    }
}

std::size_t TaskManager::getNumberOfTasks() const
{
    return tasks.size();
}
