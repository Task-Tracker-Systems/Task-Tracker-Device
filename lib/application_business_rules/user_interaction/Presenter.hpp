#pragma once
#include "IStatusIndicator.hpp"
#include "Menu.hpp"
#include "hmi_configuration.hpp"
#include <cstddef>

enum class TaskIndicatorState
{
    ACTIVE,
    INACTIVE,
};

using TaskIndex = std::size_t;

class Presenter
{
  public:
    Presenter(Menu &, IStatusIndicator *const (&)[hmi::numberOfStatusIndicators]);
    void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState);
    void loop();

  private:
    Menu &menu;
    IStatusIndicator *const (&statusIndicators)[hmi::numberOfStatusIndicators];
};
