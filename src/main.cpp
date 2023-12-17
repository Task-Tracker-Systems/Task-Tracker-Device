#include "main.hpp"
#include "Controller.hpp"
#include "Display.hpp"
#include "ProcessHmiInputs.hpp"
#include "keypad_factory_interface.hpp"
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
    static Controller controller(board::getKeypad());
    static auto &presenter = hmi::getPresenter();
    static ProcessHmiInputs processHmiInputs(controller, presenter);
    processHmiInputs.loop();
    delay(loopDurationMs);
    presenter.loop();
}
} // namespace main
