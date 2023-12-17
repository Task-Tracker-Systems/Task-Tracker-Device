#include "main.hpp"
#include "ProcessHmiInputs.hpp"
#include "controller_factory_interface.hpp"
#include "display.h"
#include "presenter_factory_interface.hpp"
#include "serial_port.hpp"
#include <Arduino.h>
#include <Protocol.hpp>

namespace main
{
void setup(char const *programIdentificationString)
{
    serial_port::initialize();
    setup_display();
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });
}
void loop()
{
    constexpr unsigned long loopDurationMs = 250;
    static ProcessHmiInputs processHmiInputs(hmi::getController(), hmi::getPresenter());
    processHmiInputs.loop();
    delay(loopDurationMs);
    refresh_display(); // Animate bitmaps
}
} // namespace main
