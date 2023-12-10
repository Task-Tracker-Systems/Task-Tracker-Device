/**
 * \file
 */

#include <HmiCoordinator.hpp>
#include <display_interface.hpp>
#include <serial_port_interface.hpp>

void setup()
{
    serial_port::initialize();
    hmi_coordinator::setup();
    display::setup();
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
}

void loop()
{
    display::loop();
}
