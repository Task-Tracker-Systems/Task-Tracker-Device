#pragma once
#include "IDisplay.hpp"

class Menu
{
  public:
    Menu(IDisplay &);
    virtual void loop();

  private:
    IDisplay &display;
};
