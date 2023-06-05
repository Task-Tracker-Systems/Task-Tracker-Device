#pragma once
#include "IPresenter.hpp"
#include "IStatusIndicator.hpp"
#include <array>

class Presenter : public IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) override;

  private:
    const std::array<IStatusIndicator *const, 4> statusIndicators;
};
