#pragma once
#include "IPresenter.hpp"
#include "IStatusIndicator.hpp"
#include "Menu.hpp"
#include "hmi_configuration.hpp"

class Presenter : public IPresenter
{
  public:
    Presenter(Menu &, IStatusIndicator *const (&)[hmi::numberOfStatusIndicators]);
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) override;
    virtual void loop() override;

  private:
    Menu &menu;
    IStatusIndicator *const (&statusIndicators)[hmi::numberOfStatusIndicators];
};
