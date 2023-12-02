/** \file */
#pragma once

#include "Task.hpp"
#include <cstddef>
#include <vector>

class TaskManager
{
  public:
    Task *getTask(const Task::ID id);
    std::size_t getNumberOfTasks() const;

  private:
    std::vector<Task> tasks;
};
