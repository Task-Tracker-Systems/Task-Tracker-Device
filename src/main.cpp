/**
 * \file
 *
 * - setup
 *   - configure hardware (pins)
 *   - initialize
 *     - serial port
 *     - display
 *   - send "hello" over serial port (could be in serial port initialization)
 * - loop
 *   - fetches event
 *   - print event
 *   - play note
 *   - (todo) forward event to (tasks)
 */

#include "board_pins.hpp"
#include "display.h"
#include "pitches.hpp"
#include <Arduino.h>
#include <HmiCoordinator.hpp>
#include <cstdint>
#include <serial_port_interface.hpp>

void setup()
{
    serial_port::initialize();
    hmi_coordinator::setup();
    display::setup_display();
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
}
void loop()
{
    display::refresh_display(); // Animate bitmaps
}
