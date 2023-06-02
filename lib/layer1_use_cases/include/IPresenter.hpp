#pragma once
#include <cstdint>

enum class TaskIndicatorState
{
    ACTIVE,
    INACTIVE,
};

using TaskIndex = std::uint8_t;

class IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) = 0;
    virtual ~IPresenter()
    {
    }
};
