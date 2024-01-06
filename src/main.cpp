/**
 * \file
 */

#include "tasks/Task.hpp"
#include <HmiCoordinator.hpp>
#include <Protocol.hpp>
#include <display_interface.hpp>
#include <serial_port_interface.hpp>

void setup()
{
    serial_port::initialize();
    hmi_coordinator::setup();
    display::setup();
    serial_port::cout << "\x1b[20h"; // Tell the terminal to use CR/LF for newlines instead of just CR.
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });
}

void loop()
{
    display::loop();

    for (auto task : device::tasks)
    {
        serial_port::cout << task.second.getLabel() << " : " << std::boolalpha << task.second.isRunning()
                          << std::noboolalpha << "   with " << task.second.getRecordedDuration().count() << " s" << std::endl;
    }
    serial_port::cout << "_\r" << std::endl;

    sleep(1);
}
