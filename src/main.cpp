#include "main.hpp"
#include "Display.hpp"
#include "ProcessHmiInputs.hpp"
#include "controller_factory_interface.hpp"
#include "presenter_factory_interface.hpp"
#include "serial_port.hpp"
#include <Arduino.h>
#include <Protocol.hpp>

namespace main
{
void setup(char const *programIdentificationString)
{
    serial_port::initialize();
    serial_port::cout << std::endl
                      << " begin program '" << programIdentificationString << std::endl;
    serial_port::setCallbackForLineReception([](const serial_port::String &commandLine) {
        ProtocolHandler::execute(commandLine.c_str());
    });
}
void loop()
{
    constexpr unsigned long loopDurationMs = 250;
    auto &presenter = hmi::getPresenter();
    static ProcessHmiInputs processHmiInputs(hmi::getController(), presenter);
    processHmiInputs.loop();
    delay(loopDurationMs);
    presenter.loop();
}
} // namespace main
