#pragma once
#include <cstddef>

enum class TaskIndicatorState
{
    ACTIVE,
    INACTIVE,
};

using TaskIndex = std::size_t;

class IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) = 0;
    virtual void loop() = 0;
    virtual ~IPresenter()
    {
    }
};
