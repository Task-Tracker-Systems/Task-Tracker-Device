#include "Menu.hpp"

Menu::Menu(IDisplay &displayToUse)
    : display(displayToUse)
{
    // TODO Auto-generated constructor stub
}

void Menu::loop()
{
    display.refresh();
}
