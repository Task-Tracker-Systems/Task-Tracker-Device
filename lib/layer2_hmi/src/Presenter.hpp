#pragma once
#include "IPresenter.hpp"

class Presenter : public IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) override;
};
