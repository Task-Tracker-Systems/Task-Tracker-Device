#pragma once
#include "IPresenter.hpp"
#include "IStatusIndicator.hpp"

class Presenter : public IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) override;

  private:
    const IStatusIndicator *statusIndicators[4];
};
