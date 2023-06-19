#include "display_factory_interface.hpp"
#include "Display.hpp"

namespace board
{
IDisplay &getDisplay()
{
    static Display singleton;
    return singleton;
}
} // namespace board
