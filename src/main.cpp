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
    static constexpr const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });

    device::tasks.emplace(31, "Task 1");
    device::tasks.emplace(32, "Task 2");
    device::tasks.emplace(33, "Task 3");
    device::tasks.emplace(34, "Task 4");
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
