#pragma once
#include "IDisplay.hpp"

class Menu
{
  public:
    Menu(IDisplay &);

  private:
    IDisplay &display;
};
